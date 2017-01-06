#!/usr/bin/python3
# -*- coding: utf-8 -*-

import signal
from gui import *
from yahdlc import *
from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *

class ConnectScreen(QWidget):
    
    def __init__(self):
        super().__init__()             
        self.resize(640, 480)
        self.move(300, 300)
        self.setWindowTitle('8=> Vizu')  
              
        self.com = ArdSerial()
        self.com.registerListener(self._dataAvailable)
        
        self.tab_Com = TabCom(self.com)
        self.tab_Table = self.buildTabTable()
        self.tab_Robot = self.buildTabRobot()
        self.tab_Log = TabLog()
        
        self.tabs = QTabWidget(self)
        self.tabs.setTabShape(QTabWidget.Rounded)
        self.tabs.addTab(self.tab_Com,   "Com")
        self.tabs.addTab(self.tab_Log,   "Logs")
        self.tabs.addTab(self.tab_Table, "Table")
        self.tabs.addTab(self.tab_Robot, "Robot")
        
        layout_main = QHBoxLayout(self)
        layout_main.addWidget(self.tabs)
        
        self.tab_Com.serialConnected.connect(self._connectionEstablished)
        self.tab_Com.sendMsg.connect(self._msgToSend)
        
    @pyqtSlot()
    def _dataAvailable(self):
        data = self.com.readAll()
        self.tab_Log.appendLog(data.decode())
    
    @pyqtSlot()
    def _connectionEstablished(self):
        self.tab_Log.appendLog(("-----------connected-------------\n"))
        #---DEBUG---
        #self.tabs.setCurrentWidget(self.tab_Log)
        #self.tab_Com._getStats(True)
        
    @pyqtSlot(bytes)
    def _msgToSend(self, msg):
        print("Msg send request : [%s]" % msg)
        # sequence id is not used yet, as Teleop doesn't require any robustness, 
        # in order to use this com with a robot to robot com, it'll be required
        frame = frame_data(msg, FRAME_DATA, 1)
        print("HLDC frame : [%s]" % frame)
        if self.com.write(frame) == len(frame):
            print("Frame sent successfully.")
        else:
            print("Frame send error.")
    
    def buildTabTable(self):
        tab_Table = QWidget(self)        
        return tab_Table
   
    def buildTabRobot(self):
        tab_Table = QWidget(self)        
        return tab_Table            
            
if __name__ == '__main__':
    import sys
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    app = QApplication(sys.argv)
    screen = ConnectScreen()
    screen.show()
    sys.exit(app.exec_())