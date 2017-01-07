#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from com.ArdSerial import *
from yahdlc import *
from proto import *

#
# This class manage a serial connection
# It acts as a decorator of QSerialPort in order to protect and simplify its API
# When the class is killed, the serial is automatically closed
#
class Teleop(QObject):
    
    #QT emitted signals :
    log = pyqtSignal(str)
    
    def __init__(self):
        super().__init__() 
        self.com = ArdSerial()
        self.com.registerListener(self._dataAvailable)
                    
    @pyqtSlot()
    def _dataAvailable(self):
        data = self.com.readAll()
        self.log.emit(data.decode())
    
    def _sendMsg(self, msg):
        assert isinstance(msg, bytes), "Teleop._sendMsg expects to receive a byte array"
        #---DEBUG---- print("Msg send request : [%s]" % msg)
        # sequence id is not used yet, as Teleop doesn't require any robustness, 
        # in order to use this com with a robot to robot com, it'll be required
        frame = frame_data(msg, FRAME_DATA, 1)
        #---DEBUG---- print("HLDC frame : [%s]" % frame)
        if self.com.write(frame) == len(frame):
            print("Frame sent successfully.")
        else:
            print("Frame send error.")
            
#---------------------------------------------------------------------------------
# TELEOP API : any of the method below send a msg with the same name in Teleop.py
#---------------------------------------------------------------------------------
    
    @pyqtSlot()
    def getOsStats(self):
        msg = Teleop_pb2.TeleopRequest()
        msg.getOsStats.SetInParent()
        print("Teleop request : " + str(msg))
        self._sendMsg(msg.SerializeToString())
    
    @pyqtSlot(int, int)
    def configureMatch(self, strategy, color):
        msg = Teleop_pb2.TeleopRequest()
        msg.configureMatch.strategy = strategy
        msg.configureMatch.matchColor = color
        print("Teleop request : " + str(msg))
        self._sendMsg(msg.SerializeToString())
    
    @pyqtSlot()
    def startMatch(self):
        msg = Teleop_pb2.TeleopRequest()
        msg.startMatch.SetInParent()
        print("Teleop request : " + str(msg))
        self._sendMsg(msg.SerializeToString())