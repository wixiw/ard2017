#!/usr/bin/python3
# -*- coding: utf-8 -*-

import signal

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *

from com import *
from gui import *

class ConnectScreen(QWidget):
    
    def __init__(self):
        super().__init__()             
        self.resize(640, 480)
        self.move(300, 300)
        self.setWindowTitle('8=> Vizu')  
        
        self.teleop = Teleop()
        
        self.tab = dict()
        self.tab["Com"]   = TabCom(self.teleop.com)
        self.tab["Table"] = self.buildTabTable()
        self.tab["Robot"] = self.buildTabRobot()
        self.tab["Log"]   = TabLog()
        
        self.tabs = QTabWidget(self)
        self.tabs.setTabShape(QTabWidget.Rounded)
        self.tabs.addTab(self.tab["Com"],   "Com")
        self.tabs.addTab(self.tab["Log"],   "Logs")
        self.tabs.addTab(self.tab["Table"], "Table")
        self.tabs.addTab(self.tab["Robot"], "Robot")
        
        layout_main = QHBoxLayout(self)
        layout_main.addWidget(self.tabs)
        
        self.teleop.log[str].connect(self.log)
        self.tab["Com"].serialConnected     .connect(self._connectionEstablished)
        self.tab["Com"].getOsStats          .connect(self.teleop.getOsStats)
        self.tab["Com"].configureMatch      .connect(self.teleop.configureMatch)
        self.tab["Com"].startMatch          .connect(self.teleop.startMatch)
        
        
        #add shortcut to quit the app with ESC
        self.shortcuts = dict()
        self.shortcuts["ESC"] = QShortcut(QKeySequence(Qt.Key_Escape), self)
        self.shortcuts["ESC"].activated.connect(QCoreApplication.quit)
        
        #add F1 to F4 shortcuts for each tab
        self.tabShortcutMap = QSignalMapper()
        self.tabShortcutMap.mapped.connect(self.selectTab)
            #F1 for Com tab
        self.shortcuts["F1"] = QShortcut(QKeySequence(Qt.Key_F1), self)
        self.shortcuts["F1"].activated.connect(self.tabShortcutMap.map)
        self.tabShortcutMap.setMapping(self.shortcuts["F1"], 0)
            #F2 for Log tab
        self.shortcuts["F2"] = QShortcut(QKeySequence(Qt.Key_F2), self)
        self.shortcuts["F2"].activated.connect(self.tabShortcutMap.map)
        self.tabShortcutMap.setMapping(self.shortcuts["F2"], 1)
            #F3 for Table tab
        self.shortcuts["F3"] = QShortcut(QKeySequence(Qt.Key_F3), self)
        self.shortcuts["F3"].activated.connect(self.tabShortcutMap.map)
        self.tabShortcutMap.setMapping(self.shortcuts["F3"], 2)
            #F4 for Log tab
        self.shortcuts["F4"] = QShortcut(QKeySequence(Qt.Key_F4), self)
        self.shortcuts["F4"].activated.connect(self.tabShortcutMap.map)
        self.tabShortcutMap.setMapping(self.shortcuts["F4"], 3)

    
    @pyqtSlot()
    def _connectionEstablished(self):
        self.tab["Log"].appendLog(("-----------connected-------------\n"))
        #---DEBUG---
        #self.tabs.setCurrentWidget(self.tab["Log"])
        #self.tab["Com"]._getStats(True)
    
    @pyqtSlot(str)
    def log(self, logMsg):
        self.tab["Log"].appendLog(logMsg)
    
    @pyqtSlot(int)
    def selectTab(self, tabId):
        self.tabs.setCurrentIndex(tabId)
    
    def buildTabTable(self):
        tab_Table = QWidget(self)        
        return tab_Table
   
    def buildTabRobot(self):
        tab_Table = QWidget(self)        
        return tab_Table            
            
if __name__ == '__main__':
    import sys
    import os
    
    #re-generate proto (not optimal, but as they will change a lot at project beginning...)
    os.system("..\generate.bat ..\\")
    
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    app = QApplication(sys.argv)
    screen = ConnectScreen()
    screen.show()
    sys.exit(app.exec_())