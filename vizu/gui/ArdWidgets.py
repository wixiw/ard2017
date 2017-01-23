#!/usr/bin/python3
# -*- coding: utf-8 -*-

import signal
import math
from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from proto import *

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

#
# This widget is a restricted text field usable to select a heading/angle value.
#
class HeadingInput(FloatInput):
    def __init__(self, parent):
        super().__init__(parent, -180., 180., 1)
        
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
    
    