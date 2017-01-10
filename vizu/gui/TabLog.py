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

        layout = QVBoxLayout(self)
        layout.addWidget(self.btn_reset)
        layout.addWidget(self.text_logs)
                 
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
        self.text_logs.document().setMaximumBlockCount(nb);

    @pyqtSlot(Teleop_pb2.Log)
    def log(self, logMsg):
        header = str(logMsg.date).zfill(5) + " [" + Types_pb2.eLogLevel.Name(logMsg.level) + "] "
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