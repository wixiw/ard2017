#!/usr/bin/python3
# -*- coding: utf-8 -*-

#expand path to find modules :
import sys
sys.path.append("com")
sys.path.append("gui")
sys.path.append("proto")

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
        
        self.teleop = RemoteControl()
        
        self.tab = dict()
        self.tab["Com"]   = TabCom(self.teleop)
        self.tab["Log"]   = TabLog()
        self.tab["Strat"] = self.buildTabTable()
        self.tab["Robot"] = TabRobot()
        
        
        self.tabs = QTabWidget(self)
        for tabName, tab in self.tab.items():
            self.tabs.addTab(tab, tabName)
        #disable tabs requiring a network connection
        self._handleNetworkStatus(False)
        layout_main = QHBoxLayout(self)
        layout_main.addWidget(self.tabs)
        
        #connect Com tab
        self.tab["Com"].networkStatus       .connect(self._handleNetworkStatus)
        self.tab["Com"].getOsStats          .connect(self.teleop.getOsStats)
        self.tab["Com"].configureMatch      .connect(self.teleop.configureMatch)
        self.tab["Com"].startMatch          .connect(self.teleop.startMatch)
        #connect Log tab
        self.teleop.log.connect(self.tab["Log"].log)
        #conenct Robot tab
        for cmd, widget in self.tab["Robot"].navTab.items():
            widget.execute.connect(getattr(self.teleop, cmd))  #getattr is used to get a method reference from name, hence automatically binding signals ;p
        
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

    
    @pyqtSlot(bool)
    def _handleNetworkStatus(self, connected):
        if connected:
            self.tab["Log"].appendLog(("-----------connected-------------"))
            self.tabs.setTabEnabled(self.tabs.indexOf(self.tab["Strat"]), True)
            self.tabs.setTabEnabled(self.tabs.indexOf(self.tab["Robot"]), True)
            #self.tabs.setCurrentWidget(self.tab["Strat"])
        else:
            self.tab["Log"].appendLog(("----------disconnected-----------"))
            self.tabs.setTabEnabled(self.tabs.indexOf(self.tab["Strat"]), False)
            self.tabs.setTabEnabled(self.tabs.indexOf(self.tab["Robot"]), False)
            self.tabs.setCurrentWidget(self.tab["Com"])
    
    @pyqtSlot(int)
    def selectTab(self, tabId):
        self.tabs.setCurrentIndex(tabId)
        
    def buildTabTable(self):
        tab_Table = QWidget(self)        
        return tab_Table
            
if __name__ == '__main__':
    import sys
    import os
    
    #re-generate proto (not optimal, but as they will change a lot at project beginning...)
    os.system("..\generateCom.bat ..\\ off")
    
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    app = QApplication(sys.argv)
    screen = ConnectScreen()
    screen.show()
    sys.exit(app.exec_())