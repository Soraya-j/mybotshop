/* static/scripts/webserver_mic_script.js */
(function () {
    'use strict';

    let mediaStream = null;
    let audioContext = null;
    let mediaStreamSource = null;
    let processor = null;
    let silentGain = null;
    let isTransmitting = false;

    // Amplification factor (adjust between 1.0 - 10.0)
    const AMPLIFICATION_FACTOR = 2.5;  // Reduced from 3.5 to help with echo

    const btnSpeech = document.getElementById('btnSpeech');
    if (!btnSpeech) { console.error('[mic] btnSpeech not found'); return; }
    btnSpeech.disabled = false;

    // ---- helpers ----
    function downsampleBuffer(input, fromRate, toRate) {
        if (toRate === fromRate) return input;
        const ratio = fromRate / toRate;
        const newLen = Math.floor(input.length / ratio);
        const out = new Float32Array(newLen);
        let i = 0, j = 0, acc = 0, count = 0;
        while (i < input.length && j < newLen) {
            acc += input[i++]; count++;
            if (count >= ratio) { out[j++] = acc / count; acc = 0; count = 0; }
        }
        return out;
    }

    function amplifyBuffer(input, gain) {
        const output = new Float32Array(input.length);
        for (let i = 0; i < input.length; i++) {
            // Apply gain with soft clipping to prevent harsh distortion
            let amplified = input[i] * gain;
            // Soft clipping using tanh-like curve
            if (amplified > 1.0) {
                amplified = 1.0 - Math.exp(-(amplified - 1.0));
            } else if (amplified < -1.0) {
                amplified = -1.0 + Math.exp(-(Math.abs(amplified) - 1.0));
            }
            output[i] = amplified;
        }
        return output;
    }

    function floatTo16LE(float32) {
        const buf = new ArrayBuffer(float32.length * 2);
        const view = new DataView(buf);
        for (let i = 0, off = 0; i < float32.length; i++, off += 2) {
            let s = Math.max(-1, Math.min(1, float32[i]));
            view.setInt16(off, s < 0 ? s * 0x8000 : s * 0x7FFF, true);
        }
        return buf;
    }

    async function startAudioTransmit() {
        try {
            // Request high-quality audio with AGGRESSIVE echo cancellation
            const stream = await navigator.mediaDevices.getUserMedia({
                audio: {
                    echoCancellation: { ideal: true, exact: true },  // Force echo cancellation
                    noiseSuppression: { ideal: true },
                    autoGainControl: false,  // Disable AGC so we can control gain
                    sampleRate: { ideal: 48000 },
                    channelCount: 1,
                    // Additional constraints to improve quality
                    latency: 0.01,  // Low latency
                    googEchoCancellation: true,  // Chrome-specific
                    googNoiseSuppression: true,  // Chrome-specific
                    googAutoGainControl: false,
                    googHighpassFilter: true,  // Remove low-frequency noise
                    googTypingNoiseDetection: true
                }
            });
            mediaStream = stream;

            audioContext = new (window.AudioContext || window.webkitAudioContext)();
            mediaStreamSource = audioContext.createMediaStreamSource(mediaStream);

            processor = audioContext.createScriptProcessor(2048, 1, 1);
            processor.onaudioprocess = (e) => {
                if (!isTransmitting) return;
                const inBuf = e.inputBuffer.getChannelData(0);

                // Apply amplification
                const amplified = amplifyBuffer(inBuf, AMPLIFICATION_FACTOR);

                // Downsample to 16kHz
                const down = downsampleBuffer(amplified, audioContext.sampleRate, 16000);
                if (!down || down.length === 0) return;

                const pcmLE = floatTo16LE(down);

                fetch(`${location.origin}/api/audio_stream`, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/octet-stream' },
                    body: pcmLE,
                    keepalive: true
                }).catch((err) => {
                    if (Math.random() < 0.01) console.warn('[mic] POST failed:', err);
                });
            };

            // Keep graph silent (no local playback)
            silentGain = audioContext.createGain();
            silentGain.gain.value = 0.0;

            mediaStreamSource.connect(processor);
            processor.connect(silentGain);
            silentGain.connect(audioContext.destination);

            isTransmitting = true;
            btnSpeech.classList.add('speech-active');
            btnSpeech.innerHTML = '<i class="fas fa-microphone"></i> Audio: On';
            console.log('[mic] transmit started @', audioContext.sampleRate, 'Hz, gain:', AMPLIFICATION_FACTOR);
            console.log('[mic] Echo cancellation active');
        } catch (err) {
            console.error('[mic] start failed:', err);
            alert('Microphone access denied or unavailable.');
            stopAudioTransmit();
        }
    }

    function stopAudioTransmit() {
        isTransmitting = false;
        try { if (processor) processor.disconnect(); } catch { }
        try { if (silentGain) silentGain.disconnect(); } catch { }
        try { if (mediaStreamSource) mediaStreamSource.disconnect(); } catch { }
        try { if (audioContext) audioContext.close(); } catch { }
        if (mediaStream) { try { mediaStream.getTracks().forEach(t => t.stop()); } catch { } }
        mediaStream = audioContext = mediaStreamSource = processor = silentGain = null;

        btnSpeech.classList.remove('speech-active');
        btnSpeech.innerHTML = '<i class="fas fa-microphone-slash"></i> Audio: Off';
        console.log('[mic] transmit stopped');
    }

    btnSpeech.addEventListener('click', () => {
        if (isTransmitting) stopAudioTransmit();
        else startAudioTransmit();
    });

    window.addEventListener('beforeunload', () => { if (isTransmitting) stopAudioTransmit(); });

    console.log('[mic] amplified module with echo cancellation loaded');
})();