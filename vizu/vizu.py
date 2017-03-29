#!/usr/bin/python3
# -*- coding: utf-8 -*-

# expand path to find modules :
import sys

# Find the directory in which vizu.py is located
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
        
        self.shortcuts = dict()
        self.shortcutsOffline = dict()
        self.shortcutsText = dict()
        
        self.teleop = RemoteControl()
        
        self.tab = dict()
        self.tab["Com"] = TabCom(self.teleop)
        self.tab["Log"] = TabLog()
        self.tab["Strat"] = TabStrat()
        self.tab["Robot"] = TabRobot()
        self.tab["Config"] = TabConfig()
        self.tab["Help"] = TabHelp(self.shortcutsText)
        
        
        self.tabs = QTabWidget(self)
        for tabName, tab in self.tab.items():
            self.tabs.addTab(tab, tabName)
        layout_main = QHBoxLayout(self)
        layout_main.addWidget(self.tabs)
        
        # connect Com tab
        self.tab["Com"].networkStatus         .connect(self._handleNetworkStatus)
        self.tab["Robot"].requestPlaySound    .connect(self.teleop.requestPlaySound)
        self.tab["Robot"].getOsStatsLogs      .connect(self.teleop.getOsStatsLogs)
        self.tab["Robot"].getTelemetry        .connect(self.teleop.getTelemetry)
        self.tab["Robot"].configureMatch      .connect(self.teleop.configureMatch)
        self.tab["Robot"].startMatch          .connect(self.teleop.startMatch)
        self.tab["Robot"].resetCpu            .connect(self.teleop.resetCpu)
        self.tab["Robot"].blocked             .connect(self.teleop.requestBlockRobot)
        # connect Log tab
        self.teleop.log.connect(self.tab["Log"].log)
        # connect Strat tab
        self.teleop.telemetry.connect(self.tab["Strat"]._telemetryDataCb)
        # connect Robot tab
        for cmd, widget in self.tab["Robot"].navTab.items():
            widget.execute.connect(getattr(self.teleop, cmd))  # getattr is used to get a method reference from name, hence automatically binding signals ;p
        
        # add shortcut to quit the app with ESC
        self.addShortcut("ESC",
                    Qt.Key_Escape,
                    True,
                    "Quits vizu",
                    QCoreApplication.quit)

        # Add shortcuts for tabs
        self.tabShortcutMap = QSignalMapper()
        self.tabShortcutMap.mapped.connect(self.selectTab)
        self.addShortcut("F1",
                    Qt.Key_F1,
                    True,
                    "Switch to Com tab.",
                    self.tabShortcutMap.map)
        self.addShortcut("F2",
                    Qt.Key_F2,
                    True,
                    "Switch to Logs tab.",
                    self.tabShortcutMap.map)
        self.addShortcut("F3",
                    Qt.Key_F3,
                    False,
                    "Switch to Strategy tab.",
                    self.tabShortcutMap.map)
        self.addShortcut("F4",
                    Qt.Key_F4,
                    False,
                    "Switch to Robot control tab.",
                    self.tabShortcutMap.map)
        self.addShortcut("F5",
                    Qt.Key_F5,
                    False,
                    "Switch to Robot configuration tab.",
                    self.tabShortcutMap.map)
        self.addShortcut("F6",
                    Qt.Key_F6,
                    True,
                    "Switch to Help tab.",
                    self.tabShortcutMap.map)
        self.tabShortcutMap.setMapping(self.shortcuts["F1"], 0)
        self.tabShortcutMap.setMapping(self.shortcuts["F2"], 1)
        self.tabShortcutMap.setMapping(self.shortcuts["F3"], 2)
        self.tabShortcutMap.setMapping(self.shortcuts["F4"], 3)
        self.tabShortcutMap.setMapping(self.shortcuts["F5"], 4)
        self.tabShortcutMap.setMapping(self.shortcuts["F6"], 5)
        
        # add shortcut to manage match
        self.configureMap = QSignalMapper()
        self.configureMap.mapped.connect(self._configShortcut)
            # YELLOW config with "y"
        self.addShortcut("y",
                    Qt.Key_Y,
                    True,
                    "Configure a match with Alpha strategy and YELLOW color.",
                    self.configureMap.map)  
        self.configureMap.setMapping(self.shortcuts["y"], Types_pb2.PREF)
            # BLUE config with "b"       
        self.addShortcut("b",
                    Qt.Key_B,
                    True,
                    "Configure a match with Alpha strategy and BLUE color.",
                    self.configureMap.map)  
        self.configureMap.setMapping(self.shortcuts["b"], Types_pb2.SYM)
            # Start match with s
        self.addShortcut("s",
                    Qt.Key_S,
                    True,
                    "Starts the match.",
                    self.teleop.startMatch) 
            # reset with r
        self.addShortcut("r",
                    Qt.Key_R,
                    True,
                    "Starts the match.",
                    self.teleop.resetCpu) 
            # block/unblock robot with x
        self.addShortcut("x",
                    Qt.Key_X,
                    True,
                    "Create/destroy a virtual opponent on the robot path",
                    self.tab["Robot"]._blockFromShortcut) 
        
            # disable tabs and shortcuts requiring a network connection
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
            
            for key, shortcut in self.shortcuts.items():
                shortcut.setEnabled(True)
        else:
            self.tab["Log"].appendLog(("----------disconnected-----------"))
            self.tabs.setTabEnabled(self.tabs.indexOf(self.tab["Strat"]), False)
            self.tabs.setTabEnabled(self.tabs.indexOf(self.tab["Robot"]), False)
            self.tabs.setCurrentWidget(self.tab["Com"])
            
            for key, shortcut in self.shortcuts.items():
                shortcut.setEnabled(self.shortcutsOffline[key])
    
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
        self.teleop.configureMatch(0, color)  # 0 for default strat
        
    def addShortcut(self, key, keycode, availableWhenDisconnected, text, callback):
        self.shortcuts[key] = QShortcut(QKeySequence(keycode), self)
        self.shortcuts[key].activated.connect(callback)
        self.shortcutsOffline[key] = availableWhenDisconnected
        self.shortcutsText[key] = text
        
if __name__ == '__main__':
    import os
    
    # re-generate proto (not optimal, but as they will change a lot at project beginning...)
    # os.system("..\com\generateCom.bat .. off")
    
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    
    app = QApplication(sys.argv)
    app.setApplicationName("Vizu")
    app.setOrganizationName("A.R.D.")
    app.setOrganizationDomain("team-ard.com")
    
    # Start application
    screen = ConnectScreen()
    screen.show()
    res = app.exec_()
    
    # save settings
    del screen
    print("Settings saved")
    
    
    sys.exit(res)
