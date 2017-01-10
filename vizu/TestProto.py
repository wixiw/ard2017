#!/usr/bin/python3
# -*- coding: utf-8 -*-

#expand path to find modules :
import sys
sys.path.append("com")
sys.path.append("proto")

from PyQt5.Qt import *
from PyQt5.QtCore import *

from com import *
from proto import *


if __name__ == '__main__':
    import sys
    import os
    import traceback
    #re-generate proto (not optimal, but as they will change a lot at project beginning...)
    os.system("..\generateCom.bat ..\\ off")
    
    #message filling
    msgLog = Teleop_pb2.TeleopResponse()
    msgLog.log.date = 1234
    msgLog.log.level = Types_pb2.DEBUG
    msgLog.log.text = "abcd"
    
    #serialization
    logPayload = msgLog.SerializeToString()
    msgLogUnser = Teleop_pb2.TeleopResponse()
    msgLogUnser.ParseFromString(logPayload)
    #display
    print("text       =" + ":".join("{:02x}".format()) for c in "abcd")
    print("msgLog     =" + str(msgLog))
    import binascii
    print("logPayload =" + str(binascii.hexlify(logPayload)))
    print("msgLogUnser=" + str(msgLogUnser))
    
    #force a decoding error
    try:
        msgLogUnser.ParseFromString(b'1234567')
    except:
        traceback.print_exc()
        print("ZIZIZIZIZI ")