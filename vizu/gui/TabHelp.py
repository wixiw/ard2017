#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtWidgets import *
from ArdWidgets import ImageWidget

class TabShortcuts(QWidget):
    def __init__(self, parent):
        super().__init__(parent)
        layout = QVBoxLayout(self)
        self.layoutForm = QFormLayout()
        layout.addLayout(self.layoutForm)
        
    def setShortcuts(self, shortcutContexts):
        for key, context in shortcutContexts.items():
            text = QLabel(context.helpText)
            self.layoutForm.addRow(context.shortcut, text)
        
class TabBuild(QWidget):
    def __init__(self, parent):
        super().__init__(parent)
             
class TabHelp(QWidget):
    def __init__(self, parent):
        super().__init__(parent)
        self.tab = dict()
        self.tab["Shortcuts"]   = TabShortcuts(self)
        self.tab["Build"]       = TabBuild(self)
        self.tab["Table plan"]  = ImageWidget(self, "img/plan_table.png")
        self.tab["Pen plan"]    = ImageWidget(self, "img/plan_pen.png")
        self.tab["Tration plan"]= ImageWidget(self, "img/plan_tration.png")
        
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