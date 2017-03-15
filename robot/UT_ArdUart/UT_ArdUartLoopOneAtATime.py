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
        self.label_nbCharReceived = QLabel("0")
        self.label_nbCharLost = QLabel("0")
        self.label_nbPercentLoss = QLabel("0")
        layout.addWidget(QLabel("Last char sent :"))
        layout.addWidget(self.label_lastCharSent)
        layout.addWidget(QLabel("Last char received :"))
        layout.addWidget(self.label_lastCharReceived)
        layout.addWidget(QLabel("Bytes received :"))
        layout.addWidget(self.label_nbCharReceived)
        layout.addWidget(QLabel("Bytes lost :"))
        layout.addWidget(self.label_nbCharLost)
        layout.addWidget(QLabel("% loss :"))
        layout.addWidget(self.label_nbPercentLoss)
        layout.addStretch()
        self.resize(QSize(300, 50))
        self.move(QPoint(300, 300))
        
        self._serial = QSerialPort()
        self._serial.setDataBits(8)
        self._serial.setParity(QSerialPort.EvenParity)
        self._serial.setStopBits(QSerialPort.OneStop)
        self._serial.setFlowControl(QSerialPort.NoFlowControl)
        self._serial.error.connect(self.handleError)
        assert self.connect("COM3", 250000, self._dataReceived), "Failed to connect to COM3, 250kbit/s"
        self.byte = 1
        self.tickDelay = 10
        QTimer.singleShot(self.tickDelay, self.tick)
        self.lastReceived = 0
        self.nbBytesReceived = 0
        self.nbBytesLost = 0
        self.timeoutTimer = QTimer()
        self.timeoutTimer.timeout.connect(self.tickTimeout)
        
    @pyqtSlot()
    def tick(self):
        data = bytearray(1)
        data[0] = self.byte
        self._serial.write(data)
        self.label_lastCharSent.setText(str(self.byte))
        print("S : " + str(self.byte))
        #Arm a timer to detect if no bytes if received in time  
        self.timeoutTimer.start(5000)
       
    @pyqtSlot()
    def tickTimeout(self):
        self.timeoutTimer.stop()
        print("Tick timeout re-sending")
        self.nbBytesLost += 1
        self.label_nbCharLost.setText(str(self.nbBytesLost))
        if self.nbBytesReceived != 0:
            self.label_nbPercentLoss.setText(str(100.*self.nbBytesLost/(self.nbBytesReceived)))
        QTimer.singleShot(self.tickDelay, self.tick)
        
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
        self.timeoutTimer.stop()
        #read serail line until no data is available        
        while self._serial.bytesAvailable():
            byte= (self._serial.read(1))[0]
            self.label_lastCharReceived.setText(str(byte)) 
            print("R : " + str(byte))
            self.nbBytesReceived += 1
            self.label_nbCharReceived.setText(str(self.nbBytesReceived))
            if self.nbBytesReceived != 0:
                self.label_nbPercentLoss.setText(str(100.*self.nbBytesLost/(self.nbBytesReceived)))
            
            if byte != self.byte:
                print("Byte lost :-(")
                self._serial.close()
            else:
                self.byte = (self.byte + 1) % 255 
                if self.byte == 0:
                    self.byte = 1
                QTimer.singleShot(self.tickDelay, self.tick)
                
    @pyqtSlot(QSerialPort.SerialPortError )  
    def handleError(self, error):
        if error != 0:
            print("SERIAL ERROR : [" + str(error) + "]")
            sys.exit(-1)
        
if __name__ == '__main__':
    import os
    
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    
    app = QApplication(sys.argv)
    
    #Start application
    test = UT_ArdUart()
    test.show()
    
    res = app.exec_()
    sys.exit(res)

