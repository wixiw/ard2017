#!/usr/bin/python3
# -*- coding: utf-8 -*-

#expand path to find modules :
import sys
import time
#Find the directory in which test is located
from os.path import dirname, abspath
DIR = dirname(abspath(__file__))

import signal

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *

class UT_ArdUart(QWidget):
    
    def __init__(self):
        super().__init__()             
        self.setWindowTitle('UT_ArdUart')
        layout = QVBoxLayout(self)
        self.label_lastCharSent = QLabel("xx")
        self.label_lastCharReceived = QLabel("xx")
        layout.addWidget(QLabel("Last char sent :"))
        layout.addWidget(self.label_lastCharSent)
        layout.addWidget(QLabel("Last char received :"))
        layout.addWidget(self.label_lastCharReceived)
        layout.addStretch()
        self.resize(QSize(300, 50))
        self.move(QPoint(300, 300))
        
        self._serial = QSerialPort()
        self._serial.setDataBits(8)
        self._serial.setParity(QSerialPort.EvenParity)
        self._serial.setStopBits(QSerialPort.OneStop)
        self._serial.setFlowControl(QSerialPort.NoFlowControl)
        assert self.connect("COM3", 250000, self._dataReceived), "Failed to connect to COM3, 250kbit/s"
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.tick)
        self.byte = 0
        self.timer.setSingleShot(True)
        self.timer.start(1) 
        self.lastReceived = 0
        
    @pyqtSlot()
    def tick(self):
        #self.byte = (self.byte + 1) % 255
        
        for self.byte in range(1, 256):
            data = bytearray(1)
            data[0] = self.byte
            self._serial.write(data)
            self.label_lastCharSent.setText(str(self.byte))
            print("S : " + str(self.byte))
        
    # Connect to the specified port at the specified baudrate
    # @param str port : like "COM1", ideally from getAvailablePorts()
    # @param int baudrate : ideally from getAvailableBaudrates()
    # @param void slot(void) : the callback to be called when data are available
    # @return bool : true if the connection succeed, false otherwise
    def connect(self, port, baudrate, slot = None):            
        self._serial.setPortName(port)
        self._serial.setBaudRate(baudrate)
        if slot != None:
            self._serial.readyRead.connect(slot)
        return self._serial.open(QSerialPort.ReadWrite)
        
    @pyqtSlot()
    def _dataReceived(self):
        #read serail line until no data is available        
        while self._serial.bytesAvailable():
            byte= (self._serial.read(1))[0]
            self.label_lastCharReceived.setText(str(byte)) 
            print("R : " + str(byte))
            
            if byte != 0:
                #0 is reserved and skipped
                if self.lastReceived == 255:
                    self.lastReceived = 0
                if self.lastReceived + 1 != byte:
                    print("Byte lost :-(")
                    self.timer.stop() 
                    self._serial.disconnect()
                    return
                else:
                    self.lastReceived = byte
                    if byte == 255:
                        self.timer.start()
            else:
                print("Reset confirmed")
                self.lastReceived = 0
        
if __name__ == '__main__':
    import os
    
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    
    app = QApplication(sys.argv)
    
    #Start application
    test = UT_ArdUart()
    test.show()
    
    res = app.exec_()
    sys.exit(res)


