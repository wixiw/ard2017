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
        
             
class TabStratAPI(QWidget):
    def __init__(self, parent):
        super().__init__(parent)
        layout = QVBoxLayout(self)
        
        box_nav = QGroupBox("Navigation")
        nav_layout = QVBoxLayout()
        box_nav.setLayout(nav_layout)
        layout.addWidget(box_nav)
        nav_layout.addWidget(QLabel("<b>robot.nav.setPosition(x,y,h)</b> : force a new robot position."))
        nav_layout.addWidget(QLabel("<b>robot.nav.goTo(x,y,[dir])</b> : move to target point, function doesn't block until position is reached"))
        nav_layout.addWidget(QLabel("<b>robot.nav.goToCap(x,y,h,[dir])</b> : move to target point and heading, function doesn't block until position is reached"))
        nav_layout.addWidget(QLabel("<b>robot.nav.wait()</b> : wait until current motion command is executed"))
        
        box_stratInfo = QGroupBox("Strat info")
        stratInfo_layout = QVBoxLayout()
        box_stratInfo.setLayout(stratInfo_layout)
        layout.addWidget(box_stratInfo)
        stratInfo_layout.addWidget(QLabel("<b>robot.strategy.informTaken_XXX()</b> : inform robot that we took an element from table (replace XXX by the cylinder name)."))
        stratInfo_layout.addWidget(QLabel("<b>robot.strategy.informPushedAway_XXX()</b> : inform robot that we pushed an element from its default position (replace XXX by the cylinder name)."))
        stratInfo_layout.addWidget(QLabel("<b>robot.strategy.informWithdraw_XXX(nb)</b> : inform robot that we took elements from a dispenser (replace XXX by dispenser name)."))
        stratInfo_layout.addWidget(QLabel("<b>robot.strategy.informPooed_XXX(nb)</b> : inform robot that we pooed an element (replace XXX by container name)."))
        
        box_HMI = QGroupBox("HMI")
        HMI_layout = QVBoxLayout()
        box_HMI.setLayout(HMI_layout)
        layout.addWidget(box_HMI)
        HMI_layout.addWidget(QLabel("<b>robot.setRGBled(color, blink)</b> : drive the RGB led."))
        HMI_layout.addWidget(QLabel("<b>robot.setLed(led, blink)</b> : drive one of the 4 green led."))
        HMI_layout.addWidget(QLabel("<b>robot.buzzer.playTone(f, time)</b> : play a sound at defined frequency for a duraction."))
        HMI_layout.addWidget(QLabel("<b>robot.buzzer.XXX()</b> : play a predefined melody."))
        layout.addStretch()
        
class TabHelp(QWidget):
    def __init__(self, parent):
        super().__init__(parent)
        self.tab = dict()
        self.tab["Shortcuts"]   = TabShortcuts(self)
        self.tab["Strat API"]   = TabStratAPI(self)
        self.tab["Table plan"]  = ImageWidget(self, "img/plan_table.png")
        self.tab["Pen plan"]    = ImageWidget(self, "img/plan_pen.png")
        self.tab["Tration plan"]= ImageWidget(self, "img/plan_tration.png")
        
        #retrieve saved tab
        settings = QSettings("config.ini", QSettings.IniFormat)
        settings.beginGroup("TabId")
        lastTabId = int(settings.value("Help", 0))
        settings.endGroup()
        
        #register tabs
        self.tabs = QTabWidget(self)
        self.tabs.currentChanged.connect(self.tabChanged)
        for tabName, tab in self.tab.items():
            self.tabs.addTab(tab, tabName)
        self.tabs.setCurrentIndex(lastTabId)
        

        layout_main = QHBoxLayout(self)
        layout_main.addWidget(self.tabs)
        
    def setShortcuts(self, shortcuts):
        self.tab["Shortcuts"].setShortcuts(shortcuts)
            
    @pyqtSlot(int)
    def tabChanged(self, tabId):
        self.tabs.setCurrentIndex(tabId)
        settings = QSettings("config.ini", QSettings.IniFormat)
        settings.beginGroup("TabId")
        settings.setValue("Help", tabId)
        settings.endGroup()
        
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    widget = TabHelp()
    widget.show()
    sys.exit(app.exec_())