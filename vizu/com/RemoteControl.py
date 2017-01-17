#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from ArdHdlc import *
from proto import *

#
# This class is a network middleware allowing SW to call RPC method 
# A message is created behing any RPC call, it acts as a decorator of ArdHdlc
# It is a Level 6 OSI layer (Presentation Layer), encoding data with procol buffer 
#
class RemoteControl(QObject):
    
    #-------------------------
    # TELEOP RECEIVE API
    #-------------------------
    log             = pyqtSignal(RemoteControl_pb2.Log)
    osStats         = pyqtSignal(CommonMsg_pb2.EmptyMsg)
    rspState        = pyqtSignal(RemoteControl_pb2.RSPState)
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
        
    # go throught decorator
    def disconnect(self):
        self.com.disconnect()
      
    # go throught decorator  
    def isConnected(self):
        self.com.isConnected()
               
    # decode an HDLC frame payload to get the message, which is the RemoteControlResponse type
    @pyqtSlot(bytes)
    def _frameReceived(self, data):
        response = RemoteControl_pb2.RemoteControlResponse()
        try:
            response.ParseFromString(data)
        except:
            print("Failed to decode protobuf ms : " + str(data))
            print(str(response))
            traceback.print_exc()
        else:
            if response.WhichOneof("type") != None:
                print("RemoteControl message received:")
                print(str(response))
                signal = getattr(self, response.WhichOneof("type"))
                subMsg = getattr(response, response.WhichOneof("type"))
                signal.emit(subMsg)
            else:
                print("RemoteControl : Unkown message " + str(response))
        
            
    #serialize and send the message on communication link
    #do not use this function directly, use TELEOP API below
    def _sendMsg(self, msg):
        assert isinstance(msg, RemoteControl_pb2.RemoteControlRequest), "RemoteControl_pb2._sendMsg expects to receive a RemoteControlRequest class"
        print("RemoteControl request : " + str(msg))

        if self.com.sendMsg(msg.SerializeToString()):
            print("Frame sent successfully.")
        else:
            print("Frame send error.")
            
#---------------------------------------------------------------------------------
# TELEOP API : any of the method below send a msg with the same name in RemoteControl_pb2.py
#---------------------------------------------------------------------------------
    
    @pyqtSlot()
    def getOsStats(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.getOsStats.SetInParent()
        self._sendMsg(msg)
    
    @pyqtSlot(int, int)
    def configureMatch(self, strategy, color):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.configureMatch.strategy = strategy
        msg.configureMatch.matchColor = color
        self._sendMsg(msg)
    
    @pyqtSlot()
    def startMatch(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.startMatch.SetInParent()
        self._sendMsg(msg)
        
    @pyqtSlot(int,int,float)
    def setPosition(self, x,y,h):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.setPosition.x = x
        msg.setPosition.y = y
        msg.setPosition.h = h
        self._sendMsg(msg)
        
    @pyqtSlot(int,int,float,int)
    def requestGotoCap(self, x,y,h,dir):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestGotoCap.target.x = x
        msg.requestGotoCap.target.y = y
        msg.requestGotoCap.target.h = h
        msg.requestGotoCap.direction = dir
        self._sendMsg(msg)
        
    @pyqtSlot(int,int,int)
    def requestGoto(self, x,y,dir):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestGoto.target.x = x
        msg.requestGoto.target.y = y
        msg.requestGoto.direction = dir
        self._sendMsg(msg)
        