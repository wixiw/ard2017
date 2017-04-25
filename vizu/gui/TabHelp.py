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
        nav_layout.addWidget(QLabel("<b>enableAvoidance</b> : activates avoidance sensors or not."))
        nav_layout.addWidget(QLabel("<b>setPosition(x,y,h)</b> : force a new robot position."))
        nav_layout.addWidget(QLabel("<b>goTo(x,y,[dir])</b> : move to target point"))
        nav_layout.addWidget(QLabel("<b>goToCap(x,y,h,[dir])</b> : move to target point and heading"))
        nav_layout.addWidget(QLabel("<b>goForward(dist)</b> : move 'dist' mm ahead (positive) / behind (negative)"))
        nav_layout.addWidget(QLabel("<b>turnDelta(dh)</b> : turn relatively of 'dh' degrees"))
        nav_layout.addWidget(QLabel("<b>turnTo(h)</b> : turn to target heading"))
        nav_layout.addWidget(QLabel("<b>faceTo(x,y)</b> : face to the target point (rotation only)"))
        nav_layout.addWidget(QLabel("<b>recalFace(border)</b> : Use front robot face to recal position on border"))
        nav_layout.addWidget(QLabel("<b>recalRear(border)</b> : Use rear robot face to recal position on border"))
        nav_layout.addWidget(QLabel("<b>stopMoving()</b> : Stop any ongoing move"))
        nav_layout.addWidget(QLabel("<b>targetReached()</b> : is true when target is reached"))
        
        box_act = QGroupBox("Actuators")
        act_layout = QVBoxLayout()
        box_act.setLayout(act_layout)
        layout.addWidget(box_act)
        act_layout.addWidget(QLabel("<b>swallow(nbCylinders)</b> : open arms and turn whell to swallow."))
        act_layout.addWidget(QLabel("<b>retractArms(nbCylinders)</b> : retract arm inside robot to move safely on table."))
        act_layout.addWidget(QLabel("<b>withdraw(nbCylinders)</b> : take cylinders from a dispenser."))
        act_layout.addWidget(QLabel("<b>poo(nbCylinders)</b> : eject some cylinders."))
        act_layout.addWidget(QLabel("<b>disableActuators()</b> : stop sending commands to actuators (no torque)."))
        
#         box_stratInfo = QGroupBox("Strat info")
#         stratInfo_layout = QVBoxLayout()
#         box_stratInfo.setLayout(stratInfo_layout)
#         layout.addWidget(box_stratInfo)
#         stratInfo_layout.addWidget(QLabel("<b>robot.strategy.informTaken_XXX()</b> : inform robot that we took an element from table (replace XXX by the cylinder name)."))
#         stratInfo_layout.addWidget(QLabel("<b>robot.strategy.informPushedAway_XXX()</b> : inform robot that we pushed an element from its default position (replace XXX by the cylinder name)."))
#         stratInfo_layout.addWidget(QLabel("<b>robot.strategy.informWithdraw_XXX(nb)</b> : inform robot that we took elements from a dispenser (replace XXX by dispenser name)."))
#         stratInfo_layout.addWidget(QLabel("<b>robot.strategy.informPooed_XXX(nb)</b> : inform robot that we pooed an element (replace XXX by container name)."))
#         
#         box_HMI = QGroupBox("HMI")
#         HMI_layout = QVBoxLayout()
#         box_HMI.setLayout(HMI_layout)
#         layout.addWidget(box_HMI)
#         HMI_layout.addWidget(QLabel("<b>beep(nb)</b> : play 'nb' bips."))
#         HMI_layout.addWidget(QLabel("<b>led1(blink)</b> : drive green led 1."))
#         HMI_layout.addWidget(QLabel("<b>led2(blink)</b> : drive green led 2."))
#         HMI_layout.addWidget(QLabel("<b>led3(blink)</b> : drive green led 3."))
#         HMI_layout.addWidget(QLabel("<b>led4(blink)</b> : drive green led 4."))
#         HMI_layout.addWidget(QLabel("<b>ledRGB(color, blink)</b> : drive RGB led (see HMI.h)."))
#         layout.addStretch()
        
class TabHelp(QWidget):
    def __init__(self, parent):
        super().__init__(parent)
        self.tab = dict()
        self.tab["Shortcuts"]   = TabShortcuts(self)
        self.tab["Strat API"]   = TabStratAPI(self)
        self.tab["Table plan"]  = ImageWidget(self, "img/plan_table.png")
        self.tab["Robot plan"]    = ImageWidget(self, "img/plan_pen.png")
        self.tab["Elements ID"]= ImageWidget(self, "img/idElements.png")
        
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
    