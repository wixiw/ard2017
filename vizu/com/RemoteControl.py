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

import time

#
# This class is a network middleware allowing SW to call RPC method 
# A message is created behing any RPC call, it acts as a decorator of ArdHdlc
# It is a Level 6 OSI layer (Presentation Layer), encoding data with procol buffer 
#
# Telemetry polling is started as soon as a bootup is received
#
class RemoteControl(QObject):
    
     #connection state :
    networkStatus = pyqtSignal(bool)
    
    #-------------------------
    # TELEOP RECEIVE/Reply API
    #-------------------------
    log             = pyqtSignal(RemoteControl_pb2.Log)
    config          = pyqtSignal(RemoteControl_pb2.Configuration)
    serialNumber    = pyqtSignal(RemoteControl_pb2.SerialNumber)
    telemetry       = pyqtSignal(RemoteControl_pb2.Telemetry)
    graphState     = pyqtSignal(RemoteControl_pb2.GraphState)
    graphNodes     = pyqtSignal(RemoteControl_pb2.GraphNodes)
    graphLinks     = pyqtSignal(RemoteControl_pb2.GraphLinks)
    #-------------------------
    
    #trigger listeners
    
    
    def __init__(self):
        super().__init__() 
        self.com = ArdHdlc()
        self.timer_telemetry = QTimer(self)
        self.timer_telemetry.timeout.connect(self._telemetryTick)
        self.serialNumber.connect(self._bootup)
        self.bootupReceived = False
        self.simulated = False
        self.graphActive = False
          
#---------------------------------------------------------------------------------
# Public API :
#---------------------------------------------------------------------------------

    #@return [list(str), list(str)] : list of ports and baudrates
    def getSerialPortInfo(self):
        return self.com.getAvailablePorts(), self.com.getAvailableBaudrates()
    
    # Connect to the specified port at the specified baudrate
    # @param str port : like "COM1", ideally from getAvailablePorts()
    # @param int baudrate : ideally from getAvailableBaudrates()
    # @return bool : true if the connection succeed, false otherwise
    def connect(self, port, baudrate):
        res = self.com.connect(port, baudrate, self._frameReceived)
        #Workaround as first message is often lost
        self.getSerial()
        #Reset to get bootup logs
        self.resetCpu()
        return res 
        
    # go throught decorator
    def disconnect(self):
        print("disconnect")
        self.timer_telemetry.stop()
        self.com.disconnect()
        self.networkStatus.emit(False)
      
    def getUnexpectedDisconnect(self):
        return self.com.getUnexpectedDisconnect()
    
    # go throught decorator  
    def isConnected(self):
        return self.com.isConnected()
    
    #configure if the robot is accessed in a simulated or real environment
    def setSimulated(self, on):
        self.simulated = on
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
    def getComStatsLogs(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.getComStatsLogs.SetInParent()
        self._sendMsg(msg)
        
    @pyqtSlot(bool)
    def getTelemetry(self, graphActive):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.getTelemetry = graphActive
        self._sendMsg(msg)
    
    @pyqtSlot(Melody)
    def requestPlaySound(self, melody):
        msg = RemoteControl_pb2.RemoteControlRequest()
        melody.toMsg(msg.requestPlaySound)
        self._sendMsg(msg)
        
    @pyqtSlot()
    def getConfig(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.getConfig.SetInParent()
        self._sendMsg(msg)
        
    @pyqtSlot(RemoteControl_pb2.RemoteControlRequest)
    def setConfig(self, request):
        self._sendMsg(request)
        
    @pyqtSlot()
    def getSerial(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.getSerial.SetInParent()
        self._sendMsg(msg)
    
    @pyqtSlot()
    def resetCpu(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.reboot.SetInParent()
        self._sendMsg(msg)
        
    @pyqtSlot(int, int)
    def configureMatch(self, strategy, color, simulated=False):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.configureMatch.strategy = strategy
        msg.configureMatch.matchColor = color
        msg.configureMatch.simulated = self.simulated
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
        print(msg)

    @pyqtSlot(int, int, int, int)
    def setSpeedAcc(self, vMax, vMaxTurn, accMax, accMaxTurn):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.setSpeedAcc.vMax = vMax
        msg.setSpeedAcc.vMaxTurn = vMaxTurn
        msg.setSpeedAcc.accMax = accMax
        msg.setSpeedAcc.accMaxTurn = accMaxTurn
        self._sendMsg(msg)
        print(msg)
                
    @pyqtSlot(Pose2D, int)
    def gotoCap(self, pose, dir):
        msg = RemoteControl_pb2.RemoteControlRequest()
        pose.toPoseMsg(msg.requestGotoCap.target)
        msg.requestGotoCap.direction = dir
        self._sendMsg(msg)
        print(msg)
        
    @pyqtSlot(Point ,int)
    def goto(self, point, dir):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestGoto.target.x = point.x
        msg.requestGoto.target.y = point.y
        msg.requestGoto.direction = dir
        self._sendMsg(msg)
        print(msg)

    @pyqtSlot(float)
    def goForward(self, d):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestGoForward = d
        self._sendMsg(msg)
        print(msg)
        
    @pyqtSlot(float)
    def turnDelta(self, dh):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestTurnDelta= dh
        self._sendMsg(msg)
        print(msg)
        
    @pyqtSlot(float)
    def turnTo(self, h):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestTurnTo = h
        self._sendMsg(msg)
        print(msg)
        
    @pyqtSlot(Point)
    def faceTo(self, point):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestFaceTo.x = point.x
        msg.requestFaceTo.y = point.y
        self._sendMsg(msg)
        print(msg)
        
    @pyqtSlot(Pose2D, int)
    def graphTo(self, pose, dir):
        msg = RemoteControl_pb2.RemoteControlRequest()
        pose.toPoseMsg(msg.requestGraphTo.target)
        msg.requestGraphTo.direction = dir
        self._sendMsg(msg)
        print(msg)
              
    @pyqtSlot(int)
    def recalFace(self, wall):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.recalFaceOnBorder = wall
        self._sendMsg(msg)
        print(msg)

    @pyqtSlot(int)
    def recalRear(self, wall):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.recalRearOnBorder = wall
        self._sendMsg(msg)
        print(msg)        
                
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
        
    @pyqtSlot(bool)
    def requestBlockRobot(self, blocked):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestBlockRobot = blocked
        self._sendMsg(msg)
        
    @pyqtSlot()
    def requestMotionGraph(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestMotionGraph.SetInParent()
        self._sendMsg(msg)        
        
    @pyqtSlot()
    def requestTooLittleMsg(self):
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestTooLittleMsg.SetInParent()
        self._sendMsg(msg)
        
    @pyqtSlot(int)
    def requestLifterServo(self, angle): 
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestActuators.lifter = angle
        msg.requestActuators.hasLifter = True
        self._sendMsg(msg)
        
    @pyqtSlot(int)
    def requestLeftArmServo(self, angle): 
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestActuators.leftArm = angle
        msg.requestActuators.hasLeftArm = True
        self._sendMsg(msg)
        
    @pyqtSlot(int)
    def requestRightArmServo(self, angle): 
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestActuators.rightArm = angle
        msg.requestActuators.hasRightArm = True
        self._sendMsg(msg)
        
    @pyqtSlot(int)
    def requestLeftWheelServo(self, angle): 
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestActuators.leftWheel = angle
        msg.requestActuators.hasLeftWheel = True
        self._sendMsg(msg)
        
    @pyqtSlot(int)
    def requestRightWheelServo(self, angle): 
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestActuators.rightWheel = angle
        msg.requestActuators.hasRightWheel = True
        self._sendMsg(msg)
        
    @pyqtSlot(int)
    def requestFunnyActionServo(self, angle): 
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestActuators.funnyAction = angle
        msg.requestActuators.hasFunnyAction = True
        self._sendMsg(msg)
        
    @pyqtSlot(int)
    def requestRotatorServo(self, angle): 
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestActuators.rotator = angle
        msg.requestActuators.hasRotator = True
        self._sendMsg(msg)
        
    @pyqtSlot(int)
    def requestServo8Servo(self, angle): 
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestActuators.servo8 = angle
        msg.requestActuators.hasServo8 = True
        self._sendMsg(msg)
        
    @pyqtSlot(int)
    def requestRotatorTurn(self, turn): 
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestActuators.hasRotatorTurn = turn
        msg.requestActuators.rotatorTurn = True
        self._sendMsg(msg)
        
    @pyqtSlot(int)
    def requestActuatorsCmd(self, cmd): 
        msg = RemoteControl_pb2.RemoteControlRequest()
        msg.requestActuators.hasHighLevelCmd = True
        msg.requestActuators.highLevelCmd = cmd
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
                import traceback
                print("----- exception ---------")
                traceback.print_exc()
                print("------ stack ------------")
                traceback.print_stack()
                print("-------------------------")
                print("RemoteControl : Unkown message ")
                print(str(response))
        
            
    #serialize and send the message on communication link
    #do not use this function directly, use TELEOP API above
    def _sendMsg(self, msg):
        if not self.isConnected():
            print("RemoteControl : Message not send because not connected.")
            return
        
        assert isinstance(msg, RemoteControl_pb2.RemoteControlRequest), "RemoteControl_pb2._sendMsg expects to receive a RemoteControlRequest class"
        #---DEBUG--- print("RemoteControl request : " + str(msg))

        try:
            if self.com.sendMsg(msg.SerializeToString()):
                #--- DEBUG---print("Frame sent successfully.")
                pass
            else:
                print("Frame send error.")
        except:
            import traceback
            print("----- exception ---------")
            traceback.print_exc()
            print("------ stack ------------")
            traceback.print_stack()
            print("-------------------------")
            print("Failed to serialize msg.")

    #self.timer_telemetry Callback : send a telemetry data request  
    @pyqtSlot()
    def _telemetryTick(self):
        #print("tick")
        self.getTelemetry(self.graphActive)

    @pyqtSlot(RemoteControl_pb2.SerialNumber)        
    def _bootup(self, serial):
        self.networkStatus.emit(True)
        self.timer_telemetry.start(100)  
        pass
        