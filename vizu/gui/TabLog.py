#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from proto import *
import threading

#
# This class is a pre-built widget which is designed to display 
# a console log.
#
class TabLog(QWidget):
    
    def __init__(self):
        super().__init__()

        self.time       = Qt.Checked
        self.level      = Qt.Checked
        self.component  = Qt.Checked 

        self.text_logs = QPlainTextEdit(self)
        self.text_logs.centerOnScroll()
        self.text_logs.setMaximumBlockCount(200);
        self.text_logs.setReadOnly(True)
        
#         p = QPalette()
#         p.setColor(QPalette.Base, Qt.black)
#         p.setColor(QPalette.Text, Qt.green)
#         self.text_logs.setPalette(p)

        self.btn_reset = QPushButton('Reset logs', self)
        self.btn_reset.clicked.connect(self.text_logs.clear)

#         self.combo_level  = QComboBox(self);
#         self.combo_level.addItem("No level")
#         self.combo_level.addItem("Short")
#         self.combo_level.addItem("Explicit")

        self.chk_time   = QCheckBox("time", self)
        self.chk_time.stateChanged.connect(self.timeCfg)
        self.chk_time.setCheckState(Qt.Checked)
        
        self.chk_level   = QCheckBox("level", self)
        self.chk_level.setTristate(True)
        self.chk_level.stateChanged.connect(self.levelCfg)
        self.chk_level.setCheckState(Qt.Checked)
        
        self.chk_cpt    = QCheckBox("component", self)
        self.chk_cpt.stateChanged.connect(self.componentCfg)
        self.chk_cpt.setCheckState(Qt.Checked)
        
        layout = QVBoxLayout(self)
        layoutFilters = QHBoxLayout()
        layoutFilters.addWidget(self.chk_time)
        layoutFilters.addWidget(self.chk_level)
        layoutFilters.addWidget(self.chk_cpt)
        layoutFilters.addWidget(self.btn_reset)
        layout.addLayout(layoutFilters)
        layout.addWidget(self.text_logs)
                 
    @pyqtSlot(int)
    def timeCfg(self, state):
        self.time = state
       
    @pyqtSlot(int)
    def levelCfg(self, state):
        self.level = state
        
    @pyqtSlot(int)
    def componentCfg(self, state):
        self.component = state 
        
    # append a log at the end of the display
    # if the number of log is more than the one configured with
    # setNbLogs(), the oldest log (the one at the top) is deleted.
    # @param str: the data to log
    def appendLog(self, data):
        assert isinstance(data, str), "appendLog expects to receive a string" 
        print(data)
        self.text_logs.appendPlainText(data)
        bar = self.text_logs.verticalScrollBar()
        bar.setValue(bar.maximum())
        
    # configure the number of logs displayed
    # @param int
    def setNbLogs(self, nb):
        self.text_logs.document().setMaximumBlockCount(nb)

    def getLogLvlShortName(self, lvl):
        if lvl == Types_pb2.DEBUG:
            return 'D'
        elif lvl == Types_pb2.ERROR:
            return 'E'
        elif lvl == Types_pb2.INFO:
            return 'I'
        elif lvl == Types_pb2.ASSERT:
            return 'A'
        else:
            return '?'

    @pyqtSlot(RemoteControl_pb2.Log)
    def log(self, logMsg):
        header = str()
        #time
        if self.time == Qt.Checked:
            header+= str(logMsg.date).zfill(5) + " "
        #level
        if self.level == Qt.Checked:
            header+= "[" + Types_pb2.eLogLevel.Name(logMsg.level) + "] "
        elif self.level == Qt.PartiallyChecked:
            header+= "[" + self.getLogLvlShortName(logMsg.level) + "] "
        #component
        if self.component == Qt.Checked:
            header+= "[" + logMsg.component.rjust(8) + "] "
            
        self.appendLog(header + logMsg.text)
        
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    widget = TabLog()
    widget.show()
    widget.setNbLogs(5)
    widget.appendLog("1 Coucou\n")
    widget.appendLog("2 Poil\n")
    widget.appendLog("3 Bite\r\n")
    widget.appendLog("4 Chatte\n")
    widget.appendLog("5 Couille\n")
    widget.appendLog("6 Godanal")
    sys.exit(app.exec_())
