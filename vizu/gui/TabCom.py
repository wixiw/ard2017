#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtWidgets import *

class TabCom(QWidget):
    
    #QT emitted signals :
    networkStatus = pyqtSignal(bool)
    getOsStats = pyqtSignal()
    configureMatch = pyqtSignal(int, int)
    startMatch = pyqtSignal()
    
    #@param ArdSerial : a reference on the object managing the serial line
    def __init__(self, comMdw):
        super().__init__()
        self.com = comMdw
        ports, baudrates = self.com.getSerialPortInfo()
        
        #Serial port configuration
        self.combo_COM = QComboBox(self)
        self.combo_COM.addItems(ports)
        #---DEBUG ---- pour simplifier la vie a cette feignasse de Lambert
        self.combo_COM.setCurrentIndex(1)
        self.combo_Baudrate = QComboBox(self)
        for baudrate in baudrates:
            self.combo_Baudrate.addItem(str(baudrate), baudrate)
        
        self.btn_connect = QPushButton('Connect', self)
        self.btn_connect.setCheckable(True)
        self.btn_connect.toggled[bool].connect(self._connectFromButton)
        
        self.connected_btn = dict()
        self.connected_btn["getOsStats"] = QPushButton('Get Stats', self)
        self.connected_btn["getOsStats"].hide()
        self.connected_btn["getOsStats"].clicked.connect(self._getOsStats) 
        
        self.connected_btn["configureMatch"] = QPushButton('Configure Match', self)
        self.connected_btn["configureMatch"].hide()
        self.connected_btn["configureMatch"].clicked.connect(self._configureMatch) 
        
        self.connected_btn["startMatch"] = QPushButton('Start Match', self)
        self.connected_btn["startMatch"].hide()
        self.connected_btn["startMatch"].clicked.connect(self._startMatch) 
        
        layout = QVBoxLayout(self)
        layoutH1 = QHBoxLayout()
        layoutH2 = QVBoxLayout()
        layout.addLayout(layoutH1)
        layout.addLayout(layoutH2)
        layout.addStretch()
        
        layoutH1.addWidget(self.combo_COM)
        layoutH1.addWidget(self.combo_Baudrate)
        layoutH1.addWidget(self.btn_connect)      
        layoutH1.addStretch()
        
        for button in self.connected_btn:    
            layoutH2.addWidget(self.connected_btn[button])
        layoutH2.addStretch()
        
        #keyboard shortcuts
        QShortcut(QKeySequence(Qt.Key_C), self).activated.connect(self._connectFromShorcut)

    @pyqtSlot(bool)
    def _connectFromButton(self, pressed):
        if pressed:
            self._connect()
        else:
            self._disconnect()
        
    def _connectFromShorcut(self):
        self.btn_connect.toggle()            
        
    @pyqtSlot()
    def _getOsStats(self): 
       print("Stats request")
       self.getOsStats.emit()
       
    @pyqtSlot()
    def _configureMatch(self): 
       print("Configure match request")
       self.configureMatch.emit(1, 2)
       
    @pyqtSlot()
    def _startMatch(self): 
       print("Start match request")
       self.startMatch.emit()
        
    def _connect(self):
        port = self.combo_COM.currentText()
        baudrate = self.combo_Baudrate.currentData()
        print("Connection request on port " + port + " at a baudrate of " + str(baudrate))
        self.combo_COM.setEnabled(False)
        self.combo_Baudrate.setEnabled(False)
        self.btn_connect.setText("Disconnect")
        
        if self.com.connect(port, baudrate):
            print("Connected")
            for button in self.connected_btn:
                self.connected_btn[button].show()
            self.networkStatus.emit(True)
        else:
            print("Connection error")
            self.btn_connect.setText("Connect")
            self.btn_connect.setChecked(False)
            self.combo_COM.setEnabled(True)
            self.combo_Baudrate.setEnabled(True)
            for button in self.connected_btn:
                self.connected_btn[button].hide()
            
    def _disconnect(self):
        self.com.disconnect()
        self.btn_connect.setText("Connect")
        self.btn_connect.setChecked(False)
        self.combo_COM.setEnabled(True)
        self.combo_Baudrate.setEnabled(True)
        for button in self.connected_btn:
            self.connected_btn[button].hide()
        self.networkStatus.emit(False)
        print("Disconnected")
            
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    widget = TabCom()
    widget.show()
    sys.exit(app.exec_())