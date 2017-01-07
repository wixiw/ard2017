#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.QtSerialPort import *

#
# This class manage a serial connection
# It acts as a decorator of QSerialPort in order to protect and simplify its API
# When the class is killed, the serial is automatically closed
#
class ArdSerial():
    
    def __init__(self):
        self._serial = QSerialPort()
        self._serial.setDataBits(8)
        self._serial.setParity(QSerialPort.NoParity)
        self._serial.setStopBits(QSerialPort.OneStop)
        self._serial.setFlowControl(QSerialPort.NoFlowControl)
        
    # Register a listener to be triggered when data is available
    # @param the callback to register
    def registerListener(self, callback):
        self._serial.readyRead.connect(callback)
        
    # Get the list of physically connected ports
    # @return list[str]
    def getAvailablePorts(self):
        plist = list()
        for port in QSerialPortInfo.availablePorts():
            plist.append(port.portName())
        return plist

    # Get the list of available baudrates
    # @return list[str]
    def getAvailableBaudrates(self):
        blist = QSerialPortInfo.standardBaudRates()
        blist.insert(0,250000)
        return blist
    
    # Connect to the specified port at the specified baudrate
    # @param str port : like "COM1", ideally from getAvailablePorts()
    # @param int baudrate ideally from getAvailableBaudrates()
    # @return bool : true if the connection succeed, false otherwise
    def connect(self, port, baudrate):            
        self._serial.setPortName(port)
        self._serial.setBaudRate(baudrate)
        return self._serial.open(QSerialPort.ReadWrite)
            
    # to be call after having connect() to close the line()
    def disconnect(self): 
        self._serial.close()
        
    # Read all available data
    # @return QByteArray
    def readAll(self):
        return bytes(self._serial.readAll())
    
    # Write a bunch of data
    # @param str : the data to send on the serial line
    # @return int : nb bytes written
    def write(self, data):
        return self._serial.writeData(data)
            
if __name__ == '__main__':
    import sys
    print("Testing ArdSerial")
    
    #build object
    com = ArdSerial()
    
    #display config
    print(com.getAvailablePorts())
    print(com.getAvailableBaudrates())
    
    #play with it
    if com.connect("COM3", 250000):
        print("connected")
    else:
        print("connection failed")
    if com.connect("COM3", 250000):
        print("connected")
    else:
        print("connection failed")
    com.disconnect()
    print("disconnected")
    
    sys.exit()