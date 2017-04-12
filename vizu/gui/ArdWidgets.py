#!/usr/bin/python3
# -*- coding: utf-8 -*-

import signal
import math
from ArdMath import *
from Music import *
from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from generated import *

#
# This widget is a restricted text field usable to select an integer value in a range
#
class IntegerInput(QLineEdit):
    def __init__(self, parent, minValue, maxValue):
        super().__init__(parent)
        # count the maximal number of digits
        maxIntLength = max(len(str(abs(minValue))),len(str(abs(maxValue))))
        #display a default text with the number of digits
        defaultText = "-"
        for i in range(0,maxIntLength):
            defaultText += "0"
        self.setPlaceholderText(defaultText)
        #configure widget size
        self.setMaxLength(1 + maxIntLength)
        self.setMinimumWidth(20 + QFontMetrics(self.font()).width(defaultText)) # +20: take padding into account
        self.setSizePolicy(QSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum))
        self.setAlignment(Qt.AlignHCenter)
        # add validator on acceptable range and display it in tooltip
        self.setValidator(QIntValidator(minValue, maxValue));
        self.setToolTip("range : [" + str(minValue) + " ; " + str(maxValue) + "]")
        
    #@return int : return the numeric value of the field (or 0 if not set)
    def getValue(self):
        if self.text() != "":
            return int(self.text())
        else:
            return 0
        
    def setValue(self, value):
        self.setText(str(value)) 

#
# This widget is a restricted text field usable to select an float value in a range 
# with a restricted number of decimals
#     
class FloatInput(QLineEdit):
    def __init__(self, parent, minValue, maxValue, nbDecimals):
        super().__init__(parent)
        # count the maximal number of digitse
        maxIntLength = max(len(str(math.floor(abs(minValue)))), len(str(math.floor(abs(maxValue)))))
        #display a default text with the number of digits
        defaultText = "-"
        for i in range(0,maxIntLength):
            defaultText += "0"
        defaultText+="."
        for i in range(0,nbDecimals):
            defaultText += "0"
        self.setPlaceholderText(defaultText)
        #configure widget size
        self.setMaxLength(1 + maxIntLength + 1 + nbDecimals)
        self.setMinimumWidth(20 + QFontMetrics(self.font()).width(defaultText)) # +20: take padding into account
        self.setSizePolicy(QSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum))
        self.setAlignment(Qt.AlignHCenter)
        # add validator on acceptable range and display it in tooltip
        validator = QDoubleValidator(minValue, maxValue, nbDecimals)
        validator.setLocale(QLocale(QLocale.English))
        self.setValidator(validator);
        self.setToolTip("range : [" + str(minValue) + " ; " + str(maxValue) + "], max decimals : " + str(nbDecimals))
        
    #@return float : return the numeric value of the field (or 0.0 if not set)
    def getValue(self):
        if self.text() != "":
            return float(self.text())
        else:
            return 0.0
        
    def setValue(self, value):
        self.setText(str(value)) 

#
# This widget is a restricted text field usable to select a heading/angle value.
# User sets its value in degree, but SW get it in rads
#
class HeadingInput(FloatInput):
    def __init__(self, parent):
        super().__init__(parent, -180., 180., 1)
        
    def getValue(self):
        return normalizeAngle(math.radians(FloatInput.getValue(self)))
    
        
#
# This widget is a restricted combobox to select a direction forward/backward
#        
class DirectionInput(QComboBox):
    def __init__(self, parent):
        super().__init__(parent)
        self.addItem("Forward", Types_pb2.FORWARD)
        self.addItem("Backward", Types_pb2.BACKWARD)
        self.addItem("Undefined", Types_pb2.UNDEFINED)
        self.setEditable(True)
        self.lineEdit().setReadOnly(True)
        self.lineEdit().setAlignment(Qt.AlignHCenter)
        
    #@return int : return the numeric value of the field (or 0==Undefined if not set)
    def getValue(self):
        return self.currentData()
    
class ToneWidget(QWidget):
    toneRequest = pyqtSignal(Tone, int)
    
    def __init__(self, parent):
        super().__init__(parent)
        self.frequencyWidget = IntegerInput(self, 0, 20000)
        self.durationWidget = IntegerInput(self, 0, 10000)
        self.countWidget = IntegerInput(self, 0, 10)
        
        self.play = QPushButton('Play', self)
        self.play.clicked.connect(self._play) 
        
        layout = QVBoxLayout(self)
        layoutForm = QFormLayout()
        layout.addLayout(layoutForm)
        layout.addWidget(self.play)
        
        layoutForm.addRow("frequency (Hz)", self.frequencyWidget)
        layoutForm.addRow("duration (ms)", self.durationWidget)
        layoutForm.addRow("count", self.countWidget)
    
    @pyqtSlot()
    def _play(self):
        self.toneRequest.emit(Tone(self.frequencyWidget.getValue(), self.durationWidget.getValue()), self.countWidget.getValue())
     
class ImageWidget(QMdiArea):
    def __init__(self, parent, image): 
        super().__init__(parent)
        pixmap = QPixmap(image)
        if pixmap.isNull():
            print("Error loading " + image);
        self.background_pixmap = pixmap
        
    def paintEvent(self, event):
        painter = QPainter()
        painter.begin(self.viewport())
        painter.fillRect(event.rect(), self.palette().color(QPalette.Window))
        x = (self.width() - self.display_pixmap.width())/2
        y = (self.height() - self.display_pixmap.height())/2
        painter.drawPixmap(x, y, self.display_pixmap)
        painter.end()
    
    def resizeEvent(self, event):
    
        self.display_pixmap = self.background_pixmap.scaled(event.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation)

        
class LedIndicator(QWidget):
    def __init__(self, parent): 
        super().__init__(parent)
        self.l = QLabel("Bite")
        layout = QVBoxLayout(self)
        layout.addWidget(self.l)
        self.light(False)
        
    def light(self, on):
        if on:
            pixmap = QPixmap('img/green_light.png')
            if pixmap.isNull():
                print("Error loading img/green_light.png");
            self.l.setPixmap(pixmap)
        else:
            pixmap = QPixmap('img/grey_light.png')
            if pixmap.isNull():
                print("Error loading img/grey_light.png");
            self.l.setPixmap(pixmap)
            
            