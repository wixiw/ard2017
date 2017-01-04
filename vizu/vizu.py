#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import *
from PyQt5.QtSerialPort import *

class ConnectScreen(QWidget):
    
    def __init__(self):
        super().__init__()
        self.initUI()
        
        
    def initUI(self):      
        self.serial = QSerialPort(self)
        self.serialPort = "COM3"
        self.serialBaudrate = 256000
        
        self.resize(250, 150)
        self.move(300, 300)
        self.setWindowTitle('8=> Vizu')     
              
        self.combo_COM = QComboBox(self)
        self.combo_COM.addItem("")
        for port in QSerialPortInfo.availablePorts():
            self.combo_COM.addItem(port.portName())
        self.combo_COM.move(10, 10)
        self.combo_COM.activated[str].connect(self.onCOMChanged)  
        #---DEBUG ----
        self.combo_COM.setCurrentIndex(2)

        self.combo_Baudrate = QComboBox(self)
        self.combo_Baudrate.addItem("", 0)
        for baudrate in QSerialPortInfo.standardBaudRates():
            self.combo_Baudrate.addItem(str(baudrate), baudrate)
        self.combo_Baudrate.addItem(str(250000), 250000)
        self.combo_Baudrate.move(80, 10)
        self.combo_Baudrate.activated[int].connect(self.onBaudrateChanged)  
        #---DEBUG ----
        self.combo_Baudrate.setCurrentIndex(16)
        
        self.btn_connect = QPushButton('Connect', self)
        #---DEBUG ---- self.btn_connect.setEnabled(False)
        self.btn_connect.setCheckable(True)
        self.btn_connect.move(150, 10)
        self.btn_connect.clicked[bool].connect(self.connectRequest)
        
        self.btn_getStats = QPushButton('Get Stats', self)
        self.btn_getStats.hide()
        self.btn_getStats.move(10, 50)
        self.btn_getStats.clicked[bool].connect(self.getStats)
           
    def onCOMChanged(self, text):
        if text != "":
            if self.serialBaudrate != None:
                self.btn_connect.setEnabled(True)
            print("COM port selected : " + text)  
        else:
            self.btn_connect.setEnabled(False)
            print("COM port unselected")  
        self.serialPort = text   
        
    def onBaudrateChanged(self, index):
        if index != 0:
            if self.serialPort != None:
                self.btn_connect.setEnabled(True)
            print("Baudrate selected : " + self.combo_Baudrate.itemText(index))  
        else:
            self.btn_connect.setEnabled(False)
            print("Baudrate unselected")  
        self.serialBaudrate = self.combo_Baudrate.itemData(index)      

    def connectRequest(self, pressed):
        if pressed:
            print("Connect request to " + self.serialPort)            
            if self.connect():
                print("Success")
            else:
                self.disconnect()
                print("Error")
        else:
            self.disconnect()
            print("Disconnected")
        
    def getStats(self, pressed): 
           print("Stats request")
           self.serial.putChar("s")
           self.serial.flush()
        
    def connect(self):
            self.combo_COM.setEnabled(False)
            self.combo_Baudrate.setEnabled(False)
            self.btn_connect.setText("Disconnect")
            
            #create serial connection
            self.serial.setPortName(self.serialPort)
            self.serial.setBaudRate(self.serialBaudrate)
            self.serial.setDataBits(8)
            self.serial.setParity(QSerialPort.NoParity)
            self.serial.setStopBits(QSerialPort.OneStop)
            self.serial.setFlowControl(QSerialPort.NoFlowControl)
            
            if self.serial.open(QSerialPort.ReadWrite):
                self.btn_getStats.show()
                return True
            else:
                return False
            
    def disconnect(self):
            self.serial.close()
            self.btn_connect.setText("Connect")
            self.btn_connect.setChecked(False)
            self.combo_COM.setEnabled(True)
            self.combo_Baudrate.setEnabled(True)
            self.btn_getStats.hide()
            
if __name__ == '__main__':
    
    app = QApplication(sys.argv)
    screen = ConnectScreen()
    screen.show()
    sys.exit(app.exec_())