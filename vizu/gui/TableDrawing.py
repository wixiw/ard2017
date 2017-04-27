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
        self.robotPen = RobotPenWidget(self.p)
        self.robotTration = RobotTrationWidget(self.p)
        self.robot = None
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
        
        self.table.draw(self.parent().robotState.stratInfo)
        self.p.translate(T_WIDTH/2, T_HEIGHT/2)
        drawingPose = Pose2D()
        drawingPose.x = self.robotPose.x
        drawingPose.y = -self.robotPose.y
        drawingPose.h = normalizeAngle(math.pi + self.robotPose.h)
        if self.robot != None and self.parent().robotConfig != None:
            self.robot.draw(drawingPose, self.parent().robotConfig, self.parent().robotState.stratInfo)
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
                
    def draw(self, pose, cfg, stratInfo):
        self.p.save()
        self.p.setRenderHint(QPainter.Antialiasing)
        self.p.translate(pose.x, pose.y)
        self.p.rotate(180 - math.degrees(pose.h))
        self.drawCarriage(cfg)
        self.drawWheels(cfg)
        self.drawMarks(cfg)
        self.drawObjects(stratInfo)
        self.p.restore()

    def drawWheels(self, cfg):
        #draw wheels
        self.p.setBrush(Qt.green)
        self.p.drawRoundedRect(QRectF(-cfg.leftWheelDiameter/2, cfg.voie/2. - 5, cfg.leftWheelDiameter, 10), 3, 3)
        self.p.drawRoundedRect(QRectF(-cfg.rightWheelDiameter/2, -cfg.voie/2. - 5, cfg.rightWheelDiameter, 10), 3, 3)
        
    def drawMarks(self, cfg):
        pen = QPen(ardGray)
        pen.setWidth(1)
        pen.setCosmetic(True)
        pen.setDashPattern([2, 3])
        self.p.setPen(pen)
        
        self.p.drawLine(-cfg.xar*1.5, 0, cfg.xav*1.5, 0)
        self.p.drawLine(0, cfg.yside*1.5, 0, -cfg.yside*1.5)

class RobotPenWidget(RobotWidget):
    def drawCarriage(self, cfg):
        self.p.setPen(markPen)
        self.p.setBrush(trafficWhite)
        carriage = QPainterPath()
        mouthX = 35
        mouthY = 35
        #print("xar = " + str(cfg.xar))
        carriage.moveTo(-cfg.xar, cfg.yside)
        carriage.lineTo(cfg.xav, cfg.yside)
        carriage.lineTo(cfg.xav, mouthX)
        carriage.lineTo( mouthY, mouthX)
        carriage.lineTo( mouthY,-mouthX)
        carriage.lineTo(cfg.xav,-mouthX)
        carriage.lineTo(cfg.xav,-cfg.yside)
        carriage.lineTo(-cfg.xar,-cfg.yside)
        carriage.closeSubpath()
        self.p.drawPath(carriage)
        
        #draw front lines
        pen = QPen(ardGray)
        pen.setWidth(1)
        pen.setCosmetic(True)
        pen.setDashPattern([2, 3])
        self.p.setPen(pen)
        carriage = QPainterPath()
        carriage.moveTo(cfg.xav, cfg.yside)
        carriage.lineTo(cfg.xav, -cfg.yside)
        carriage.moveTo(cfg.xavExtended, cfg.yside)
        carriage.lineTo(cfg.xavExtended, -cfg.yside)
        self.p.drawPath(carriage)
        self.p.setPen(markPen)
        
        #draw actuators
        self.p.setBrush(jetBlack)
        drawCircle(self.p, 185,  60, 22)
        drawCircle(self.p, 185, -60, 22)
        self.p.setBrush(Qt.gray)
        drawCircle(self.p, 185,  60, 15)
        drawCircle(self.p, 185, -60, 15)
        arm = QPainterPath()
        arm.moveTo(cfg.xav + 40, 50)
        arm.lineTo(cfg.xav - 30, 50)
        arm.lineTo(cfg.xav - 30, 70)
        arm.lineTo(cfg.xav + 40, 70)
        arm.closeSubpath()
        self.p.drawPath(arm)
        arm = QPainterPath()
        arm.moveTo(cfg.xav + 40, -50)
        arm.lineTo(cfg.xav - 30, -50)
        arm.lineTo(cfg.xav - 30, -70)
        arm.lineTo(cfg.xav + 40, -70)
        arm.closeSubpath()
        self.p.drawPath(arm)
        
    def drawObjects(self, stratInfo):
        if stratInfo.robotCylinderStockNb:
            self.p.setPen(markPen)
            drawHorizontalCylinder(self.p, stratInfo.matchColor, 85, 0, 0)
                        
        if stratInfo.robotCylinderStockNb == 6:
            drawVerticalCylinder(self.p, stratInfo.matchColor, 180, 0)
        
        
class RobotTrationWidget(RobotWidget):
    def drawCarriage(self, cfg):
        self.p.setPen(markPen)
        self.p.setBrush(Qt.gray)
        carriage = QPainterPath()
        mouthX = 35
        mouthY = 35
        #print("xar = " + str(cfg.xar))
        carriage.moveTo(-cfg.xar, cfg.yside)
        carriage.lineTo(cfg.xav, cfg.yside)
        carriage.lineTo(cfg.xav, mouthX)
        carriage.lineTo( mouthY, mouthX)
        carriage.lineTo( mouthY,-mouthX)
        carriage.lineTo(cfg.xav,-mouthX)
        carriage.lineTo(cfg.xav,-cfg.yside)
        carriage.lineTo(-cfg.xar,-cfg.yside)
        carriage.closeSubpath()
        self.p.drawPath(carriage)
        
        #draw front lines
        pen = QPen(ardGray)
        pen.setWidth(1)
        pen.setCosmetic(True)
        pen.setDashPattern([2, 3])
        self.p.setPen(pen)
        carriage = QPainterPath()
        carriage.moveTo(cfg.xav, cfg.yside)
        carriage.lineTo(cfg.xav, -cfg.yside)
        carriage.moveTo(cfg.xavExtended, cfg.yside)
        carriage.lineTo(cfg.xavExtended, -cfg.yside)
        self.p.drawPath(carriage)
        self.p.setPen(markPen)
        
        #draw actuators
        self.p.setBrush(jetBlack)
        drawCircle(self.p, 185,  60, 22)
        drawCircle(self.p, 185, -60, 22)
        self.p.setBrush(Qt.gray)
        drawCircle(self.p, 185,  60, 15)
        drawCircle(self.p, 185, -60, 15)
        arm = QPainterPath()
        arm.moveTo(cfg.xav + 40, 50)
        arm.lineTo(cfg.xav - 30, 50)
        arm.lineTo(cfg.xav - 30, 70)
        arm.lineTo(cfg.xav + 40, 70)
        arm.closeSubpath()
        self.p.drawPath(arm)
        arm = QPainterPath()
        arm.moveTo(cfg.xav + 40, -50)
        arm.lineTo(cfg.xav - 30, -50)
        arm.lineTo(cfg.xav - 30, -70)
        arm.lineTo(cfg.xav + 40, -70)
        arm.closeSubpath()
        self.p.drawPath(arm)
        
    def drawObjects(self, stratInfo):
        if stratInfo.robotCylinderStockNb:
            self.p.setPen(markPen)
            drawHorizontalCylinder(self.p, stratInfo.matchColor, 85, 0, 0)
                        
        if stratInfo.robotCylinderStockNb == 6:
            drawVerticalCylinder(self.p, stratInfo.matchColor, 180, 0)


class TableWidget():
    
    def __init__(self, painter):
        self.p = painter
                
    def draw(self, stratInfo):
        self.drawBackground()
        self.drawStartArea()
        self.drawCraters()
        self.drawContainers(stratInfo)
        self.drawBorders()
        self.drawDispensers(stratInfo)
        self.drawMarks()
        self.drawCylinders(stratInfo)
        self.drawPooedObjects(stratInfo)
        
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
        
    def drawPooedObjects(self, stratInfo):
       
        #Draw Opp straight border area pooed objects
        for i in range (0, stratInfo.containerBorderOppNb):
            if stratInfo.matchColor == Types_pb2.PREF:
                drawHorizontalCylinder(self.p, stratInfo.matchColor, 40, 700 + 25 + 100/2. + i*100, 90)
            else:
                drawHorizontalCylinder(self.p, stratInfo.matchColor, T_WIDTH - 40, 700 + 25 + 100/2. + i*100, 90)
        
        #Draw Own straight border area pooed objects    
        for i in range (0, stratInfo.containerBorderNb):
            if stratInfo.matchColor == Types_pb2.PREF:
                drawHorizontalCylinder(self.p, stratInfo.matchColor, T_WIDTH - 40, 700 + 25 + 100/2. + i*100, 90)
            else:
                drawHorizontalCylinder(self.p, stratInfo.matchColor, 40, 700 + 25 + 100/2. + i*100, 90)
                        
        #Draw midle center pooed objects
        for i in range (0, stratInfo.containerMidleCenterNb):
            drawHorizontalCylinder(self.p, stratInfo.matchColor, T_WIDTH/2., 1250 + i*100, 90)
            
        #Draw own midle side pooed objects
        self.p.save()
        self.p.translate(T_WIDTH/2., T_HEIGHT)
            #Yellow side
        if stratInfo.matchColor == Types_pb2.PREF:
            self.p.rotate(-45)
            #Blue side
        else:
            self.p.rotate(-135)
        for i in range (0, stratInfo.containerMidleOwnNb):
            drawHorizontalCylinder(self.p, stratInfo.matchColor, 750 - i*100, 0, 0)
        self.p.restore()
        
        #Draw opp midle side pooed objects
        self.p.save()
        self.p.translate(T_WIDTH/2., T_HEIGHT)
            #Yellow side
        if stratInfo.matchColor == Types_pb2.PREF:
            self.p.rotate(-135)
            #Blue side
        else:
            self.p.rotate(-45)
        for i in range (0, stratInfo.containerMidleOppNb):
            drawHorizontalCylinder(self.p, stratInfo.matchColor, 750 - i*100, 0, 0)
        self.p.restore()

        #Draw start area pooed objects
        for i in range (0, stratInfo.containerStartNb):
            #Yellow side
            if stratInfo.matchColor == Types_pb2.PREF:
                if i < 4 :
                    drawHorizontalCylinder(self.p, stratInfo.matchColor, 2000 + i*75, 100, 90)
                else:
                    drawHorizontalCylinder(self.p, stratInfo.matchColor, 2000 + (i-4)*75, 250, 90)
            #Blue side
            else:
                if i < 4 :
                    drawHorizontalCylinder(self.p, stratInfo.matchColor, 1000 - i*75, 100, 90)
                else:
                    drawHorizontalCylinder(self.p, stratInfo.matchColor, 1000 - (i-4)*75, 250, 90) 

    def _drawOneCrater(self, x, y):
        self.p.setPen(markPen)
        self.p.save()
        self.p.setBrush(trafficWhite)
        self.p.translate(x, y)
        drawCircle(self.p, 0,0, 251/2.)
        self.p.setBrush(ardBackground)
        drawCircle(self.p, 0,0, 191/2.)
        self.p.restore()
    
    def drawContainers(self, stratInfo):
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
        
    def drawDispensers(self, stratInfo):
        #Bicolor lateral dispenser on Blue side
        self.p.save()
        self.p.setPen(markPen)
        self.p.setBrush(peebleGrey)
        self.p.translate(40, 1350)
        drawCircle(self.p, 0,0,40)
        self.p.restore()
        if stratInfo.dispenserBicolorNb != 0 and stratInfo.matchColor == Types_pb2.SYM:
            for i in range(0, stratInfo.dispenserBicolorNb):
                drawVBicolorCylinder(self.p, 40 - 20*i, 1350, 135)
        if stratInfo.dispenserOppNb != 0 and stratInfo.matchColor == Types_pb2.PREF:
            for i in range(0, stratInfo.dispenserOppNb):
                drawVBicolorCylinder(self.p, 40 - 20*i, 1350, 135)
            
        #Blue Top dispenser
        self.p.save()
        self.p.setPen(markPen)
        self.p.setBrush(peebleGrey)
        self.p.translate(1150, 40)
        drawCircle(self.p, 0,0,40)
        self.p.restore()
        if stratInfo.dispenserMonocolorNb != 0 and stratInfo.matchColor == Types_pb2.SYM:
            for i in range(0, stratInfo.dispenserMonocolorNb):
                drawVerticalCylinder(self.p, Types_pb2.SYM, 1150, 40 - 20*i)     
        if stratInfo.matchColor == Types_pb2.PREF:
            for i in range(0, 4):
                drawVerticalCylinder(self.p, Types_pb2.SYM, 1150, 40 - 20*i) 
        
        #Yellow Top dispenser
        self.p.save()
        self.p.translate(1850, 40)
        drawCircle(self.p, 0,0,40)
        self.p.restore()
        if stratInfo.dispenserMonocolorNb != 0 and stratInfo.matchColor == Types_pb2.PREF:
            for i in range(0, stratInfo.dispenserMonocolorNb):
                drawVerticalCylinder(self.p, Types_pb2.PREF, 1850, 40 - 20*i)  
        if stratInfo.matchColor == Types_pb2.SYM:
            for i in range(0, 4):
                drawVerticalCylinder(self.p, Types_pb2.PREF, 1850, 40 - 20*i)  

        #Bicolor lateral dispenser on Yellow side
        self.p.save()
        self.p.translate(T_WIDTH - 40, 1350)
        drawCircle(self.p, 0,0,40)
        self.p.restore()
        if stratInfo.dispenserBicolorNb != 0 and stratInfo.matchColor == Types_pb2.PREF:
            for i in range(0, stratInfo.dispenserBicolorNb):
                drawVBicolorCylinder(self.p, T_WIDTH - 40 + 20*i, 1350, -45)
        if stratInfo.dispenserOppNb != 0 and stratInfo.matchColor == Types_pb2.SYM:
            for i in range(0, stratInfo.dispenserOppNb):
                drawVBicolorCylinder(self.p, T_WIDTH - 40 + 20*i, 1350, -45)
        
        
    def drawCylinders(self, stratInfo):
    #
    #Yellow area
    #
        #Cylinder close to start area
        if stratInfo.cylinderStart and stratInfo.matchColor == Types_pb2.PREF \
            or stratInfo.cylinderOppStart and stratInfo.matchColor == Types_pb2.SYM:
            drawVBicolorCylinder(self.p, 2000, 600, -45) 
                        
        #Cylinder between straight container and little border corner.
        if stratInfo.cylinderCorner and stratInfo.matchColor == Types_pb2.PREF \
            or stratInfo.matchColor == Types_pb2.SYM:
            drawVerticalCylinder(self.p, Types_pb2.PREF, 2800,600)  
            
        #Cylinder in the "middle" of the half-table
        if stratInfo.cylinderCenter and stratInfo.matchColor == Types_pb2.PREF \
            or stratInfo.cylinderOppCenter and stratInfo.matchColor == Types_pb2.SYM:
            drawVBicolorCylinder(self.p, 2500, 1100, -45) 
            
        #Cylinder blocking central container
        if stratInfo.cylinderContainer and stratInfo.matchColor == Types_pb2.PREF \
            or stratInfo.cylinderOppContainer and stratInfo.matchColor == Types_pb2.SYM:
            drawVBicolorCylinder(self.p, 2100, 1400, -45) 
            
        #Cylinder between bottom craters
        if stratInfo.cylinderCrater and stratInfo.matchColor == Types_pb2.PREF \
            or stratInfo.matchColor == Types_pb2.SYM:
            drawVerticalCylinder(self.p, Types_pb2.PREF, 2200,1850) 
            
            
    #
    #Blue area
    #
        #Cylinder close to start area
        if stratInfo.cylinderStart and stratInfo.matchColor == Types_pb2.SYM \
            or stratInfo.cylinderOppStart and stratInfo.matchColor == Types_pb2.PREF:
            drawVBicolorCylinder(self.p, 1000, 600, 135) 
                        
        #Cylinder between straight container and little border corner.
        if stratInfo.cylinderCorner and stratInfo.matchColor == Types_pb2.SYM \
            or stratInfo.matchColor == Types_pb2.PREF:
            drawVerticalCylinder(self.p, Types_pb2.SYM, 200, 600)
            
        #Cylinder in the "middle" of the half-table
        if stratInfo.cylinderCenter and stratInfo.matchColor == Types_pb2.SYM \
            or stratInfo.cylinderOppCenter and stratInfo.matchColor == Types_pb2.PREF:
            drawVBicolorCylinder(self.p, 500, 1100, 135)
            
        #Cylinder blocking central container
        if stratInfo.cylinderContainer and stratInfo.matchColor == Types_pb2.SYM \
            or stratInfo.cylinderOppContainer and stratInfo.matchColor == Types_pb2.PREF:
            drawVBicolorCylinder(self.p, 900, 1400, 135)
            
        #Cylinder between bottom craters
        if stratInfo.cylinderCrater and stratInfo.matchColor == Types_pb2.SYM \
            or stratInfo.matchColor == Types_pb2.PREF:
            drawVerticalCylinder(self.p, Types_pb2.SYM, 800, 1850)
        
    
def drawCircle(painter, x, y, radius):
    painter.save()
    painter.setRenderHint(QPainter.Antialiasing)
    painter.drawEllipse(QRectF(x - radius, y - radius, 2*radius, 2*radius))
    painter.restore()

def drawVerticalCylinder(painter, color, x, y):
    painter.save()
    painter.setPen(markPen)
    if color == Types_pb2.PREF:
        painter.setBrush(trafficYellow)
    else:
        painter.setBrush(skyBlue)
    painter.translate(x,y)
    drawCircle(painter, 0,0, 63/2.)
    painter.restore()
    
def drawVBicolorCylinder(painter, x, y, angle):
    painter.save()
    painter.setRenderHint(QPainter.Antialiasing)
    painter.setPen(markPen)
    painter.translate(x,y)
    painter.rotate(angle)
    
    #white sections
    painter.setBrush(trafficWhite)
    path = QPainterPath()
    path.moveTo(0,0)
    path.lineTo(0,63/2.)
    path.arcTo(QRectF(-63/2., -63/2., 63, 63), -90, 90)
    path.lineTo(-63/2.,0)
    path.arcTo(QRectF(-63/2., -63/2., 63, 63), 180, -90)
    path.closeSubpath()
    painter.drawPath(path)
    
    #yellow section
    painter.setBrush(trafficYellow)
    path = QPainterPath()
    path.moveTo(0,0)
    path.lineTo(63/2.,0)
    path.arcTo(QRectF(-63/2., -63/2., 63, 63), 0, 90)
    path.closeSubpath()
    painter.drawPath(path)
    
    #blue section
    painter.setBrush(skyBlue)
    path = QPainterPath()
    path.moveTo(0,0)
    path.lineTo(-63/2.,0)
    path.arcTo(QRectF(-63/2., -63/2., 63, 63), 180, 90)
    path.closeSubpath()
    painter.drawPath(path)
    painter.restore()
        
def drawHorizontalCylinder(painter, color, x, y, angle):     
    painter.save()
    
    painter.translate(x,y)
    painter.rotate(angle)
    
    painter.setPen(markPen)
    if color == Types_pb2.PREF:
        painter.setBrush(trafficYellow)
    else:
        painter.setBrush(skyBlue)
    
    cylinder = QPainterPath()
    cylinder.moveTo(-50, 0)
    cylinder.lineTo(-50, -63/2.)
    cylinder.lineTo(50, -63/2.)
    cylinder.lineTo(50, 63/2.)
    cylinder.lineTo(-50, 63/2.)
    cylinder.closeSubpath()
    painter.drawPath(cylinder)
    
    painter.restore()
    