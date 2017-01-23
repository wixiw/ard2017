#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from core import *
import copy

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
    
    def __init__(self, parent = None):
        super().__init__(parent)
        self.table = TableWidget(self)
        layoutFilters = QHBoxLayout()
        layoutFilters.addWidget(self.table)

class TableWidget(QWidget):
    
    def __init__(self, parent = None):
        super().__init__(parent)
        self.p = QPainter()
        self.resize(600,400)
        self.view = QRect( - T_SPARE,
                           - T_SPARE,
                            T_WIDTH     +  2.*T_SPARE,
                            T_HEIGHT    +  2.*T_SPARE)
        
    def paintEvent(self, event):
        self.p.begin(self)
        print(str(self.p.device().width()) + " " + str(self.p.device().height()))
        self.p.setWindow(self.view)
        self.p.setViewport(0., 0., self.p.device().width(), self.p.device().height())
        print(self.view)
        
        self.drawEmptyTable()
        self.p.end()
        
    def mousePressEvent(self, event):
        p = event.pos()
        #trans, ok = self.p.transform().inverted()
        trans = self.p.deviceTransform()
        p2 = trans.map(p)
        qDebug(str(p.x()) + " " + str(p.y()) + " => " + (str(p2.x()) + " " + str(p2.y()))) 
        return QWidget.mousePressEvent(self, event)

    def drawEmptyTable(self):
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
    
if __name__ == '__main__':
    #expand path to find modules :
    import sys
    sys.path.append("com")
    sys.path.append("core")
    sys.path.append("../proto")
    
    app = QApplication(sys.argv)
    #widget = TabRobot()
    widget = TableWidget()
    widget.show()
    sys.exit(app.exec_())
    
    
    
    