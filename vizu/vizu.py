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

class ShortcutContext():
    AVAILABLE_OFFLINE = True
    
    def __init__(self, shortcut, keycode, callback, helpText = "no help", isAvailableOffline = False):
        self.shortcut = shortcut
        self.keycode = keycode
        self.helpText = helpText 
        self.callback = callback
        self.isAvailableOffline = isAvailableOffline
        self.widget = None
        
    def build(self, parent):
        self.widget = QShortcut(QKeySequence(self.keycode), parent)
        self.widget.activated.connect(self.callback)
        
class TabContext(ShortcutContext):
    
    def __init__(self, tab, shortcut, keycode, callback, helpText = "no help", isAvailableOffline = False):
        super().__init__(shortcut, keycode, callback, helpText, isAvailableOffline)
        self.tab = tab


        

class VizuMainScreen(QWidget):
    
    def __init__(self):
        super().__init__()             
        self.readSettings()
        self.setWindowTitle('8=> Vizu')  
        
        self.teleop = RemoteControl()
        self.shortcuts = dict()
        
        #create tabs content        
        self.tabShortcutMap = QSignalMapper()
        self.tabShortcutMap.mapped.connect(self.selectTab)
        self.tabContexts = dict()
        self.tabContexts["Com"]     = TabContext(TabCom(self.teleop),           "F1", Qt.Key_F1, self.tabShortcutMap.map, "Switch to Com tab.", TabContext.AVAILABLE_OFFLINE)
        self.tabContexts["Log"]     = TabContext(TabLog(),                      "F2", Qt.Key_F2, self.tabShortcutMap.map, "Switch to Log tab.", TabContext.AVAILABLE_OFFLINE)
        self.tabContexts["Strat"]   = TabContext(TabStrat(),                    "F3", Qt.Key_F3, self.tabShortcutMap.map, "Switch to Strat tab.")
        self.tabContexts["Robot"]   = TabContext(TabRobot(),                    "F4", Qt.Key_F4, self.tabShortcutMap.map, "Switch to Robot tab.")
        self.tabContexts["Config"]  = TabContext(TabConfig(),                   "F5", Qt.Key_F5, self.tabShortcutMap.map, "Switch to Config tab.")
        self.tabContexts["Help"]    = TabContext(TabHelp(),       "F6", Qt.Key_F6, self.tabShortcutMap.map,"Switch to LHelpog tab.", TabContext.AVAILABLE_OFFLINE)
        
        #create shortcuts
        for tabName, tabContext in self.tabContexts.items():
            self.shortcuts[tabContext.shortcut] = tabContext
        self.initShortcuts()
        
        #build tab widget from tab contents
        self.tabs = QTabWidget(self)
        self.tabs.currentChanged.connect(self.tabChanged)
        i = 0
        for tabName, tabContext in self.tabContexts.items():
            self.tabs.addTab(tabContext.tab, tabName)
            self.tabShortcutMap.setMapping(self.shortcuts[tabContext.shortcut].widget, i)
            i = i+1
        layout_main = QHBoxLayout(self)
        layout_main.addWidget(self.tabs)
        
        # connect Com Tab msg requests
        self.tabContexts["Com"].tab.networkStatus         .connect(self._handleNetworkStatus)

        # connect Log tab
        self.teleop.log.connect(self.tabContexts["Log"].tab.log)
        
        # connect Strat tab
        self.teleop.telemetry.connect(self.tabContexts["Strat"].tab._telemetryDataCb)
        
        # connect Robot tab
        self.tabContexts["Robot"].tab.requestPlaySound    .connect(self.teleop.requestPlaySound)
        self.tabContexts["Robot"].tab.getOsStatsLogs      .connect(self.teleop.getOsStatsLogs)
        self.tabContexts["Robot"].tab.getComStatsLogs     .connect(self.teleop.getComStatsLogs)
        self.tabContexts["Robot"].tab.getTelemetry        .connect(self.teleop.getTelemetry)
        self.tabContexts["Robot"].tab.configureMatch      .connect(self.teleop.configureMatch)
        self.tabContexts["Robot"].tab.startMatch          .connect(self.teleop.startMatch)
        self.tabContexts["Robot"].tab.resetCpu            .connect(self.teleop.resetCpu)
        self.tabContexts["Robot"].tab.blocked             .connect(self.teleop.requestBlockRobot)
        for cmd, widget in self.tabContexts["Robot"].tab.navTab.items():
            widget.execute.connect(getattr(self.teleop, cmd))  # getattr is used to get a method reference from name, hence automatically binding signals ;p
        
        # connect Config tab
        self.tabContexts["Config"].tab.getConfig          .connect(self.teleop.getConfig)
        self.tabContexts["Config"].tab.setConfig          .connect(self.teleop.setConfig)
        self.teleop.config.connect(self.tabContexts["Config"].tab.updateConfig)
        
        #Spread shortcuts data to Help screen
        self.tabContexts["Help"].tab.setShortcuts(self.shortcuts)
        
        # disable tabs and shortcuts requiring a network connection
        self._handleNetworkStatus(False)
    
    def __del__(self):
        self.writeSettings()
    
    @pyqtSlot(bool)
    def _handleNetworkStatus(self, connected):
        if connected:
            self.tabContexts["Log"].tab.appendLog(("-----------connected-------------"))
            #enable all tabs
            for tabName, tabContext in self.tabContexts.items():
                self.tabs.setTabEnabled(self.tabs.indexOf(tabContext.tab), True)
            
            #automatically switch to strat tab for convenience
            self.tabs.setCurrentWidget(self.tabContexts["Strat"].tab)
            
            #activate all shortcuts
            for key, shortcutContext in self.shortcuts.items():
                shortcutContext.widget.setEnabled(True)
        else:
            self.tabContexts["Log"].tab.appendLog(("----------disconnected-----------"))
            #disable all tabs that are not available offline
            for tabName, tabContext in self.tabContexts.items():
                self.tabs.setTabEnabled(self.tabs.indexOf(tabContext.tab), tabContext.isAvailableOffline)
                
            #automatically switch to Com tab for convenience
            self.tabs.setCurrentWidget(self.tabContexts["Com"].tab)
            
            #Deactivate shortcuts that are only available online
            for key, shortcut in self.shortcuts.items():
                shortcut.widget.setEnabled(shortcut.isAvailableOffline)
    
    #Signal activated by shortcuts (helper to being able to map in conneciton)
    @pyqtSlot(int)
    def selectTab(self, tabId):
        self.tabs.setCurrentIndex(tabId)
        
    #Signal emitted by QT when a new tab is displayed
    @pyqtSlot(int)
    def tabChanged(self, tabId):
        #if current tab is config, then request the current config
        if self.tabs.currentWidget() == self.tabContexts["Config"].tab:
            self.teleop.getConfig()
        
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
        
    def initShortcuts(self):        
        # add shortcut to quit the app with ESC
        self.shortcuts["ESC"] = ShortcutContext("ESC", Qt.Key_Escape, QCoreApplication.quit, "Quits vizu", ShortcutContext.AVAILABLE_OFFLINE)

        # add shortcut to manage match
        self.configureMap = QSignalMapper()
        self.configureMap.mapped.connect(self._configShortcut)
            # YELLOW config with "y"
        self.shortcuts["y"] = ShortcutContext("y", Qt.Key_Y, self.configureMap.map, "Configure a match with Alpha strategy and YELLOW color.")  
            # BLUE config with "b"       
        self.shortcuts["b"] = ShortcutContext("b", Qt.Key_B, self.configureMap.map, "Configure a match with Alpha strategy and BLUE color.")  
            # Start match with s
        self.shortcuts["s"] = ShortcutContext("s", Qt.Key_S, self.teleop.startMatch, "Starts the match.") 
            # reset with r
        self.shortcuts["r"] = ShortcutContext("r", Qt.Key_R, self.teleop.resetCpu, "Starts the match.") 
            # block/unblock robot with x
        self.shortcuts["x"] = ShortcutContext("x", Qt.Key_X, self.tabContexts["Robot"].tab._blockFromShortcut, "Create/destroy a virtual opponent on the robot path") 
        
        #register shortcuts into main widget
        for name, shortcutContext in self.shortcuts.items():
            shortcutContext.build(self)
            
        self.configureMap.setMapping(self.shortcuts["y"].widget, Types_pb2.PREF)
        self.configureMap.setMapping(self.shortcuts["b"].widget, Types_pb2.SYM)
        
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
    screen = VizuMainScreen()
    screen.show()
    res = app.exec_()
    
    # save settings
    del screen
    print("Settings saved")
    
    
    sys.exit(res)
