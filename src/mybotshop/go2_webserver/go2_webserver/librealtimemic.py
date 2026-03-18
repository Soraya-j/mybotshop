#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import shutil
import threading
import subprocess
import time
import struct
import array


class _AudioStreamSink:
    """
    Persistent raw-PCM sink with amplification using 'aplay' (S16_LE, 16 kHz, mono).
    Auto-detects a working ALSA device and applies volume boost.
    """

    def __init__(self, node, preferred_device: str = None, amplification: float = 1.8):
        self._node = node
        self._lock = threading.Lock()
        self._proc = None
        self._bytes = 0
        self._start_ts = time.time()
        # Clamp 1-10x (reduced default)
        self._amplification = max(1.0, min(10.0, amplification))

        if not shutil.which("aplay"):
            node.get_logger().error("`aplay` not found. Install `alsa-utils`.")
            raise RuntimeError("aplay missing")

        # Build candidate list
        env_pcm = os.environ.get("ALSA_PCM") or os.environ.get("AUDIODEV")
        cands = []
        if preferred_device:
            cands.append(preferred_device)
        if env_pcm:
            cands.append(env_pcm)
        # Pulse/pipewire usually expose these
        cands += ["pulse", "default"]
        # Common hw ids
        cands += ["plughw:0,0", "hw:0,0", "plughw:1,0", "hw:1,0"]

        self._device = self._pick_device(cands)
        self._args = [
            "aplay", "-t", "raw", "-f", "S16_LE", "-r", "16000", "-c", "1",
            "-D", self._device, "-q", "-"
        ]
        self._ensure_proc()

        # Set system volume to maximum if using PulseAudio/PipeWire
        self._maximize_volume()

    def _pick_device(self, candidates):
        # Try each device by attempting to open/close it quickly
        self._node.get_logger().info(self._node.colorize(
            f"Probing audio devices: {candidates}", "blue"))
        for dev in candidates:
            try:
                # Open and immediately close aplay; feed tiny silence
                p = subprocess.Popen(
                    ["aplay", "-t", "raw", "-f", "S16_LE", "-r",
                        "16000", "-c", "1", "-D", dev, "-q", "-"],
                    stdin=subprocess.PIPE, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL
                )
                try:
                    p.stdin.write(b"\x00" * 320)  # 10 ms @ 16k mono s16
                    p.stdin.close()
                except Exception:
                    pass
                rc = p.wait(timeout=1.0)
                # If open failed, aplay usually exits nonzero immediately
                if rc == 0:
                    self._node.get_logger().info(self._node.colorize(
                        f"Audio device selected: {dev}", "blue"))
                    return dev
            except Exception:
                pass
        # Last resort: let ALSA decide
        self._node.get_logger().info(self._node.colorize(
            "[WARN] No candidate device opened; falling back to 'default'", "orange"))
        return "default"

    def _maximize_volume(self):
        """Attempt to set system volume to maximum"""
        try:
            # Try PulseAudio/PipeWire volume control
            if shutil.which("pactl"):
                subprocess.run(
                    ["pactl", "set-sink-volume", "@DEFAULT_SINK@", "100%"],
                    stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=1.0
                )
        except Exception:
            pass

        try:
            # Try ALSA mixer
            if shutil.which("amixer"):
                subprocess.run(
                    ["amixer", "-D", self._device, "sset", "Master", "100%"],
                    stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=1.0
                )
        except Exception:
            pass

    def _ensure_proc(self):
        if self._proc is None or self._proc.poll() is not None:
            try:
                self._proc = subprocess.Popen(
                    self._args, stdin=subprocess.PIPE)
                self._node.get_logger().info(self._node.colorize(
                    f"ROS2 Real-time Mic Audio Sink Initialized -> {self._device} "
                    f"(Amplification: {self._amplification:.1f}x)", "green"))
            except Exception as e:
                self._node.get_logger().error(
                    f"Failed to start aplay on {self._device}: {e}")
                self._proc = None

    def _amplify_pcm(self, data: bytes) -> bytes:
        """Amplify 16-bit PCM audio data"""
        if self._amplification == 1.0:
            return data

        try:
            # Unpack S16_LE samples
            samples = array.array('h', data)  # 'h' = signed short (16-bit)

            # Apply amplification with soft clipping
            for i in range(len(samples)):
                amplified = int(samples[i] * self._amplification)
                # Hard limit to prevent overflow
                samples[i] = max(-32768, min(32767, amplified))

            return samples.tobytes()
        except Exception as e:
            self._node.get_logger().error(f"Amplification failed: {e}")
            return data

    def write(self, data: bytes):
        if not data:
            return
        with self._lock:
            self._ensure_proc()
            if not (self._proc and self._proc.stdin):
                return
            try:
                # Amplify the audio before writing
                amplified_data = self._amplify_pcm(data)
                self._proc.stdin.write(amplified_data)
                self._bytes += len(data)
                # do not flush per packet
            except Exception as e:
                self._node.get_logger().error(
                    f"ROS2 Audio sink write failed: {e}")
                try:
                    self._proc.kill()
                except Exception:
                    pass
                self._proc = None  # restart on next write

    def set_amplification(self, factor: float):
        """Adjust amplification factor on the fly (1.0 - 10.0)"""
        self._amplification = max(1.0, min(10.0, factor))
        self._node.get_logger().info(self._node.colorize(
            f"Amplification set to {self._amplification:.1f}x", "cyan"))

    def stats(self):
        dur = max(1e-6, time.time() - self._start_ts)
        return {
            "device": self._device,
            "bytes_received": self._bytes,
            "seconds": dur,
            "kbps": (self._bytes * 8 / 1000.0) / dur,
            "amplification": self._amplification
        }
