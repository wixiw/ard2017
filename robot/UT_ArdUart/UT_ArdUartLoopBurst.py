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
        self.label_nbMagic = QLabel("0")
        self.label_nbPercentMagic = QLabel("0")
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
        layout.addWidget(QLabel("Magic bytes :"))
        layout.addWidget(self.label_nbMagic)
        layout.addWidget(QLabel("% magic :"))
        layout.addWidget(self.label_nbPercentMagic)
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
        self.nextAwaitedByte = 1
        self.tickDelay = 10
        QTimer.singleShot(self.tickDelay, self.tick)
        self.lastReceived = 0
        self.nbBytesReceived = 0
        self.burstMaxValue = 255
        self.nbBytesLost = 0
        self.nbMagic = 0
        self.timeoutTimer = QTimer()
        self.timeout = 5000
        self.timeoutTimer.timeout.connect(self.tickTimeout)
        #0 idle, 1 sending data, 2 waiting data, 3 receiving, 4 error
        print("INIT")
        self.state = 0
        self.magic = False
        
    @pyqtSlot()
    def tick(self):
        if self.state != 0:
            print("tick:: state unexpected " + str(self.state))
            self._serial.close()
            assert(False)
        self.state = 1
        for i in range(1, self.burstMaxValue+1): 
            data = bytearray(1)
            data[0] = i
            self._serial.write(data)
            self.label_lastCharSent.setText(str(i))
            print("S : " + str(i))
        #Arm a timer to detect if no bytes if received in time  
        self._serial.flush()
        assert(self._serial.bytesToWrite() == 0)
        #self.timeoutTimer.start(self.timeout)
        self.state = 2
    
    @pyqtSlot()
    def tickTimeout(self):
        if self.state != 2 and self.state != 3:
            print("tickTimeout:: state unexpected " + str(self.state))
            self._serial.close()
            assert(False)
        self.state = 4
        self.timeoutTimer.stop()
        print("Tick timeout re-sending")
        
        if(self._serial.bytesAvailable()):
            print("unread bytes pending " + self._serial.bytesAvailable())
            self._serial.close()
            assert(False)
        self.nbBytesLost += 1
        self.label_nbCharLost.setText(str(self.nbBytesLost))
        if self.nbBytesReceived != 0:
            self.label_nbPercentLoss.setText(str(100.*self.nbBytesLost/(self.nbBytesReceived)))
        QTimer.singleShot(self.tickDelay, self.tick)
        print("End of timeout")
        self.state = 0
        
    # Connect to the specified port at the specified baudrate
    # @param str port : like "COM1", ideally from getAvailablePorts()
    # @param int baudrate : ideally from getAvailableBaudrates()
    # @param void slot(void) : the callback to be called when data are available
    # @return bool : true if the connection succeed, False otherwise
    def connect(self, port, baudrate, slot = None):            
        self._serial.setPortName(port)
        self._serial.setBaudRate(baudrate)
        if slot != None:
            self._serial.readyRead.connect(slot)
        res = self._serial.open(QSerialPort.ReadWrite)
        if res:
            self._serial.clear()
        return res
        
    @pyqtSlot()
    def _dataReceived(self):
        if self.state == 1:
            #sending in progress keep waiting
            return
        
        if self.state != 2 and self.state != 3:
            print("_dataReceived:: state unexpected " + str(self.state))
            self._serial.close()
            assert(False)
        self.state = 3
        #read serail line until no data is available        
        while self._serial.bytesAvailable():
            data = bytes(self._serial.readAll())
            print("Nb bytes received : " + str(len(data)))
            for i in range(0,len(data)):
                byte = data[i]
                self.label_lastCharReceived.setText(str(byte)) 
                print("R : " + str(byte))
                self.nbBytesReceived += 1
                self.label_nbCharReceived.setText(str(self.nbBytesReceived))
                
                if byte == 0:
                    print("Embedded error")
                    continue
                    self._serial.close()
                    assert(False)
                
                if byte != self.nextAwaitedByte:
                    
                    print("Byte lost :-( byte=" + str(byte) +" awaited=" + str(self.nextAwaitedByte) + " , magic = " + str(self.nextAwaitedByte - byte))
                    print("SERIAL ERROR : [" + str(self._serial.error()) + "]")
                    if self.magic == False:
                        print("Starting magic")
                        self.magic = True
                    self.nbMagic += 1
                    self.label_nbMagic.setText(str(self.nbMagic))
                    if self.nbBytesReceived != 0:
                        self.label_nbPercentMagic.setText(str(100.*self.nbMagic/(self.nbBytesReceived)))
                else:
                    if self.magic == True:
                        self.magic = False
                        print("Magic recovery")
                        
                self.nextAwaitedByte = (self.nextAwaitedByte + 1) % (self.burstMaxValue+1)
                if self.nextAwaitedByte == 0:
                    print("End of burst")
                    self.nextAwaitedByte = 1
                    self.state = 0
                    self.timeoutTimer.stop()
                    QTimer.singleShot(self.tickDelay, self.tick)
                
            
        
    @pyqtSlot(QSerialPort.SerialPortError )  
    def handleError(self, error):
        if error != 0:
            print("SERIAL ERROR : [" + str(self._serial.error()) + "] : " + self._serial.errorString());
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

