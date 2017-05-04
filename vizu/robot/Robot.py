#!/usr/bin/python3
# -*- coding: utf-8 -*-

import signal
import math
from ArdMath import *
from Music import *
from PyQt5.Qt import *
from PyQt5.QtCore import *
from generated import *
from core import *

#
# This widget represent the robot
#
class Robot(QObject):
    def __init__(self):
        super().__init__()
        self.state = RemoteControl_pb2.Telemetry()
        self.past = list()
        
    @pyqtSlot(RemoteControl_pb2.Telemetry)     
    def _telemetryDataCb(self, msg):
        self.state = msg
        if msg.chrono.chrono_ms != 0 and msg.chrono.timeLeft_ms !=0 :
            self.past.append(msg)
            
    @pyqtSlot(RemoteControl_pb2.SerialNumber)
    def _handleSerialNumber(self, serial):
        print("--- Robot reboot ---")
        self.past.clear()