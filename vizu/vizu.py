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
sys.path.append(DIR + "/robot")

import signal

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *

from com import *
from gui import *
from robot import *

class ShortcutContext():
    AVAILABLE_OFFLINE = True
    
    def __init__(self, shortcut, callback, helpText = "no help", isAvailableOffline = False):
        self.shortcut = shortcut
        self.helpText = helpText 
        self.callback = callback
        self.isAvailableOffline = isAvailableOffline
        self.widget = None
        
    def build(self, parent):
        self.widget = QShortcut(QKeySequence(self.shortcut), parent)
        self.widget.activated.connect(self.callback)
        
class TabContext(ShortcutContext):
    
    def __init__(self, tab, shortcut, callback, helpText = "no help", isAvailableOffline = False):
        super().__init__(shortcut, callback, helpText, isAvailableOffline)
        self.tab = tab


        

class VizuMainScreen(QWidget):
    
    def __init__(self, clipboard):
        super().__init__()  
        self.resize(870, 500)           
        self.readSettings()
        self.setWindowTitle('Vizu [8=>]')  
        self.robotProxy = Robot()
        self.teleop = RemoteControl()
        self.shortcuts = dict()
        
        #create tabs content        
        self.tabShortcutMap = QSignalMapper()
        self.tabShortcutMap.mapped.connect(self.selectTab)
        self.tabContexts = dict()
        self.tabContexts["Com"]     = TabContext(TabCom(self, self.teleop),         "F1", self.tabShortcutMap.map, "Switch to Com tab.", TabContext.AVAILABLE_OFFLINE)
        self.tabContexts["Log"]     = TabContext(TabLog(self),                      "F2", self.tabShortcutMap.map, "Switch to Log tab.", TabContext.AVAILABLE_OFFLINE)
        self.tabContexts["Strat"]   = TabContext(TabStrat(self, self.teleop, self.robotProxy, clipboard),       "F3", self.tabShortcutMap.map, "Switch to Strat tab.")
        self.tabContexts["Robot"]   = TabContext(TabRobot(self),                    "F4", self.tabShortcutMap.map, "Switch to Robot tab.")
        self.tabContexts["Config"]  = TabContext(TabConfig(self),                   "F5", self.tabShortcutMap.map, "Switch to Config tab.")
        self.tabContexts["Help"]    = TabContext(TabHelp(self),                     "F6", self.tabShortcutMap.map,"Switch to LHelpog tab.", TabContext.AVAILABLE_OFFLINE)
        
        #create shortcuts
        for tabName, tabContext in self.tabContexts.items():
            self.shortcuts[tabContext.shortcut] = tabContext
        self.initShortcuts()
        
        #build tab widget from tab contents
        self.tabs = QTabWidget(self)
        i = 0
        for tabName, tabContext in self.tabContexts.items():
            self.tabs.addTab(tabContext.tab, tabName)
            self.tabShortcutMap.setMapping(self.shortcuts[tabContext.shortcut].widget, i)
            i = i+1
        layout_main = QHBoxLayout(self)
        layout_main.addWidget(self.tabs)
        
        # connect Com Tab msg requests
        self.teleop.networkStatus.connect(self._handleNetworkStatus)
        self.teleop.serialNumber.connect(self._handleSerialNumber)

        # connect Log tab
        self.teleop.log.connect(self.tabContexts["Log"].tab.log)
        
        # connect Strat tab
        self.teleop.telemetry.connect(self.tabContexts["Strat"].tab._telemetryDataCb)
        self.teleop.config.connect(self.tabContexts["Strat"].tab._updateConfig)
        self.teleop.graphState.connect(self.tabContexts["Strat"].tab._updateGraphState)
        self.teleop.graphNodes.connect(self.tabContexts["Strat"].tab._updateGraphNodes)
        self.teleop.graphLinks.connect(self.tabContexts["Strat"].tab._updateGraphLinks)
        
        # connect Robot tab
        self.tabContexts["Robot"].tab.tab["Commands"].sections["sound"].requestPlaySound      .connect(self.teleop.requestPlaySound)
        self.tabContexts["Robot"].tab.tab["Commands"].sections["general"].getOsStatsLogs      .connect(self.teleop.getOsStatsLogs)
        self.tabContexts["Robot"].tab.tab["Commands"].sections["general"].getComStatsLogs     .connect(self.teleop.getComStatsLogs)
        self.tabContexts["Robot"].tab.tab["Commands"].sections["general"].getTelemetry        .connect(self.teleop.getTelemetry)
        self.tabContexts["Robot"].tab.tab["Commands"].sections["general"].resetCpu            .connect(self.tabContexts["Strat"].tab.resetCpu)
        self.tabContexts["Robot"].tab.tab["Commands"].sections["general"].requestMotionGraph  .connect(self.teleop.requestMotionGraph)
        self.tabContexts["Robot"].tab.tab["Commands"].sections["nav"].blocked                 .connect(self.teleop.requestBlockRobot)
        for cmd, widget in self.tabContexts["Robot"].tab.tab["Commands"].sections["nav"].navTab.items():
            widget.execute.connect(getattr(self.teleop, cmd))  # getattr is used to get a method reference from name, hence automatically binding signals ;p
        self.tabContexts["Robot"].tab.tab["Calib"].sections["Linear"].move                    .connect(self.teleop.goForward)
        self.tabContexts["Robot"].tab.tab["Calib"].sections["Rotation"].move                  .connect(self.teleop.turnDelta)
        self.tabContexts["Robot"].tab.tab["Actuators"].sections["lifter"].actCmd              .connect(self.teleop.requestActuatorsCmd)
        self.tabContexts["Robot"].tab.tab["Servos"].lifterCmd                                 .connect(self.teleop.requestLifterServo)
        self.tabContexts["Robot"].tab.tab["Servos"].leftArmCmd                                .connect(self.teleop.requestLeftArmServo)
        self.tabContexts["Robot"].tab.tab["Servos"].rightArmCmd                               .connect(self.teleop.requestRightArmServo)
        self.tabContexts["Robot"].tab.tab["Servos"].leftWheelCmd                              .connect(self.teleop.requestLeftWheelServo)
        self.tabContexts["Robot"].tab.tab["Servos"].rightWheelCmd                             .connect(self.teleop.requestRightWheelServo)
        self.tabContexts["Robot"].tab.tab["Servos"].funnyCmd                                  .connect(self.teleop.requestFunnyActionServo)
        self.teleop.telemetry.connect(self.tabContexts["Robot"].tab.tab["Status"]._telemetryDataCb)
        self.teleop.telemetry.connect(self.tabContexts["Robot"].tab.tab["Servos"]._telemetryDataCb)
        self.teleop.telemetry.connect(self.tabContexts["Robot"].tab.tab["Actuators"]._telemetryDataCb)
        
        # connect Config tab
        self.tabContexts["Config"].tab.getConfig          .connect(self.teleop.getConfig)
        self.tabContexts["Config"].tab.setConfig          .connect(self.teleop.setConfig)
        self.teleop.config.connect(self.tabContexts["Config"].tab.updateConfig)
        self.teleop.config.connect(self.tabContexts["Robot"].tab.tab["Calib"].sections["Linear"].updateConfig)
        self.teleop.config.connect(self.tabContexts["Robot"].tab.tab["Calib"].sections["Rotation"].updateConfig)
        
        #RobotProxy
        self.teleop.telemetry.connect(self.robotProxy._telemetryDataCb)
        self.teleop.serialNumber.connect(self.robotProxy._handleSerialNumber)
        
        #Spread shortcuts data to Help screen
        self.tabContexts["Help"].tab.setShortcuts(self.shortcuts)
        
        # disable tabs and shortcuts requiring a network connection
        self._handleNetworkStatus(False)
    
    @pyqtSlot(bool)
    def _handleNetworkStatus(self, connected):
        if connected:
#             #As connecting may reboot the robot wait a bit
#             time.sleep(2.200)
#             
#             #request robot serial
#             self.teleop.getSerial()
            
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
                
            #remove robot name from title
            self.setWindowTitle('8=> Vizu')
    
    @pyqtSlot(RemoteControl_pb2.SerialNumber)
    def _handleSerialNumber(self, serial):
        #print("New serial received : " + serial.value)
        self.setWindowTitle('8=> Vizu [' + serial.value + ']') 
        self.tabContexts["Strat"].tab.updateRobot(serial.value)
        time.sleep(0.100)
        self.teleop.requestMotionGraph()
    
    #Signal activated by shortcuts (helper to being able to map in conneciton)
    @pyqtSlot(int)
    def selectTab(self, tabId):
        self.tabs.setCurrentIndex(tabId)
        
    def readSettings(self):
        settings = QSettings("config.ini", QSettings.IniFormat)
        settings.beginGroup("MainWindow")
        self.resize(settings.value("size", QSize(870, 500)))
        self.move(settings.value("pos", QPoint(300, 300)))
        settings.endGroup();
    
    def writeSettings(self):
        settings = QSettings("config.ini", QSettings.IniFormat)
        settings.beginGroup("MainWindow")
        settings.setValue("size", self.size())
        settings.setValue("pos", self.pos())
        settings.endGroup()
        print("Settings saved")
        
    def initShortcuts(self):        
        # add shortcut to quit the app with ESC
        self.shortcuts["Ctrl+C"] = ShortcutContext("Ctrl+C", QCoreApplication.quit, "Quits vizu", ShortcutContext.AVAILABLE_OFFLINE)

        # add shortcut to manage match
        self.configureMap = QSignalMapper()
        self.configureMap.mapped.connect(self.tabContexts["Strat"].tab.setColor)
            # YELLOW config with "y"
        self.shortcuts["y"] = ShortcutContext("y", self.configureMap.map, "Configure a match with Alpha strategy and YELLOW color.")  
            # BLUE config with "b"       
        self.shortcuts["b"] = ShortcutContext("b", self.configureMap.map, "Configure a match with Alpha strategy and BLUE color.")  
            # Start match with s
        self.shortcuts["s"] = ShortcutContext("s", self.tabContexts["Strat"].tab.startMatch, "Starts the match.") 
            # reset with r
        self.shortcuts["r"] = ShortcutContext("r", self.tabContexts["Strat"].tab.resetCpu, "Reset the CPU.") 
            # block/unblock robot with x
        self.shortcuts["x"] = ShortcutContext("x", self.tabContexts["Robot"].tab.tab["Commands"].sections["nav"]._blockFromShortcut, "Create/destroy a virtual opponent on the robot path.") 
            # CGTC
        self.shortcuts["g"] = ShortcutContext("g", self.tabContexts["Strat"].tab.copyGhostToClipboard, "*THE* famous copy ghost to clipboard feature.")
            #Connect/Disconnect
        self.shortcuts["c"] = ShortcutContext("c", self.tabContexts["Com"].tab._connectFromShorcut, "Connects or disconnect from the robot", ShortcutContext.AVAILABLE_OFFLINE)

        #register shortcuts into main widget
        for name, shortcutContext in self.shortcuts.items():
            shortcutContext.build(self)
            
        self.configureMap.setMapping(self.shortcuts["y"].widget, Types_pb2.PREF)
        self.configureMap.setMapping(self.shortcuts["b"].widget, Types_pb2.SYM)
   
   
class VizuApplication(QApplication):
    
    def __init__(self):
        super().__init__(sys.argv)
        self.setApplicationName("Vizu")
        self.setOrganizationName("A.R.D.")
        self.setOrganizationDomain("team-ard.com")
        self.aboutToQuit.connect(self._quitHandler)
        
        #Build main view
        self.mainWindow = VizuMainScreen(self.clipboard())
        
        signal.signal(signal.SIGINT, self._ctrlC_handler)
        
    #start the application. Blocking call.
    #@return program exit code (0 is ok, other is error)
    def run(self):
        self.mainWindow.show()
        return self.exec_()
    
    def _ctrlC_handler(self, signum, frame):
        #This call request pyqt to quit the application, it will requires the next event loop to execute
        #Note that cleanup code is in the aboutToQuit() method
        print("Ctrl+C stroked")
        self.quit()
        
    def _quitHandler(self):
        print("Application quit handler")
        
        #Save persisted settings
        self.mainWindow.writeSettings()
 
if __name__ == '__main__':
    import os
    
    # re-generate proto (not optimal, but as they will change a lot at project beginning...)
    # os.system("..\com\generateCom.bat .. off")
    
    # Build application
    app = VizuApplication()

    #A timer is requested to let pyqt interpreter give hand to python on Ctr+c
    #We miss a standard pyqt solution here IMO
    #see http://stackoverflow.com/questions/4938723/what-is-the-correct-way-to-make-my-pyqt-application-quit-when-killed-from-the-co/4939113#4939113
    timer = QTimer()
    timer.start(250)  # You may change this if you wish.
    timer.timeout.connect(lambda: None)  # Let the interpreter run each 500 ms.

    #Run
    res = app.run()
    
    #Quit
    print("Application quit with code : " + str(res))
    sys.exit(res)
    
    
    
    
