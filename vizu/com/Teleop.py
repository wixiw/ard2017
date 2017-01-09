#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from com import *
from proto import *

#
# This class is a network middleware allowing SW to call RPC method 
# A message is created behing any RPC call, it acts as a decorator of ArdHdlc
# It is a Level 6 OSI layer (Presentation Layer), encoding data with procol buffer 
#
class Teleop(QObject):
    
    #-------------------------
    # TELEOP RECEIVE API
    #-------------------------
    log             = pyqtSignal(Teleop_pb2.Log)
    osStats         = pyqtSignal(CommonMsg_pb2.EmptyMsg)
    rspState        = pyqtSignal(Teleop_pb2.RSPState)
    #-------------------------
    
    def __init__(self):
        super().__init__() 
        self.com = ArdHdlc()

    #@return [list(str), list(str)] : list of ports and baudrates
    def getSerialPortInfo(self):
        return self.com.getAvailablePorts(), self.com.getAvailableBaudrates()
    
    # Connect to the specified port at the specified baudrate
    # @param str port : like "COM1", ideally from getAvailablePorts()
    # @param int baudrate : ideally from getAvailableBaudrates()
    # @return bool : true if the connection succeed, false otherwise
    def connect(self, port, baudrate):
        return self.com.connect(port, baudrate, self._frameReceived)
        
    # go throw decorator
    def disconnect(self):
        self.com.disconnect()
               
    # decode an HDLC frame payload to get the message, which is the TeleopResponse type
    @pyqtSlot(bytes)
    def _frameReceived(self, data):
        try:
            response = ParseFromString(data)
        except:
            print("Failed to decode protobuf ms : " + str(data))
        else:
            if response.WhichOneof("type") != None:
                print("Teleop : message received " + str(response))
                signal = getattr(self, "_" + response.WhichOneof("type"))
                subMsg = getattr(response, response.WhichOneof("type"))
                signal.emit(subMsg)
            else:
                print("Teleop : Unkown message " + str(response))
        
            
    #serialize and send the message on communication link
    #do not use this function directly, use TELEOP API below
    def _sendMsg(self, msg):
        assert isinstance(msg, Teleop_pb2.TeleopRequest), "Teleop_pb2._sendMsg expects to receive a TeleopRequest class"
        print("Teleop request : " + str(msg))

        if self.com.sendMsg(msg.SerializeToString()):
            print("Frame sent successfully.")
        else:
            print("Frame send error.")
            
#---------------------------------------------------------------------------------
# TELEOP API : any of the method below send a msg with the same name in Teleop_pb2.py
#---------------------------------------------------------------------------------
    
    @pyqtSlot()
    def getOsStats(self):
        msg = Teleop_pb2.TeleopRequest()
        msg.getOsStats.SetInParent()
        self._sendMsg(msg)
    
    @pyqtSlot(int, int)
    def configureMatch(self, strategy, color):
        msg = Teleop_pb2.TeleopRequest()
        msg.configureMatch.strategy = strategy
        msg.configureMatch.matchColor = color
        self._sendMsg(msg)
    
    @pyqtSlot()
    def startMatch(self):
        msg = Teleop_pb2.TeleopRequest()
        msg.startMatch.SetInParent()
        self._sendMsg(msg)
        
    @pyqtSlot(int,int,float)
    def setPosition(self, x,y,h):
        msg = Teleop_pb2.TeleopRequest()
        msg.setPosition.x = x
        msg.setPosition.y = y
        msg.setPosition.h = h
        self._sendMsg(msg)
        
    @pyqtSlot(int,int,float,int)
    def requestGotoCap(self, x,y,h,dir):
        msg = Teleop_pb2.TeleopRequest()
        msg.requestGotoCap.target.x = x
        msg.requestGotoCap.target.y = y
        msg.requestGotoCap.target.h = h
        msg.requestGotoCap.direction = dir
        self._sendMsg(msg)
        
    @pyqtSlot(int,int,int)
    def requestGoto(self, x,y,dir):
        msg = Teleop_pb2.TeleopRequest()
        msg.requestGoto.target.x = x
        msg.requestGoto.target.y = y
        msg.requestGoto.direction = dir
        self._sendMsg(msg)
        
        