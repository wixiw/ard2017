#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from ArdWidgets import *
from core import *

#
# This class is a pre-built widget which is designed to display 
# robot state related to onboard actuators/sensors.
#
class TabRobot_Cmds(QWidget):
    
    def __init__(self, parent):
        super().__init__(parent)
        
        self.sections = dict()

        #Create sections   
        self.sections["nav"] = NavigationTeleopWidget(self)
        self.sections["general"] = GeneralTeleopWidget(self)
        self.sections["sound"] = SoundTeleopWidget(self)
        
        #create layouts objects
        self.layout = QVBoxLayout(self)
        self.split1 = QHBoxLayout()
        self.layout.addWidget(self.sections["nav"])
        self.layout.addLayout(self.split1)
        self.layout.addStretch(1)
        
        self.split1.addWidget(self.sections["general"])
        self.split1.addWidget(self.sections["sound"])
        self.split1.addStretch(1)

        
      
        
class GeneralTeleopWidget(QWidget):    
    getOsStatsLogs = pyqtSignal()
    getComStatsLogs = pyqtSignal()
    getTelemetry = pyqtSignal()
    resetCpu = pyqtSignal()
                        
    def __init__(self, parent):
        super().__init__(parent)
        self.btn_cmds = dict()
        self.layout=dict()
        
        self.btn_cmds["getOsStats"] = QPushButton('Get OS Stats', self)
        self.btn_cmds["getOsStats"].clicked.connect(self._getOsStatsLogs) 
        
        self.btn_cmds["getComStatsLogs"] = QPushButton('Get COM Stats', self)
        self.btn_cmds["getComStatsLogs"].clicked.connect(self._getComStatsLogs) 
        
        self.btn_cmds["getTelemetry"] = QPushButton('Get Telemetry', self)
        self.btn_cmds["getTelemetry"].clicked.connect(self._getTelemetry) 
        
        self.btn_cmds["resetCpu"] = QPushButton('Reset CPU', self)
        self.btn_cmds["resetCpu"].clicked.connect(self._resetCpu) 
        
        self.layout["Commands"] = QVBoxLayout(self)
        for button in self.btn_cmds:    
            self.layout["Commands"].addWidget(self.btn_cmds[button])
        self.layout["Commands"].addStretch()
        
        self.box = QGroupBox("General")
        self.box.setLayout(self.layout["Commands"])
        
        self.layout = QHBoxLayout(self)
        self.layout.addWidget(self.box)
        self.layout.addStretch()
        
    @pyqtSlot()
    def _getOsStatsLogs(self): 
       print("Stats request")
       self.getOsStatsLogs.emit()
       
    @pyqtSlot()
    def _getComStatsLogs(self): 
       print("Com Stats request")
       self.getComStatsLogs.emit()
       
    @pyqtSlot()
    def _getTelemetry(self): 
       print("Telemetry request")
       self.getTelemetry.emit()
       
    @pyqtSlot()
    def _resetCpu(self): 
       print("Reset CPU request")
       self.resetCpu.emit()
   
class NavigationTeleopWidget(QWidget): 
    blocked = pyqtSignal(bool)
                          
    def __init__(self, parent):
        super().__init__(parent)
        self.navTab = dict()
        self.layout=dict()
        
        self.navTab["setPosition"] = SetPosForm(self)
        self.navTab["setSpeedAcc"] = SetSpeedAccForm(self)
        self.navTab["gotoCap"] = GotoCapForm(self)
        self.navTab["goto"] = GotoForm(self)
        self.navTab["goForward"] = GoForwardForm(self)
        self.navTab["turnDelta"] = TurnDeltaForm(self)
        self.navTab["turnTo"] = TurnToForm(self)
        self.navTab["faceTo"] = FaceToForm(self)
        self.navCombo = QComboBox(self)
        for tabName, tab in self.navTab.items():
            self.navCombo.addItem(tabName, tab)
        self.navCombo.setEditable(True)
        self.navCombo.lineEdit().setReadOnly(True)
        self.navCombo.lineEdit().setAlignment(Qt.AlignHCenter)
        self.navCombo.highlighted[int].connect(self._navCmdChanged)
        self.navCombo.currentIndexChanged[int].connect(self._navCmdChanged)
        self.btn_nav = dict()
        self.btn_nav["Block"] = QPushButton('Block', self)
        self.btn_nav["Block"].toggled[bool].connect(self._blockFromButton)
        self.btn_nav["Block"].setCheckable(True)
        
        self.layout["NavOrder"] = QHBoxLayout(self)
        
        self.layout["NavOrder"].addWidget(self.navCombo)
        self.layout["NavStack"] = QStackedLayout()
        self.layout["NavOrder"].addLayout(self.layout["NavStack"])
        for tabName, tab in self.navTab.items():
            self.layout["NavStack"].addWidget(tab)
        self.layout["NavOrder"].addWidget(self.btn_nav["Block"])
        self.layout["NavOrder"].addStretch(1)
        
        self.box = QGroupBox("Navigation")
        self.box.setLayout(self.layout["NavOrder"])
        
        self.layoutH = QHBoxLayout(self)
        self.layoutH.addWidget(self.box)
        self.layoutH.addStretch()
        
    @pyqtSlot(int)
    def _navCmdChanged(self, comboId):
        self.navTab[self.navCombo.currentText()].reset()
        for name, widget in self.navTab.items():
            widget.reset()
        self.layout["NavStack"].setCurrentIndex(comboId)
        
    @pyqtSlot()
    def _blockFromShortcut(self): 
        self.btn_nav["Block"].toggle()
        
    @pyqtSlot(bool)
    def _blockFromButton(self, pressed): 
        if pressed:
            print("Block request") 
            self.btn_nav["Block"].setText("Unblock")
        else:
            print("Unblock request") 
            self.btn_nav["Block"].setText("Block")
        self.blocked.emit(pressed)
   
   
   
   
class SoundTeleopWidget(QWidget): 
    requestPlaySound = pyqtSignal(Melody)
    
    def __init__(self, parent):
        super().__init__(parent)
        
        self.inputs = ToneWidget(self)
        self.inputs.toneRequest.connect(self._requestPlayTone)
        
        self.layoutBox = QVBoxLayout()
        self.layoutBox.addWidget(self.inputs)
        self.layoutBox.addStretch()
        
        self.box = QGroupBox("Sound")
        self.box.setLayout(self.layoutBox)
        
        self.layout = QHBoxLayout(self)
        self.layout.addWidget(self.box)
        self.layout.addStretch()
    
    @pyqtSlot(Tone, int)
    def _requestPlayTone(self, tone, counts):    
        print("Play tone request freq=" + str(tone.frequency) + "Hz duration=" + str(tone.duration) + "ms count=" + str(counts))
        melody = Melody()
        for i in range(counts):
            print("bite")
            melody.add(tone)
        self.requestPlaySound.emit(melody)
        
    
class SetPosForm(QWidget):
    execute = pyqtSignal(Pose2D)
                        
    def __init__(self, parent):
        super().__init__(parent)
        self.x = IntegerInput(self, -2000, 2000)
        self.y = IntegerInput(self, -1500, 1500)
        self.h = HeadingInput(self)
        self.exe = QPushButton('Execute', self)
        self.exe.clicked.connect(self._execute) 
        
        layout = QHBoxLayout(self)
        layoutForm = QFormLayout()
        layout.addLayout(layoutForm)
        layout.addWidget(self.exe)
        layoutForm.addRow("x (mm)", self.x)
        layoutForm.addRow("y (mm)", self.y)
        layoutForm.addRow("h (°)", self.h)
    
    def reset(self):
        self.x.clear()
        self.y.clear()
        self.h.clear()
        
    @pyqtSlot()
    def _execute(self):
        self.execute.emit(Pose2D(self.x.getValue(), self.y.getValue(), self.h.getValue()))
        
        
        
        
class SetSpeedAccForm(QWidget):
    execute = pyqtSignal(int, int, int, int)
                        
    def __init__(self, parent):
        super().__init__(parent)
        self.vMax = IntegerInput(self, 0, 2000)
        self.vMaxTurn = IntegerInput(self, 0, 1000)
        self.accMax = IntegerInput(self, 0, 2000)
        self.accMaxTurn = IntegerInput(self, 0, 2000)
        self.exe = QPushButton('Execute', self)
        self.exe.clicked.connect(self._execute) 
        
        layout = QHBoxLayout(self)
        layoutForm = QFormLayout()
        layout.addLayout(layoutForm)
        layout.addWidget(self.exe)
        layoutForm.addRow("vMax (mm/s)", self.vMax)
        layoutForm.addRow("vMaxTurn (°/s)", self.vMaxTurn)
        layoutForm.addRow("accMax (mm/s²)", self.accMax)
        layoutForm.addRow("accMaxTurn (°/s²)", self.accMaxTurn)
    
    def reset(self):
        self.vMax.clear()
        self.vMaxTurn.clear()
        self.accMax.clear()
        self.accMaxTurn.clear()
        
    @pyqtSlot()
    def _execute(self):
        self.execute.emit(self.vMax.getValue(), self.vMaxTurn.getValue(), self.accMax.getValue(), self.accMaxTurn.getValue())
            
            
            
class GotoCapForm(QWidget):
    execute = pyqtSignal(Pose2D, int)
                        
    def __init__(self, parent):
        super().__init__(parent)
        self.x = IntegerInput(self, -2000, 2000)
        self.y = IntegerInput(self, -1500, 1500)
        self.h = HeadingInput(self)
        self.dir = DirectionInput(self)
        self.exe = QPushButton('Execute', self)
        self.exe.clicked.connect(self._execute) 
        
        layout = QHBoxLayout(self)
        layoutForm = QFormLayout()
        layout.addLayout(layoutForm)
        layout.addWidget(self.exe)
        layoutForm.addRow("x (mm)", self.x)
        layoutForm.addRow("y (mm)", self.y)
        layoutForm.addRow("h (°)", self.h)
        layoutForm.addRow("dir", self.dir)
        
    def reset(self):
        self.x.clear()
        self.y.clear()
        self.h.clear()
        
    @pyqtSlot()
    def _execute(self):
        self.execute.emit(Pose2D(self.x.getValue(), 
                                 self.y.getValue(), 
                                 self.h.getValue()), 
                          self.dir.getValue())
        
class GotoForm(QWidget):
    #signal(point, dir)
    execute = pyqtSignal(Point, int)
    
    def __init__(self, parent):
        super().__init__(parent)
        self.x = IntegerInput(self, -2000, 2000)
        self.y = IntegerInput(self, -1500, 1500)
        self.dir = DirectionInput(self)
        self.exe = QPushButton('Execute', self)
        self.exe.clicked.connect(self._execute) 
        
        layout = QHBoxLayout(self)
        layoutForm = QFormLayout()
        layout.addLayout(layoutForm)
        layout.addWidget(self.exe)
        layoutForm.addRow("x (mm)", self.x)
        layoutForm.addRow("y (mm)", self.y)
        layoutForm.addRow("dir", self.dir)

    def reset(self):
        self.x.clear()
        self.y.clear()
        
    @pyqtSlot()
    def _execute(self):
        self.execute.emit(Point(self.x.getValue(), 
                                self.y.getValue()), 
                          self.dir.getValue())
        
class GoForwardForm(QWidget):
    execute = pyqtSignal(float)
                        
    def __init__(self, parent):
        super().__init__(parent)
        self.d = IntegerInput(self, -2000, 2000)
        self.exe = QPushButton('Execute', self)
        self.exe.clicked.connect(self._execute) 
        
        layout = QHBoxLayout(self)
        layoutForm = QFormLayout()
        layout.addLayout(layoutForm)
        layout.addWidget(self.exe)
        layoutForm.addRow("d (mm)", self.d)
    
    def reset(self):
        self.d.clear()
        
    @pyqtSlot()
    def _execute(self):
        self.execute.emit(self.d.getValue())
        
class TurnDeltaForm(QWidget):
    execute = pyqtSignal(float)
                        
    def __init__(self, parent):
        super().__init__(parent)
        self.dh = HeadingInput(self)
        self.exe = QPushButton('Execute', self)
        self.exe.clicked.connect(self._execute) 
        
        layout = QHBoxLayout(self)
        layoutForm = QFormLayout()
        layout.addLayout(layoutForm)
        layout.addWidget(self.exe)
        layoutForm.addRow("dh (°)", self.dh)
    
    def reset(self):
        self.dh.clear()
        
    @pyqtSlot()
    def _execute(self):
        self.execute.emit(self.dh.getValue())
        
class TurnToForm(QWidget):
    execute = pyqtSignal(float)
                        
    def __init__(self, parent):
        super().__init__(parent)
        self.h = HeadingInput(self)
        self.exe = QPushButton('Execute', self)
        self.exe.clicked.connect(self._execute) 
        
        layout = QHBoxLayout(self)
        layoutForm = QFormLayout()
        layout.addLayout(layoutForm)
        layout.addWidget(self.exe)
        layoutForm.addRow("h (°)", self.h)
    
    def reset(self):
        self.h.clear()
        
    @pyqtSlot()
    def _execute(self):
        self.execute.emit(self.h.getValue())
        
class FaceToForm(QWidget):
    execute = pyqtSignal(Point)
                        
    def __init__(self, parent):
        super().__init__(parent)
        self.x = IntegerInput(self, -2000, 2000)
        self.y = IntegerInput(self, -1500, 1500)
        self.exe = QPushButton('Execute', self)
        self.exe.clicked.connect(self._execute) 
        
        layout = QHBoxLayout(self)
        layoutForm = QFormLayout()
        layout.addLayout(layoutForm)
        layout.addWidget(self.exe)
        layoutForm.addRow("xt (mm)", self.x)
        layoutForm.addRow("yt (mm)", self.y)
    
    def reset(self):
        self.x.clear()
        self.y.clear()
        
    @pyqtSlot()
    def _execute(self):
        self.execute.emit(Point(self.x.getValue(), self.y.getValue()))