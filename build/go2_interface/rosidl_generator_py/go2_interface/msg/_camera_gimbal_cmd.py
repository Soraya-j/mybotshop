# generated from rosidl_generator_py/resource/_idl.py.em
# with input from go2_interface:msg/CameraGimbalCmd.idl
# generated code does not contain a copyright notice


# Import statements for member types

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_CameraGimbalCmd(type):
    """Metaclass of message 'CameraGimbalCmd'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('go2_interface')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'go2_interface.msg.CameraGimbalCmd')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__camera_gimbal_cmd
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__camera_gimbal_cmd
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__camera_gimbal_cmd
            cls._TYPE_SUPPORT = module.type_support_msg__msg__camera_gimbal_cmd
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__camera_gimbal_cmd

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class CameraGimbalCmd(metaclass=Metaclass_CameraGimbalCmd):
    """Message class 'CameraGimbalCmd'."""

    __slots__ = [
        '_yaw',
        '_pitch',
        '_gimbal_mode',
    ]

    _fields_and_field_types = {
        'yaw': 'int64',
        'pitch': 'int64',
        'gimbal_mode': 'int8',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('int64'),  # noqa: E501
        rosidl_parser.definition.BasicType('int64'),  # noqa: E501
        rosidl_parser.definition.BasicType('int8'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.yaw = kwargs.get('yaw', int())
        self.pitch = kwargs.get('pitch', int())
        self.gimbal_mode = kwargs.get('gimbal_mode', int())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.yaw != other.yaw:
            return False
        if self.pitch != other.pitch:
            return False
        if self.gimbal_mode != other.gimbal_mode:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property
    def yaw(self):
        """Message field 'yaw'."""
        return self._yaw

    @yaw.setter
    def yaw(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'yaw' field must be of type 'int'"
            assert value >= -9223372036854775808 and value < 9223372036854775808, \
                "The 'yaw' field must be an integer in [-9223372036854775808, 9223372036854775807]"
        self._yaw = value

    @property
    def pitch(self):
        """Message field 'pitch'."""
        return self._pitch

    @pitch.setter
    def pitch(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'pitch' field must be of type 'int'"
            assert value >= -9223372036854775808 and value < 9223372036854775808, \
                "The 'pitch' field must be an integer in [-9223372036854775808, 9223372036854775807]"
        self._pitch = value

    @property
    def gimbal_mode(self):
        """Message field 'gimbal_mode'."""
        return self._gimbal_mode

    @gimbal_mode.setter
    def gimbal_mode(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'gimbal_mode' field must be of type 'int'"
            assert value >= -128 and value < 128, \
                "The 'gimbal_mode' field must be an integer in [-128, 127]"
        self._gimbal_mode = value
