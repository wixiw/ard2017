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
        
    def setShortcuts(self, shortcutContexts):
        for key, context in shortcutContexts.items():
            text = QLabel(context.helpText)
            self.layoutForm.addRow(context.shortcut, text)
        
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
        
    def setShortcuts(self, shortcuts):
        self.tab["Shortcuts"].setShortcuts(shortcuts)
            
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    widget = TabHelp()
    widget.show()
    sys.exit(app.exec_())