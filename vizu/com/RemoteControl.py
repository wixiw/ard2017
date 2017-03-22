#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from ArdHdlc import *
from core import *

#expand path to find modules :
import sys
#Find the directory in which vizu.py is located
from os.path import dirname, abspath
DIR = dirname(abspath(__file__))
sys.path.append(DIR + "../../com")
from generated import *

#
# This class is a network middleware allowing SW to call RPC method 
# A message is created behing any RPC call, it acts as a decorator of ArdHdlc
# It is a Level 6 OSI layer (Presentation Layer), encoding data with procol buffer 
#
class RemoteControl(QObject):
    
    #-------------------------
    # TELEOP RECEIVE/Reply API
    #-------------------------
    log             = pyqtSignal(RemoteControl_pb2.Log)
    osStats         = pyqtSignal(CommonMsg_pb2.EmptyMsg)
    telemetry       = pyqtSignal(RemoteControl_pb2.Telemetry)
    #-------------------------
    
    #trigger listeners
    
    
    def __init__(self):
        super().__init__() 
        self.com = ArdHdlc()
        self.timer_telemetry = QTimer(self)
        self.timer_telemetry.timeout.connect(self._telemetryTick)
      
      
    def __del__(self):
        del self.timer_telemetry
          
#---------------------------------------------------------------------------------
# Public API :
#---------------------------------------------------------------------------------

    #@return [list(str), list(str)] : list of ports and baudrates
    def getSerialPortInfo(self):
        return self.com.getAvailablePorts(), self.com.getAvailableBaudrates()
    
    # Connect to the specified port at the specified baudrate
    # If the connection is established, then start the polling thread to get telemetry
    # @param str port : like "COM1", ideally from getAvailablePorts()
    # @param int baudrate : ideally from getAvailableBaudrates()
    # @return bool : true if the connection succeed, false otherwise
    def connect(self, port, baudrate):
        res = self.com.connect(port, baudrate, self._frameReceived)
        if res:
          self.timer_telemetry.start(100)  
        return res 
        
    # go throught decorator
    def disconnect(self):
        self.timer_telemetry.stop()
        self.com.disconnect()
      
    # go throught decorator  
    def isConnected(self):
        self.com.isConnected()
    
#---------------------------------------------------------------------------------
# TELEOP SEND/Request API : any of the method below send a msg with the same name in RemoteControl_pb2.py
#---------------------------------------------------------------------------------
    @pyqtSlot()
    def getOsStats(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.getOsStats.SetInParent()
        self._sendMsg(msg)
    
    @pyqtSlot()
    def getOsStatsLogs(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.getOsStatsLogs.SetInParent()
        self._sendMsg(msg)
        
    @pyqtSlot()
    def getTelemetry(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.getTelemetry.SetInParent()
        self._sendMsg(msg)
    
    @pyqtSlot(Melody)
    def requestPlaySound(self, melody):
        msg = RemoteControl_pb2.RemoteControlRequest()
        melody.toMsg(msg.requestPlaySound)
        self._sendMsg(msg)
    
    @pyqtSlot()
    def resetCpu(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.reboot.SetInParent()
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
        
    @pyqtSlot(Pose2D)
    def setPosition(self, pose):
        msg = RemoteControl_pb2.RemoteControlRequest()
        pose.toPoseMsg(msg.setPosition)
        self._sendMsg(msg)
        
    @pyqtSlot(Pose2D, int)
    def requestGotoCap(self, pose, dir):
        msg = RemoteControl_pb2.RemoteControlRequest()
        pose.toPoseMsg(msg.requestGotoCap.target)
        msg.requestGotoCap.direction = dir
        self._sendMsg(msg)
        
    @pyqtSlot(Point ,int)
    def requestGoto(self, point, dir):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestGoto.target.x = point.x
        msg.requestGoto.target.y = point.y
        msg.requestGoto.direction = dir
        self._sendMsg(msg)
        
    @pyqtSlot()
    def requestMaxLengthMsg(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestMaxLengthMsg.SetInParent()
        self._sendMsg(msg)
        
    @pyqtSlot()
    def requestCrcFailMsg(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestCrcFailMsg.SetInParent()
        self._sendMsg(msg)
        
    @pyqtSlot()
    def requestTooLittleMsg(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestTooLittleMsg.SetInParent()
        self._sendMsg(msg)
        
#---------------------------------------------------------------------------------
# Private/internal API :
#---------------------------------------------------------------------------------
  
    # decode an HDLC frame payload to get the message, which is the RemoteControlResponse type
    @pyqtSlot(bytes)
    def _frameReceived(self, data):
        response = RemoteControl_pb2.RemoteControlResponse()
        try:
            response.ParseFromString(data)
        except:
            print("Failed to decode protobuf msg : " + str(data))
            print(str(response))
            #traceback.print_exc()
        else:
            try:
                #--- DEBUG --- print("RemoteControl message received:")
                #--- DEBUG --- print(str(response))
                signal = getattr(self, response.WhichOneof("type"))
                subMsg = getattr(response, response.WhichOneof("type"))
                signal.emit(subMsg)
            except:
                print("RemoteControl : Unkown message " + str(response))
        
            
    #serialize and send the message on communication link
    #do not use this function directly, use TELEOP API above
    def _sendMsg(self, msg):
        assert isinstance(msg, RemoteControl_pb2.RemoteControlRequest), "RemoteControl_pb2._sendMsg expects to receive a RemoteControlRequest class"
        #---DEBUG--- print("RemoteControl request : " + str(msg))

        if self.com.sendMsg(msg.SerializeToString()):
            #--- DEBUG---print("Frame sent successfully.")
            pass
        else:
            print("Frame send error.")

    #self.timer_telemetry Callback : send a telemetry data request  
    @pyqtSlot()
    def _telemetryTick(self):
        #print("tick")
        self.getTelemetry()
        pass
        
        
        