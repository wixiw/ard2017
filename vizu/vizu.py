#!/usr/bin/python3
# -*- coding: utf-8 -*-

#expand path to find modules :
import sys

#Find the directory in which vizu.py is located
from os.path import dirname, abspath
DIR = dirname(abspath(__file__))

sys.path.append(DIR + "/../com")
sys.path.append(DIR + "/../com/generated")
sys.path.append(DIR + "/../com/nanopb-0.3.7-windows-x86/generator/proto")
sys.path.append(DIR + "/com")
sys.path.append(DIR + "/core")
sys.path.append(DIR + "/gui")

import signal

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *

from com import *
from gui import *

class ConnectScreen(QWidget):
    
    def __init__(self):
        super().__init__()             
        self.readSettings()
        self.setWindowTitle('8=> Vizu')  
        
        self.teleop = RemoteControl()
        
        self.tab = dict()
        self.tab["Com"]   = TabCom(self.teleop)
        self.tab["Log"]   = TabLog()
        self.tab["Strat"] = TabStrat()
        self.tab["Robot"] = TabRobot()
        self.tab["Help"] = TabHelp()
        
        
        self.tabs = QTabWidget(self)
        for tabName, tab in self.tab.items():
            self.tabs.addTab(tab, tabName)
        layout_main = QHBoxLayout(self)
        layout_main.addWidget(self.tabs)
        
        #connect Com tab
        self.tab["Com"].networkStatus         .connect(self._handleNetworkStatus)
        self.tab["Robot"].requestPlaySound    .connect(self.teleop.requestPlaySound)
        self.tab["Robot"].getOsStatsLogs      .connect(self.teleop.getOsStatsLogs)
        self.tab["Robot"].getTelemetry        .connect(self.teleop.getTelemetry)
        self.tab["Robot"].configureMatch      .connect(self.teleop.configureMatch)
        self.tab["Robot"].startMatch          .connect(self.teleop.startMatch)
        self.tab["Robot"].resetCpu            .connect(self.teleop.resetCpu)
        #connect Log tab
        self.teleop.log.connect(self.tab["Log"].log)
        #connect Strat tab
        self.teleop.telemetry.connect(self.tab["Strat"]._telemetryDataCb)
        #connect Robot tab
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
            #F4 for Robot tab
        self.shortcuts["F4"] = QShortcut(QKeySequence(Qt.Key_F4), self)
        self.shortcuts["F4"].activated.connect(self.tabShortcutMap.map)
        self.tabShortcutMap.setMapping(self.shortcuts["F4"], 3)
            #F5 for Help tab
        self.shortcuts["F5"] = QShortcut(QKeySequence(Qt.Key_F5), self)
        self.shortcuts["F5"].activated.connect(self.tabShortcutMap.map)
        self.tabShortcutMap.setMapping(self.shortcuts["F5"], 4)
        
        #add shortcut to manage match
        self.configureMap = QSignalMapper()
        self.configureMap.mapped.connect(self._configShortcut)
            #YELLOW config with "y"
        self.shortcuts["y"] = QShortcut(QKeySequence(Qt.Key_Y), self)
        self.shortcuts["y"].activated.connect(self.configureMap.map)
        self.configureMap.setMapping(self.shortcuts["y"], Types_pb2.PREF)
            #BLUE config with "b"        
        self.shortcuts["b"] = QShortcut(QKeySequence(Qt.Key_B), self)
        self.shortcuts["b"].activated.connect(self.configureMap.map)
        self.configureMap.setMapping(self.shortcuts["b"], Types_pb2.SYM)
            #Start match with s
        self.shortcuts["s"] = QShortcut(QKeySequence(Qt.Key_S), self)
        self.shortcuts["s"].activated.connect(self.teleop.startMatch)
            #reset with r
        self.shortcuts["r"] = QShortcut(QKeySequence(Qt.Key_R), self)
        self.shortcuts["r"].activated.connect(self.teleop.resetCpu)
        
            #disable tabs and shortcuts requiring a network connection
        self._handleNetworkStatus(False)
    
    def __del__(self):
        self.writeSettings()
    
    @pyqtSlot(bool)
    def _handleNetworkStatus(self, connected):
        if connected:
            self.tab["Log"].appendLog(("-----------connected-------------"))
            self.tabs.setTabEnabled(self.tabs.indexOf(self.tab["Strat"]), True)
            self.tabs.setTabEnabled(self.tabs.indexOf(self.tab["Robot"]), True)
            self.tabs.setCurrentWidget(self.tab["Strat"])
            
            self.shortcuts["F1"].setEnabled(True)
            self.shortcuts["F2"].setEnabled(True)
            self.shortcuts["F3"].setEnabled(True)
            self.shortcuts["F4"].setEnabled(True)
            self.shortcuts["F5"].setEnabled(True)
            
        else:
            self.tab["Log"].appendLog(("----------disconnected-----------"))
            self.tabs.setTabEnabled(self.tabs.indexOf(self.tab["Strat"]), False)
            self.tabs.setTabEnabled(self.tabs.indexOf(self.tab["Robot"]), False)
            self.tabs.setCurrentWidget(self.tab["Com"])
            
            self.shortcuts["F1"].setEnabled(True)
            self.shortcuts["F2"].setEnabled(True)
            self.shortcuts["F3"].setEnabled(False)
            self.shortcuts["F4"].setEnabled(False)
            self.shortcuts["F5"].setEnabled(True)
    
    @pyqtSlot(int)
    def selectTab(self, tabId):
        self.tabs.setCurrentIndex(tabId)
        
    def readSettings(self):
        settings = QSettings("config.ini", QSettings.IniFormat)
        settings.beginGroup("MainWindow")
        self.resize(settings.value("size", QSize(640, 480)))
        self.move(settings.value("pos", QPoint(300, 300)))
        settings.endGroup();
    
    def writeSettings(self):
        settings = QSettings("config.ini", QSettings.IniFormat)
        settings.beginGroup("MainWindow")
        settings.setValue("size", self.size())
        settings.setValue("pos", self.pos())
        settings.endGroup()
        
    @pyqtSlot(int)
    def _configShortcut(self, color):
        self.teleop.configureMatch(0,color) #0 for default strat
        
if __name__ == '__main__':
    import os
    
    #re-generate proto (not optimal, but as they will change a lot at project beginning...)
    #os.system("..\com\generateCom.bat .. off")
    
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    
    app = QApplication(sys.argv)
    app.setApplicationName("Vizu")
    app.setOrganizationName("A.R.D.")
    app.setOrganizationDomain("team-ard.com")
    
    #Start application
    screen = ConnectScreen()
    screen.show()
    res = app.exec_()
    
    #save settings
    del screen
    print("Settings saved")
    
    
    sys.exit(res)
