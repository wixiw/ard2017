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
        try:
            self.log.emit(data.decode())
        except:
            self.log.emit("XXX unreadable log XXX")
            
    
    #serialize and send the message on communication link
    #do not use this function directly, use TELEOP API below
    def _sendMsg(self, msg):
        assert isinstance(msg, TeleopRequest), "Teleop._sendMsg expects to receive a TeleopRequest class"
        print("Teleop request : " + str(msg))
        #---DEBUG---- print("Msg send request : [%s]" % msg)
        # sequence id is not used yet, as Teleop doesn't require any robustness, 
        # in order to use this com with a robot to robot com, it'll be required
        frame = frame_data(msg.SerializeToString(), FRAME_DATA, 1)
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
        msg = TeleopRequest()
        msg.getOsStats.SetInParent()
        self._sendMsg(msg)
    
    @pyqtSlot(int, int)
    def configureMatch(self, strategy, color):
        msg = TeleopRequest()
        msg.configureMatch.strategy = strategy
        msg.configureMatch.matchColor = color
        self._sendMsg(msg)
    
    @pyqtSlot()
    def startMatch(self):
        msg = TeleopRequest()
        msg.startMatch.SetInParent()
        self._sendMsg(msg)
        
    @pyqtSlot(int,int,float)
    def setPosition(self, x,y,h):
        msg = TeleopRequest()
        msg.setPosition.x = x
        msg.setPosition.y = y
        msg.setPosition.h = h
        self._sendMsg(msg)
        
    @pyqtSlot(int,int,float,int)
    def requestGotoCap(self, x,y,h,dir):
        msg = TeleopRequest()
        msg.requestGotoCap.target.x = x
        msg.requestGotoCap.target.y = y
        msg.requestGotoCap.target.h = h
        msg.requestGotoCap.direction = dir
        self._sendMsg(msg)
        
    @pyqtSlot(int,int,int)
    def requestGoto(self, x,y,dir):
        msg = TeleopRequest()
        msg.requestGoto.target.x = x
        msg.requestGoto.target.y = y
        msg.requestGoto.direction = dir
        self._sendMsg(msg)
        
        