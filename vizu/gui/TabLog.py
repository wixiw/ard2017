#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
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
        
        layout_Log = QHBoxLayout(self)
        layout_Log.addWidget(self.text_logs)

        self.lock = threading.Lock()

    # append a log at the end of the display
    # if the number of log is more than the one configured with
    # setNbLogs(), the oldest log (the one at the top) is deleted.
    # @param str: the data to log
    def appendLog(self, data):
        assert isinstance(data, str), "appendLog expects to receive a string" 
        self.lock.acquire()
        print(data)
        self.text_logs.insertPlainText(data)
        bar = self.text_logs.verticalScrollBar()
        bar.setValue(bar.maximum())
        self.lock.release()
        
    # configure the number of logs displayed
    # @param int
    def setNbLogs(self, nb):
        self.text_logs.document().setMaximumBlockCount(nb);

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