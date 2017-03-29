#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtWidgets import *

class TabShortcuts(QWidget):
    def __init__(self, texts):
        super().__init__()
        layout = QVBoxLayout(self)
        self.layoutForm = QFormLayout()
        layout.addLayout(self.layoutForm)
        
        for key, text in texts.items():
            text = QLabel(description)
            self.layoutForm.addRow(shortcut, text)
        
class TabBuild(QWidget):
    def __init__(self):
        super().__init__()
             
class TabHelp(QWidget):
    def __init__(self, texts):
        super().__init__()
        self.tab = dict()
        self.tab["Shortcuts"]   = TabShortcuts(texts)
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