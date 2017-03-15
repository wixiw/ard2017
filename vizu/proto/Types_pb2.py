# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: Types.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='Types.proto',
  package='',
  syntax='proto2',
  serialized_pb=_b('\n\x0bTypes.proto*9\n\x04\x65\x44ir\x12\r\n\tUNDEFINED\x10\x00\x12\x0b\n\x07\x46ORWARD\x10\x01\x12\x15\n\x08\x42\x41\x43KWARD\x10\xff\xff\xff\xff\xff\xff\xff\xff\xff\x01*(\n\x06\x65\x43olor\x12\x0b\n\x07UNKNOWN\x10\x00\x12\x08\n\x04PREF\x10\x01\x12\x07\n\x03SYM\x10\x02*`\n\teNavState\x12\x08\n\x04IDLE\x10\x00\x12\x0f\n\x0b\x46\x41\x43ING_DEST\x10\x01\x12\x13\n\x0fGOING_TO_TARGET\x10\x02\x12\x15\n\x11TURNING_AT_TARGET\x10\x03\x12\x0c\n\x08STOPPING\x10\x04*:\n\teNavOrder\x12\x0b\n\x07NOTHING\x10\x00\x12\x08\n\x04GOTO\x10\x01\x12\x0c\n\x08GOTO_CAP\x10\x02\x12\x08\n\x04STOP\x10\x03*:\n\x0c\x65ObjectColor\x12\t\n\x05UNDEF\x10\x00\x12\x08\n\x04\x42LUE\x10\x01\x12\n\n\x06YELLOW\x10\x02\x12\t\n\x05WHITE\x10\x03*7\n\teLogLevel\x12\t\n\x05\x44\x45\x42UG\x10\x00\x12\x08\n\x04INFO\x10\x01\x12\t\n\x05\x45RROR\x10\x02\x12\n\n\x06\x41SSERT\x10\x03')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

_EDIR = _descriptor.EnumDescriptor(
  name='eDir',
  full_name='eDir',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='UNDEFINED', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='FORWARD', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='BACKWARD', index=2, number=-1,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=15,
  serialized_end=72,
)
_sym_db.RegisterEnumDescriptor(_EDIR)

eDir = enum_type_wrapper.EnumTypeWrapper(_EDIR)
_ECOLOR = _descriptor.EnumDescriptor(
  name='eColor',
  full_name='eColor',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='UNKNOWN', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='PREF', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='SYM', index=2, number=2,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=74,
  serialized_end=114,
)
_sym_db.RegisterEnumDescriptor(_ECOLOR)

eColor = enum_type_wrapper.EnumTypeWrapper(_ECOLOR)
_ENAVSTATE = _descriptor.EnumDescriptor(
  name='eNavState',
  full_name='eNavState',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='IDLE', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='FACING_DEST', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='GOING_TO_TARGET', index=2, number=2,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TURNING_AT_TARGET', index=3, number=3,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='STOPPING', index=4, number=4,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=116,
  serialized_end=212,
)
_sym_db.RegisterEnumDescriptor(_ENAVSTATE)

eNavState = enum_type_wrapper.EnumTypeWrapper(_ENAVSTATE)
_ENAVORDER = _descriptor.EnumDescriptor(
  name='eNavOrder',
  full_name='eNavOrder',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='NOTHING', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='GOTO', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='GOTO_CAP', index=2, number=2,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='STOP', index=3, number=3,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=214,
  serialized_end=272,
)
_sym_db.RegisterEnumDescriptor(_ENAVORDER)

eNavOrder = enum_type_wrapper.EnumTypeWrapper(_ENAVORDER)
_EOBJECTCOLOR = _descriptor.EnumDescriptor(
  name='eObjectColor',
  full_name='eObjectColor',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='UNDEF', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='BLUE', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='YELLOW', index=2, number=2,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='WHITE', index=3, number=3,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=274,
  serialized_end=332,
)
_sym_db.RegisterEnumDescriptor(_EOBJECTCOLOR)

eObjectColor = enum_type_wrapper.EnumTypeWrapper(_EOBJECTCOLOR)
_ELOGLEVEL = _descriptor.EnumDescriptor(
  name='eLogLevel',
  full_name='eLogLevel',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='DEBUG', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='INFO', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='ERROR', index=2, number=2,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='ASSERT', index=3, number=3,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=334,
  serialized_end=389,
)
_sym_db.RegisterEnumDescriptor(_ELOGLEVEL)

eLogLevel = enum_type_wrapper.EnumTypeWrapper(_ELOGLEVEL)
UNDEFINED = 0
FORWARD = 1
BACKWARD = -1
UNKNOWN = 0
PREF = 1
SYM = 2
IDLE = 0
FACING_DEST = 1
GOING_TO_TARGET = 2
TURNING_AT_TARGET = 3
STOPPING = 4
NOTHING = 0
GOTO = 1
GOTO_CAP = 2
STOP = 3
UNDEF = 0
BLUE = 1
YELLOW = 2
WHITE = 3
DEBUG = 0
INFO = 1
ERROR = 2
ASSERT = 3


DESCRIPTOR.enum_types_by_name['eDir'] = _EDIR
DESCRIPTOR.enum_types_by_name['eColor'] = _ECOLOR
DESCRIPTOR.enum_types_by_name['eNavState'] = _ENAVSTATE
DESCRIPTOR.enum_types_by_name['eNavOrder'] = _ENAVORDER
DESCRIPTOR.enum_types_by_name['eObjectColor'] = _EOBJECTCOLOR
DESCRIPTOR.enum_types_by_name['eLogLevel'] = _ELOGLEVEL


# @@protoc_insertion_point(module_scope)
