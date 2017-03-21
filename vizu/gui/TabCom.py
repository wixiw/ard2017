#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtWidgets import *

class TabCom(QWidget):
    
    #QT emitted signals :
    networkStatus = pyqtSignal(bool)
    
    #@param ArdSerial : a reference on the object managing the serial line
    def __init__(self, comMdw):
        super().__init__()
        self.com = comMdw
        settings = QSettings("config.ini", QSettings.IniFormat)
        settings.beginGroup("Com")
        defaultPort = settings.value("port", "COM1")
        defaultBaudrate = settings.value("baudrate", "250000")
        ports, baudrates = self.com.getSerialPortInfo()
          
        #Serial port configuration
            #COM port selector
        self.combo_COM = QComboBox(self)
        self.combo_COM.addItems(ports)
        i = self.combo_COM.findText(defaultPort)
        if i != -1 :
            self.combo_COM.setCurrentIndex(i)
            #Baudrate selector
        self.combo_Baudrate = QComboBox(self)
        for baudrate in baudrates:
            self.combo_Baudrate.addItem(str(baudrate), baudrate)
        i = self.combo_Baudrate.findData(defaultBaudrate)
        if i != -1 :
            self.combo_Baudrate.setCurrentIndex(i)
            
            #connection button
        self.btn_connect = QPushButton('Connect', self)
        self.btn_connect.setCheckable(True)
        self.btn_connect.toggled[bool].connect(self._connectFromButton)
        
            #tests
        self.buttonsGroup = QWidget()
        self.buttonsGroup.setEnabled(False)
        self.btn_maxLength = QPushButton('Send max length test payload', self.buttonsGroup)
        self.btn_maxLength.clicked.connect(self._maxLength) 
        self.btn_crcErr = QPushButton('Send msg with erroneous CRC', self.buttonsGroup)
        self.btn_crcErr.clicked.connect(self._crcErr) 
        self.btn_tooLittle = QPushButton('Send too little errouneous msg', self.buttonsGroup)
        self.btn_tooLittle.clicked.connect(self._tooLittle) 
        self.btn_maxLengthReq = QPushButton('Receive max length test payload', self.buttonsGroup)
        self.btn_maxLengthReq.clicked.connect(self._maxLengthReq) 
        self.btn_crcErrReq = QPushButton('Receive msg with erroneous CRC', self.buttonsGroup)
        self.btn_crcErrReq.clicked.connect(self._crcErrReq) 
        self.btn_tooLittleReq = QPushButton('Receive too little errouneous msg', self.buttonsGroup)
        self.btn_tooLittleReq.clicked.connect(self._tooLittleReq) 
        
        layout    = QVBoxLayout(self)
        layoutH1  = QHBoxLayout()
        layoutH2  = QHBoxLayout()
        layout.addLayout(layoutH1)
        layout.addLayout(layoutH2)
        layout.addStretch()
        
        layoutH1.addWidget(self.combo_COM)
        layoutH1.addWidget(self.combo_Baudrate)
        layoutH1.addWidget(self.btn_connect)      
        layoutH1.addStretch()
        
        layoutH2.addWidget(self.buttonsGroup)
        layoutH2.addStretch()
        
        layoutBtnGroup = QVBoxLayout(self.buttonsGroup)
        layoutBtnGroup.addWidget(self.btn_maxLength)
        layoutBtnGroup.addWidget(self.btn_crcErr)
        layoutBtnGroup.addWidget(self.btn_tooLittle)
        layoutBtnGroup.addWidget(self.btn_maxLengthReq)
        layoutBtnGroup.addWidget(self.btn_crcErrReq)
        layoutBtnGroup.addWidget(self.btn_tooLittleReq)
        
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
        
    def _connect(self):
        port = self.combo_COM.currentText()
        baudrate = self.combo_Baudrate.currentData()
        print("Connection request on port " + port + " at a baudrate of " + str(baudrate))
        self.combo_COM.setEnabled(False)
        self.combo_Baudrate.setEnabled(False)
        self.btn_connect.setText("Disconnect")
        
        if self.com.connect(port, baudrate):
            print("Connected")
            settings = QSettings("config.ini", QSettings.IniFormat)
            settings.beginGroup("Com")
            settings.setValue("port", port)
            settings.setValue("baudrate", baudrate)
            self.buttonsGroup.setEnabled(True)
            self.networkStatus.emit(True)
        else:
            print("ERROR : Connection failed, check that the device is connected to the right port, and that nothing is holding the COM PORT (like another vizu instance...)")
            self.btn_connect.setText("Connect")
            self.btn_connect.setChecked(False)
            self.combo_COM.setEnabled(True)
            self.combo_Baudrate.setEnabled(True)
            self.combo_COM.removeItem(self.combo_COM.findText(port))
            self._updateComInfo()
            
    def _disconnect(self):
        self.com.disconnect()
        self.btn_connect.setText("Connect")
        self.btn_connect.setChecked(False)
        self.combo_COM.setEnabled(True)
        self.combo_Baudrate.setEnabled(True)
        self.buttonsGroup.setEnabled(False)
        self.networkStatus.emit(False)
        self._updateComInfo()
        print("Disconnected")
        
    def _updateComInfo(self):
        ports, baudrates = self.com.getSerialPortInfo()
        for port in ports:
            i = self.combo_COM.findText(port)
            if i == -1 :
                self.combo_COM.addItem(port)
        
    @pyqtSlot()
    def _maxLength(self): 
       print("Send a msg with the heaviest payload.")
       #Due to 4 character escapes, it is not possible to send 512 chars. 
       #Calcul is 512 (max) - 6(header) - 4 (escapes) = 256+246
       msgMax = bytes(range(256))
       msgMax += bytes(range(246))
       self.com.com.sendMsg(msgMax)
       
    @pyqtSlot()
    def _crcErr(self): 
       print("Send message with erroneous CRC")
       self.com.com._physicalLayer.write(b"~\ff\10\06\e1~")
       
    @pyqtSlot()
    def _tooLittle(self): 
       print("Send a too little msg")
       self.com.com._physicalLayer.write(b"~\ff\10~")
      
    @pyqtSlot()
    def _maxLengthReq(self): 
       print("Receive a msg with the heaviest payload.")
       self.com.requestMaxLengthMsg()
       
    @pyqtSlot()
    def _crcErrReq(self): 
       print("Receive message with erroneous CRC")
       self.com.requestCrcFailMsg()
       
    @pyqtSlot()
    def _tooLittleReq(self): 
       print("Receive a too little msg")
       self.com.requestTooLittleMsg()
            
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    widget = TabCom()
    widget.show()
    sys.exit(app.exec_())