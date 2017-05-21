#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from ArdWidgets import *
from core import *

from TabRobot_Cmds import TabRobot_Cmds
from TabRobot_Status import TabRobot_Status
from TabRobot_Servos import TabRobot_Servos
from TabRobot_Calib import TabRobot_Calib
from TabRobot_Actuators import TabRobot_Actuators
        
class TabRobot(QWidget):
    def __init__(self, parent):
        super().__init__(parent)
        self.tab = dict()
        self.tab["Commands"] = TabRobot_Cmds(self)
        self.tab["Calib"]   = TabRobot_Calib(self)
        self.tab["Actuators"] = TabRobot_Actuators(self)
        self.tab["Status"]   = TabRobot_Status(self)
        self.tab["Servos"]   = TabRobot_Servos(self)
        
        #retrieve saved tab
        settings = QSettings("config.ini", QSettings.IniFormat)
        settings.beginGroup("TabId")
        lastTabId = int(settings.value("Robot", 0))
        settings.endGroup()
        
        #register tabs
        self.tabs = QTabWidget(self)
        self.tabs.currentChanged.connect(self.tabChanged)
        for tabName, tab in self.tab.items():
            self.tabs.addTab(tab, tabName)
        self.tabs.setCurrentIndex(lastTabId)
            
        layout_main = QHBoxLayout(self)
        layout_main.addWidget(self.tabs)
       
    @pyqtSlot(int)
    def tabChanged(self, tabId):
        self.tabs.setCurrentIndex(tabId)
        settings = QSettings("config.ini", QSettings.IniFormat)
        settings.beginGroup("TabId")
        settings.setValue("Robot", tabId)
        settings.endGroup()
             
        
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    widget = TabRobot()
    widget.show()
    sys.exit(app.exec_())
    
    
    
    