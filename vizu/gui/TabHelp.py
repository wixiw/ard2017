#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtWidgets import *

class TabShortcuts(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout(self)
        self.layoutForm = QFormLayout()
        layout.addLayout(self.layoutForm)
        
        self.addShortcut("ESC", "quits vizu")
        self.addShortcut("F1", "Switch to Com tab.")
        self.addShortcut("F2", "Switch to Logs tab.")
        self.addShortcut("F3", "Switch to Strategy tab.")
        self.addShortcut("F4", "Switch to Robot control tab.")
        self.addShortcut("F5", "Switch to Help tab.")
        self.addShortcut("y", "Configure a match with Alpha strategy and YELLOW color.")
        self.addShortcut("b", "Configure a match with Alpha strategy and BLUE color.")
        self.addShortcut("s", "Starts the match.")
        self.addShortcut("r", "Reset robot.")
        self.addShortcut("c", "{Com tab} : connects/disconnects to robot.")
        
        
    def addShortcut(self, shortcut, description):
        text = QLabel(description)
        self.layoutForm.addRow(shortcut, text)
   
class TabBuild(QWidget):
    def __init__(self):
        super().__init__()
             
class TabHelp(QWidget):
    def __init__(self):
        super().__init__()
        self.tab = dict()
        self.tab["Shortcuts"]   = TabShortcuts()
        self.tab["Build"]   = TabBuild()
        
        self.tabs = QTabWidget(self)
        for tabName, tab in self.tab.items():
            self.tabs.addTab(tab, tabName)
        layout_main = QHBoxLayout(self)
        layout_main.addWidget(self.tabs)
            
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    widget = TabHelp()
    widget.show()
    sys.exit(app.exec_())