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
        
class TabRobot(QWidget):
    def __init__(self, parent):
        super().__init__(parent)
        self.tab = dict()
        self.tab["Commands"] = TabRobot_Cmds(self)
        self.tab["Status"]   = TabRobot_Status(self)
        self.tab["Servos"]   = TabRobot_Servos(self)
        
        self.tabs = QTabWidget(self)
        
        for tabName, tab in self.tab.items():
            self.tabs.addTab(tab, tabName)
        layout_main = QHBoxLayout(self)
        layout_main.addWidget(self.tabs)
            
        
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    widget = TabRobot()
    widget.show()
    sys.exit(app.exec_())
    
    
    
    