#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from core import *
import math

#spare area around the table
T_SPARE  =  100.

#playing field size
T_WIDTH  = 3000.
T_HEIGHT = 2000.

#RAL encoding 
peebleGrey      = QColor(0xBDBAAB) #RAL7032
trafficWhite    = QColor(0xFCFFFF) #RAL9016
jetBlack        = QColor(0x03050A) #RAL9005
trafficYellow   = QColor(0xFCBD1F) #RAL1023
skyBlue         = QColor(0x1761AB) #RAL5015
ardGray         = QColor(66,66,66)
ardBackground   = QColor(0xADAFAF)

markPen = QPen(ardGray)
markPen.setWidth(1)
markPen.setCosmetic(True)

#
# This class is a pre-built widget which is designed to display 
# strategy state and a 2D table view with a moving robot.
#
class TabStrat(QWidget):
    
    #@param robot : the prowy providing telemetry data
    def __init__(self, parent = None):
        super().__init__(parent)
        
        self.overview = TableOverview(self)
        
        #build info tab
        self.label = dict()
        self.buildGeneralInfo()
        self.buildPosInfo()
        self.buildMotionInfo()
        self.buildSensorsInfo()
        self.layoutInfo = QVBoxLayout()
        self.layoutInfo.addWidget(self.box_general)
        self.layoutInfo.addWidget(self.box_pos)
        self.layoutInfo.addWidget(self.box_motion)
        self.layoutInfo.addWidget(self.box_sensors)
        self.layoutInfo.addSpacerItem(QSpacerItem(175,0,QSizePolicy.Minimum,QSizePolicy.Expanding))
        
        self.layout = QHBoxLayout(self)
        self.layout.addLayout(self.layoutInfo)
        self.layout.addWidget(self.overview)
        
        self.robotState = RemoteControl_pb2.Telemetry()
        
    def buildGeneralInfo(self):
        self.box_general = QGroupBox("General")
        self.label["bootTime"] = QLabel("0")
        self.label["bootTime"].setAlignment(Qt.AlignRight)
        box_layout = QFormLayout()
        box_layout.addRow("boot time (s): ", self.label["bootTime"])
        self.box_general.setLayout(box_layout)
            
    def buildPosInfo(self):
        self.box_pos = QGroupBox("Position")
        self.label["x"] = QLabel("0")
        self.label["y"] = QLabel("0")
        self.label["h"] = QLabel("0")
        self.label["x"].setAlignment(Qt.AlignRight)
        self.label["y"].setAlignment(Qt.AlignRight)
        self.label["h"].setAlignment(Qt.AlignRight)
        box_layout = QFormLayout()
        box_layout.addRow("x (mm) : ", self.label["x"])
        box_layout.addRow("y (mm) : ", self.label["y"])
        box_layout.addRow("h (°)  : ", self.label["h"])
        self.box_pos.setLayout(box_layout)
        
    def buildMotionInfo(self):
        self.box_motion = QGroupBox("Motion")
        self.label["state"] = QLabel("?")
        self.label["order"] = QLabel("?")
        self.label["state"].setAlignment(Qt.AlignRight)
        self.label["order"].setAlignment(Qt.AlignRight)
        box_layout = QFormLayout()
        box_layout.addRow("state : ", self.label["state"])
        box_layout.addRow("order : ", self.label["order"])
        self.box_motion.setLayout(box_layout)
        
    def buildSensorsInfo(self):
        self.box_sensors = QGroupBox("Sensors")
        self.label["RGBL"] = QLabel("?")
        self.label["objColor"] = QLabel("?")
        self.label["RGBL"].setAlignment(Qt.AlignRight)
        self.label["objColor"].setAlignment(Qt.AlignRight)
        box_layout = QFormLayout()
        box_layout.addRow("RGB : ", self.label["RGBL"])
        box_layout.addRow("Color : ", self.label["objColor"])
        self.box_sensors.setLayout(box_layout)
       
    #@return Pose2D : the last received telemetry position
    def getRobotPosition(self):
        return Pose2D.fromPoseMsg(self.robotState.nav.pos)
    
    def getMotionStateStr(self):
        return Types_pb2.eNavState.Name(self.robotState.nav.state)

    def getMotionOrderStr(self):
        return Types_pb2.eNavOrder.Name(self.robotState.nav.order)
    
    def getObjectColorStr(self):
        return Types_pb2.eObjectColor.Name(self.robotState.actuators.colorSensor.color)
    
    #telemetry reply data callback
    @pyqtSlot(RemoteControl_pb2.Telemetry)     
    def _telemetryDataCb(self, msg):
        if self.isVisible():
            #--- DEBUG --- print("Telemetry received.")
            #--- DEBUG --- print(str(msg))
            self.robotState = msg
            pose = self.getRobotPosition()
            color = self.robotState.actuators.colorSensor
            colorStr = self.getObjectColorStr()
            self.overview.robotPose = pose
            self.label["bootTime"].setText("%0.1f" % (self.robotState.date/1000.))
            self.label["x"].setText("%0.0f" %pose.x)
            self.label["y"].setText("%0.0f" %pose.y)
            self.label["h"].setText("%0.0f" % math.degrees(pose.h))
            self.label["state"].setText(self.getMotionStateStr())
            self.label["order"].setText(self.getMotionOrderStr())
            self.label["RGBL"].setText("(%d,%d,%d,%d)" % (color.r, color.g, color.b, color.l))
            self.label["RGBL"].setStyleSheet("QLabel { color : white; background-color: rgba(%d,%d,%d,%d); }" % (color.r, color.g, color.b, 255))
            self.label["objColor"].setText(colorStr)
            self.update()

#
# This class is an overview of the table with robot displayed at current position
#
class TableOverview(QWidget):
    
    #@param robot : the prowy providing telemetry data
    def __init__(self, parent = None):
        super().__init__(parent)
        self.layout = QHBoxLayout(self)
        self.resize(600,400)
        self.p = QPainter()
        self.table = TableWidget(self.p)
        self.robot = RobotWidget(self.p)
        self.view = QRect( - T_SPARE,
                   - T_SPARE,
                    T_WIDTH     +  2.*T_SPARE,
                    T_HEIGHT    +  2.*T_SPARE)
        self.robotPose = Pose2D()
        sizePolicy = QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        self.setSizePolicy(sizePolicy)
        
    def paintEvent(self, event):
        self.p.begin(self)
        
        aspectH = min( self.p.device().height(), self.p.device().width() / 1.5)
        aspectW = min( self.p.device().width(), self.p.device().height() * 1.5)
        
        #---DEBUG---  print(str(self.p.device().width()) + " " + str(self.p.device().height()))
        self.p.setWindow(self.view)
        self.p.setViewport(0., 0., aspectW, aspectH)
        #---DEBUG--- print(self.view)
        
        self.table.draw()
        self.p.translate(T_WIDTH/2, T_HEIGHT/2)
        drawingPose = Pose2D()
        drawingPose.x = self.robotPose.x
        drawingPose.y = -self.robotPose.y
        drawingPose.h = normalizeAngle(math.pi + self.robotPose.h)
        self.robot.draw(drawingPose)
        self.p.end()
    
    def mousePressEvent(self, event):
        p = event.pos()
        #trans, ok = self.p.transform().inverted()
        trans = self.p.deviceTransform()
        p2 = trans.map(p)
        qDebug(str(p.x()) + " " + str(p.y()) + " => " + (str(p2.x()) + " " + str(p2.y()))) 
        return QWidget.mousePressEvent(self, event)

class RobotWidget():
    
    def __init__(self, painter):
        self.p = painter
                
    def draw(self, pose):
        self.p.save()
        self.p.setRenderHint(QPainter.Antialiasing)
        self.p.translate(pose.x, pose.y)
        self.p.rotate(180 - math.degrees(pose.h))
        self.drawCarriage()
        self.drawWheels()
        self.drawMarks()
        self.p.restore()

    def drawCarriage(self):
        self.p.setPen(markPen)
        self.p.setBrush(Qt.gray)
        carriage = QPainterPath()
        carriage.moveTo(-40, 75)
        carriage.lineTo(130, 75)
        carriage.lineTo(130, 45)
        carriage.lineTo( 40, 45)
        carriage.lineTo( 40,-45)
        carriage.lineTo(130,-45)
        carriage.lineTo(130,-75)
        carriage.lineTo(-40,-75)
        carriage.closeSubpath()
        self.p.drawPath(carriage)
        pass
    
    def drawWheels(self):
        #draw wheels
        self.p.setBrush(Qt.green)
        self.p.drawRoundedRect(QRectF(-35, 55, 60, 10), 3, 3)
        self.p.drawRoundedRect(QRectF(-35, -65, 60, 10), 3, 3)
        
    def drawMarks(self):
        pen = QPen(ardGray)
        pen.setWidth(1)
        pen.setCosmetic(True)
        pen.setDashPattern([2, 3])
        self.p.setPen(pen)
        
        self.p.drawLine(-60, 0, 150, 0)
        self.p.drawLine(0, 90, 0, -90)

class TableWidget():
    
    def __init__(self, painter):
        self.p = painter
                
    def draw(self):
        self.drawBackground()
        self.drawStartArea()
        self.drawCraters()
        self.drawContainers()
        self.drawBorders()
        self.drawDispensers()
        self.drawMarks()
        
    def drawBorders(self):
        self.p.setPen(markPen)
        self.p.setBrush(peebleGrey)
        
        path = QPainterPath()
        path.lineTo(T_WIDTH, 0)
        path.lineTo(T_WIDTH, T_HEIGHT)
        path.lineTo(0, T_HEIGHT)
        path.lineTo(0, 0)
        path.moveTo(-22, -22)
        path.lineTo(T_WIDTH+22, -22)
        path.lineTo(T_WIDTH+22, T_HEIGHT+22)
        path.lineTo(-22, T_HEIGHT+22)
        path.lineTo(-22, -22)
        self.p.drawPath(path)
        
    def drawBackground(self):
        self.p.setPen(markPen)
        self.p.setBrush(ardBackground)
        self.p.drawRect(0, 0, T_WIDTH, T_HEIGHT)
        
    def drawStartArea(self):
        #BLUE AREA
            #start area
        self.p.setBrush(skyBlue)
        self.p.setPen(QPen())
        self.p.drawRect(0, 0, 1070, 360)
            #bascule
        self.p.setBrush(peebleGrey)
        self.p.setPen(markPen)
        self.p.drawRect(360, 0, 350, 360)
            #little border
        self.p.setBrush(skyBlue)
        self.p.setPen(markPen)
        self.p.drawRect(0, 360, 710, 22)
        
        #YELLOW AREA
            #start area
        self.p.setBrush(trafficYellow)
        self.p.setPen(QPen())
        self.p.drawRect(1930, 0, 1070, 360)   
            #bascule
        self.p.setBrush(peebleGrey)
        self.p.setPen(markPen)
        self.p.drawRect(2290, 0, 350, 360)
            #little border 
        self.p.setBrush(trafficYellow)
        self.p.setPen(markPen)   
        self.p.drawRect(2290, 360, 710, 22) 
    
    def drawMarks(self):
        pen = QPen(ardGray)
        pen.setWidth(1)
        pen.setCosmetic(True)
        pen.setDashPattern([5, 10])
        
        #Table axis
        self.p.setPen(pen)
        self.p.drawLine(T_WIDTH/2, T_HEIGHT/2, T_WIDTH, T_HEIGHT/2)
        self.p.drawLine(T_WIDTH/2, T_HEIGHT/2, 0, T_HEIGHT/2)
        self.p.drawLine(T_WIDTH/2, T_HEIGHT/2, T_WIDTH/2, 0)
        self.p.drawLine(T_WIDTH/2, T_HEIGHT/2, T_WIDTH/2, T_HEIGHT)
        
        #Central container circle
        self.p.save()
        self.p.setRenderHint(QPainter.Antialiasing)
        self.p.drawArc(QRectF(T_WIDTH/2-800, T_HEIGHT-800, 2*800, 2*800), 180*16, -180*16) 
        self.p.restore()
         
        
    def drawCraters(self):
        self.p.save()
        self.p.setPen(markPen)
        self.p.setBrush(trafficWhite)
        self.p.setRenderHint(QPainter.Antialiasing)
        
        #LeftCrater
        leftCrater = QPainterPath()
        leftCrater.moveTo(0, T_HEIGHT - 510)
        leftCrater.arcTo(QRectF(-510, T_HEIGHT - 510, 2*510, 2*510), 90, -90)
        leftCrater.lineTo(510 + 30, T_HEIGHT)
        leftCrater.arcTo(QRectF(-510-30, T_HEIGHT -510-30, 2*(510+30), 2*(510+30)), 0, 90)
        leftCrater.closeSubpath()
        self.p.drawPath(leftCrater)
        
        #RightCrater
        rightCrater = QPainterPath()
        rightCrater.moveTo(T_WIDTH, T_HEIGHT - 510)
        rightCrater.arcTo(QRectF(T_WIDTH-510, T_HEIGHT - 510, 2*510, 2*510), 90, 90)
        rightCrater.lineTo(T_WIDTH - 510 - 30, T_HEIGHT)
        rightCrater.arcTo(QRectF(T_WIDTH-510-30, T_HEIGHT -510-30, 2*(510+30), 2*(510+30)), 180, -90)
        rightCrater.closeSubpath()
        self.p.drawPath(rightCrater)
        self.p.restore()
        
        #Little ones
        self._drawOneCrater( 650,  540)
        self._drawOneCrater(2350,  540)
        self._drawOneCrater(1070, 1870)
        self._drawOneCrater(1930, 1870)

    def _drawOneCrater(self, x, y):
        self.p.setPen(markPen)
        self.p.save()
        self.p.setBrush(trafficWhite)
        self.p.translate(x, y)
        self.drawCircle(0,0, 170/2+30)
        self.p.setBrush(ardBackground)
        self.drawCircle(0,0, 170/2)
        self.p.restore()
    
    def drawContainers(self):
        self.p.setPen(markPen)
        self.p.setBrush(trafficWhite)
        
        #left straight container
        self.p.drawRect(0, 700-22, 80, 22)
        self.p.drawRect(0, 1150, 80, 22) 
        self.p.drawRect(80, 700, 28, 450)    
        
        #right straight container
        self.p.drawRect(T_WIDTH-80, 700-22, 80, 22)
        self.p.drawRect(T_WIDTH-80, 1150, 80, 22) 
        self.p.drawRect(T_WIDTH-80-28, 700, 28, 450)    
        
        #middle central container
        self.p.save()
        self.p.setRenderHint(QPainter.Antialiasing)
        self.p.translate(T_WIDTH/2, T_HEIGHT)
        self.p.rotate(45)
        for i in [1, 2, 3]:
            self.p.drawRect(-80/2-28, -800, 28, 620)
            self.p.drawRect(80/2, -800, 28, 620)
            self.p.rotate(-45)
        self.p.restore()
        
        #central circle
        self.p.save()
        self.p.setRenderHint(QPainter.Antialiasing)
        self.p.drawPie(QRectF(T_WIDTH/2-200, T_HEIGHT-200, 2*200, 2*200), 180*16, -180*16) 
        self.p.restore()
        
    def drawDispensers(self):
        self.p.save()
        self.p.translate(80/2, 1350)
        self.drawCircle(0,0,40)
        self.p.restore()
        
        self.p.save()
        self.p.translate(T_WIDTH - 80/2, 1350)
        self.drawCircle(0,0,40)
        self.p.restore()
        
        self.p.save()
        self.p.translate(1150, 80/2)
        self.drawCircle(0,0,40)
        self.p.restore()
        
        self.p.save()
        self.p.translate(1850, 80/2)
        self.drawCircle(0,0,40)
        self.p.restore()
        
    def drawCircle(self, x, y, radius):
        self.p.save()
        self.p.setRenderHint(QPainter.Antialiasing)
        self.p.drawEllipse(QRectF(x - radius, y - radius, 2*radius, 2*radius))
        self.p.restore()
        
    
    
    