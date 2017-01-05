#!/usr/bin/python3
# -*- coding: utf-8 -*-

from com import *
from PyQt5.Qt import *
from PyQt5.QtWidgets import *

class TabCom(QWidget):
    serialConnected = pyqtSignal()
    
    #@param ArdSerial : a reference on the object managing the serial line
    def __init__(self, serialManager):
        super().__init__()
        self.com = serialManager
                     
        self.combo_COM = QComboBox(self)
        self.combo_COM.addItems(self.com.getAvailablePorts())
        #---DEBUG ---- pour simplifier la vie a cette feignasse de Lambert
        self.combo_COM.setCurrentIndex(1)

        self.combo_Baudrate = QComboBox(self)
        for baudrate in self.com.getAvailableBaudrates():
            self.combo_Baudrate.addItem(str(baudrate), baudrate)
        
        self.btn_connect = QPushButton('Connect', self)
        self.btn_connect.setCheckable(True)
        self.btn_connect.clicked[bool].connect(self._connectRequest)
        
        self.btn_getStats = QPushButton('Get Stats', self)
        self.btn_getStats.hide()
        self.btn_getStats.clicked[bool].connect(self._getStats) 
        
        layout = QVBoxLayout(self)
        layoutH1 = QHBoxLayout()
        layoutH2 = QHBoxLayout()
        layout.addLayout(layoutH1)
        layout.addLayout(layoutH2)
        layout.addStretch()
        
        layoutH1.addWidget(self.combo_COM)
        layoutH1.addWidget(self.combo_Baudrate)
        layoutH1.addWidget(self.btn_connect)      
        layoutH1.addStretch()
        
        layoutH2.addWidget(self.btn_getStats)
        layoutH2.addStretch()

    @pyqtSlot(bool)
    def _connectRequest(self, pressed):
        if pressed:
            self._connect()
        else:
            self._disconnect()
        
    @pyqtSlot(bool)
    def _getStats(self, pressed): 
       print("Stats request")
       self.serial.putChar("s")
       self.serial.flush()
        
    def _connect(self):
        port = self.combo_COM.currentText()
        baudrate = self.combo_Baudrate.currentData()
        print("Connection request on port " + port + " at a baudrate of " + str(baudrate))
        self.combo_COM.setEnabled(False)
        self.combo_Baudrate.setEnabled(False)
        self.btn_connect.setText("Disconnect")
        
        if self.com.connect(port, baudrate):
            print("Connected")
            self.btn_getStats.show()
            self.serialConnected.emit()
        else:
            disconnect()
            
    def _disconnect(self):
        self.com.disconnect()
        self.btn_connect.setText("Connect")
        self.btn_connect.setChecked(False)
        self.combo_COM.setEnabled(True)
        self.combo_Baudrate.setEnabled(True)
        self.btn_getStats.hide()
        print("Disconnected")
            
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    widget = TabCom()
    widget.show()
    sys.exit(app.exec_())