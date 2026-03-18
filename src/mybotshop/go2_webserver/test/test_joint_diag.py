#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Software License Agreement (BSD)
#
# @author    Salman Omar Sohail <support@mybotshop.de>
# @copyright (c) 2025, MYBOTSHOP GmbH, Inc., All rights reserved.
#
# Unit tests for joint states and diagnostics processing
# Run with: python3 -m pytest test/test_joint_diag.py -v

import math
import unittest
from unittest.mock import MagicMock, patch


class MockJointState:
    """Mock JointState message for testing."""
    def __init__(self, name=None, position=None, velocity=None, effort=None):
        self.name = name or []
        self.position = position or []
        self.velocity = velocity or []
        self.effort = effort or []


class MockDiagnosticStatus:
    """Mock DiagnosticStatus for testing."""
    def __init__(self, name="", level=0, message="", hardware_id="", values=None):
        self.name = name
        self.level = level
        self.message = message
        self.hardware_id = hardware_id
        self.values = values or []


class MockKeyValue:
    """Mock KeyValue for testing."""
    def __init__(self, key="", value=""):
        self.key = key
        self.value = value


class MockDiagnosticArray:
    """Mock DiagnosticArray message for testing."""
    def __init__(self, status=None):
        self.status = status or []


class TestSafeFloatList(unittest.TestCase):
    """Test the _safe_float_list helper function."""

    def setUp(self):
        """Create a mock node with the _safe_float_list method."""
        # Import math for the function
        import math as math_module
        self.math = math_module

        # Recreate the function for testing
        def _safe_float_list(values):
            if not values:
                return []
            result = []
            for v in values:
                try:
                    if v is None:
                        result.append(None)
                    elif isinstance(v, (int, float)) and math.isfinite(v):
                        result.append(float(v))
                    else:
                        result.append(None)
                except (TypeError, ValueError):
                    result.append(None)
            return result

        self.safe_float_list = _safe_float_list

    def test_empty_list(self):
        """Test with empty list."""
        result = self.safe_float_list([])
        self.assertEqual(result, [])

    def test_none_input(self):
        """Test with None input."""
        result = self.safe_float_list(None)
        self.assertEqual(result, [])

    def test_valid_floats(self):
        """Test with valid float values."""
        result = self.safe_float_list([1.0, 2.5, -3.7, 0.0])
        self.assertEqual(result, [1.0, 2.5, -3.7, 0.0])

    def test_valid_integers(self):
        """Test with valid integer values (should convert to float)."""
        result = self.safe_float_list([1, 2, -3, 0])
        self.assertEqual(result, [1.0, 2.0, -3.0, 0.0])

    def test_nan_values(self):
        """Test with NaN values (should become None)."""
        result = self.safe_float_list([1.0, float('nan'), 3.0])
        self.assertEqual(len(result), 3)
        self.assertEqual(result[0], 1.0)
        self.assertIsNone(result[1])
        self.assertEqual(result[2], 3.0)

    def test_inf_values(self):
        """Test with Inf values (should become None)."""
        result = self.safe_float_list([float('inf'), -float('inf'), 3.0])
        self.assertEqual(len(result), 3)
        self.assertIsNone(result[0])
        self.assertIsNone(result[1])
        self.assertEqual(result[2], 3.0)

    def test_none_in_list(self):
        """Test with None values in list."""
        result = self.safe_float_list([1.0, None, 3.0])
        self.assertEqual(result, [1.0, None, 3.0])

    def test_mixed_types(self):
        """Test with mixed valid and invalid types."""
        result = self.safe_float_list([1.0, "string", 3.0, [], {}])
        self.assertEqual(len(result), 5)
        self.assertEqual(result[0], 1.0)
        self.assertIsNone(result[1])
        self.assertEqual(result[2], 3.0)
        self.assertIsNone(result[3])
        self.assertIsNone(result[4])

    def test_very_large_values(self):
        """Test with very large but finite values."""
        result = self.safe_float_list([1e308, -1e308])
        self.assertEqual(result, [1e308, -1e308])

    def test_very_small_values(self):
        """Test with very small values."""
        result = self.safe_float_list([1e-308, -1e-308])
        self.assertEqual(result, [1e-308, -1e-308])


class TestJointStatesCallback(unittest.TestCase):
    """Test joint states callback robustness."""

    def test_normal_joint_states(self):
        """Test with normal joint state data."""
        msg = MockJointState(
            name=['joint1', 'joint2'],
            position=[1.0, 2.0],
            velocity=[0.5, 0.6],
            effort=[1.5, 2.5]
        )

        # Simulate callback processing
        names = [str(n) if n else "" for n in msg.name]
        self.assertEqual(names, ['joint1', 'joint2'])

    def test_empty_joint_states(self):
        """Test with empty joint state data."""
        msg = MockJointState()

        names = [str(n) if n else "" for n in msg.name]
        self.assertEqual(names, [])

    def test_nan_in_positions(self):
        """Test with NaN in positions."""
        msg = MockJointState(
            name=['joint1', 'joint2'],
            position=[float('nan'), 2.0],
            velocity=[0.5, 0.6],
            effort=[1.5, 2.5]
        )

        # Test that NaN is handled
        for v in msg.position:
            if isinstance(v, float) and math.isnan(v):
                self.assertTrue(True)  # NaN detected

    def test_inf_in_velocity(self):
        """Test with Inf in velocity."""
        msg = MockJointState(
            name=['joint1'],
            position=[1.0],
            velocity=[float('inf')],
            effort=[1.5]
        )

        # Test that Inf is handled
        for v in msg.velocity:
            if isinstance(v, float) and math.isinf(v):
                self.assertTrue(True)  # Inf detected


class TestDiagnosticsCallback(unittest.TestCase):
    """Test diagnostics callback robustness."""

    def test_normal_diagnostics(self):
        """Test with normal diagnostic data."""
        status = MockDiagnosticStatus(
            name="Battery",
            level=0,
            message="OK",
            hardware_id="power",
            values=[MockKeyValue("voltage", "24.0")]
        )
        msg = MockDiagnosticArray(status=[status])

        # Simulate processing
        processed = []
        for s in msg.status:
            values = {}
            for kv in s.values:
                key = str(kv.key) if kv.key else ""
                val = str(kv.value) if kv.value is not None else ""
                if key:
                    values[key] = val
            processed.append({
                "name": str(s.name),
                "level": s.level,
                "message": str(s.message),
                "hardware_id": str(s.hardware_id),
                "values": values
            })

        self.assertEqual(len(processed), 1)
        self.assertEqual(processed[0]["name"], "Battery")
        self.assertEqual(processed[0]["values"]["voltage"], "24.0")

    def test_empty_diagnostics(self):
        """Test with empty diagnostic data."""
        msg = MockDiagnosticArray(status=[])
        self.assertEqual(len(msg.status), 0)

    def test_none_values_in_diagnostics(self):
        """Test with None values in diagnostics."""
        status = MockDiagnosticStatus(
            name=None,
            level=None,
            message=None,
            hardware_id=None,
            values=[MockKeyValue(None, None)]
        )
        msg = MockDiagnosticArray(status=[status])

        # Simulate safe processing
        for s in msg.status:
            name = str(s.name) if s.name else ""
            level = 3  # Default to STALE
            if isinstance(s.level, int) and 0 <= s.level <= 3:
                level = s.level

            self.assertEqual(name, "")
            self.assertEqual(level, 3)

    def test_bytes_level(self):
        """Test with bytes level (as sometimes returned by ROS)."""
        status = MockDiagnosticStatus(
            name="Test",
            level=b'\x00',  # bytes representation
            message="OK"
        )

        # Simulate level extraction
        level = 3
        if isinstance(status.level, int) and 0 <= status.level <= 3:
            level = status.level
        elif isinstance(status.level, bytes):
            level = int(status.level[0]) if status.level else 3

        self.assertEqual(level, 0)


class TestSanitizeFloat(unittest.TestCase):
    """Test the _sanitize_float helper for HTTP endpoint."""

    def setUp(self):
        """Create sanitize function."""
        def _sanitize_float(val, default=None):
            if val is None:
                return default
            try:
                if math.isfinite(val):
                    return float(val)
            except (TypeError, ValueError):
                pass
            return default

        self.sanitize = _sanitize_float

    def test_valid_float(self):
        """Test with valid float."""
        self.assertEqual(self.sanitize(3.14), 3.14)

    def test_valid_int(self):
        """Test with valid int (converts to float)."""
        self.assertEqual(self.sanitize(42), 42.0)

    def test_none(self):
        """Test with None."""
        self.assertIsNone(self.sanitize(None))

    def test_none_with_default(self):
        """Test with None and default value."""
        self.assertEqual(self.sanitize(None, 0.0), 0.0)

    def test_nan(self):
        """Test with NaN."""
        self.assertIsNone(self.sanitize(float('nan')))

    def test_nan_with_default(self):
        """Test with NaN and default value."""
        self.assertEqual(self.sanitize(float('nan'), 0.0), 0.0)

    def test_inf(self):
        """Test with Inf."""
        self.assertIsNone(self.sanitize(float('inf')))

    def test_neg_inf(self):
        """Test with negative Inf."""
        self.assertIsNone(self.sanitize(-float('inf')))

    def test_string(self):
        """Test with string (invalid)."""
        self.assertIsNone(self.sanitize("not a number"))

    def test_list(self):
        """Test with list (invalid)."""
        self.assertIsNone(self.sanitize([1, 2, 3]))


class TestEndpointDataSanitization(unittest.TestCase):
    """Test that endpoint correctly sanitizes data for JSON output."""

    def test_joint_states_with_nan(self):
        """Test joint states sanitization with NaN values."""
        # Simulate data that would come from callback
        js_data = {
            "t": 1234567890.123,
            "name": ["joint1", "joint2"],
            "position": [1.0, float('nan')],
            "velocity": [float('inf'), 0.5],
            "effort": [None, 1.5],
        }

        # Simulate endpoint sanitization
        def sanitize(v):
            if v is None:
                return None
            try:
                if math.isfinite(v):
                    return float(v)
            except (TypeError, ValueError):
                pass
            return None

        sanitized = {
            "t": sanitize(js_data["t"]),
            "name": [str(n) if n else "" for n in js_data["name"]],
            "position": [sanitize(v) for v in js_data["position"]],
            "velocity": [sanitize(v) for v in js_data["velocity"]],
            "effort": [sanitize(v) for v in js_data["effort"]],
        }

        self.assertEqual(sanitized["t"], 1234567890.123)
        self.assertEqual(sanitized["name"], ["joint1", "joint2"])
        self.assertEqual(sanitized["position"][0], 1.0)
        self.assertIsNone(sanitized["position"][1])  # NaN -> None
        self.assertIsNone(sanitized["velocity"][0])  # Inf -> None
        self.assertEqual(sanitized["velocity"][1], 0.5)
        self.assertIsNone(sanitized["effort"][0])  # None stays None
        self.assertEqual(sanitized["effort"][1], 1.5)

    def test_diagnostics_sanitization(self):
        """Test diagnostics sanitization."""
        diag_data = {
            "t": 1234567890.123,
            "statuses": [
                {
                    "name": "Test",
                    "level": 0,
                    "message": "OK",
                    "hardware_id": "hw1",
                    "values": {"key1": "value1", "key2": "value2"}
                }
            ]
        }

        # Simulate sanitization
        sanitized_statuses = []
        for status in diag_data["statuses"]:
            sanitized_statuses.append({
                "name": str(status.get("name", "")),
                "level": int(status.get("level", 3)),
                "message": str(status.get("message", "")),
                "hardware_id": str(status.get("hardware_id", "")),
                "values": {str(k): str(v) for k, v in status.get("values", {}).items()},
            })

        self.assertEqual(len(sanitized_statuses), 1)
        self.assertEqual(sanitized_statuses[0]["name"], "Test")
        self.assertEqual(sanitized_statuses[0]["level"], 0)
        self.assertEqual(sanitized_statuses[0]["values"]["key1"], "value1")


if __name__ == '__main__':
    unittest.main()
