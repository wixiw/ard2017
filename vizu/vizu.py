#!/usr/bin/python3
# -*- coding: utf-8 -*-

from gui import *
from PyQt5.Qt import *
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
        self.tabs.addTab(self.tab_Table, "Table")
        self.tabs.addTab(self.tab_Robot, "Robot")
        self.tabs.addTab(self.tab_Log,   "Logs")
        
        layout_main = QHBoxLayout(self)
        layout_main.addWidget(self.tabs)
        
        self.tab_Com.serialConnected.connect(self._connectionEstablished)
        
    @pyqtSlot()
    def _dataAvailable(self):
        bytes = self.com.readAll()
        #I don't know why, but QByteArray encapsulate the string into a b'...' pattern which fools the log view
        #as a workaround a string is reconstructed
        zizi = ""
        for b  in bytes:
            zizi += b
        self.tab_Log.appendLog(zizi)
    
    @pyqtSlot()
    def _connectionEstablished(self):
        self.tabs.setCurrentWidget(self.tab_Log)
    
    def buildTabTable(self):
        tab_Table = QWidget(self)        
        return tab_Table
   
    def buildTabRobot(self):
        tab_Table = QWidget(self)        
        return tab_Table            
            
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    screen = ConnectScreen()
    screen.show()
    sys.exit(app.exec_())