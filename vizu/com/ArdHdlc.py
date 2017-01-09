#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
import yahdlc
from ArdSerial import * 

#
# This class manage is a framing layer over a serial link
# (framing means cutting a continous input octet stream into well delimited messages)
# It acts as a decorator of ArdSerials hence creating the level 2 OSI Layer (Data Link)
# HDLC may also acts as a Network level as room is present in the header for an address,
# but this feature is not used for ARD yet.
#
class ArdHdlc(QObject):
    #The QT signal emitted when a data is ready, automatically connected with the connect() method
    #(yeah many connect but different meanings, one is for QT the other for serial connection)
    _newFrame = pyqtSignal(bytes)
    
    #This is enforced by hdlc as sequence number are coded on 3 bits
    NB_SEQ_NUMBERS = 8
    
    def __init__(self):
        super().__init__() 
        self._physicalLayer = ArdSerial()
        self._reset()
    
    def _reset(self):
        self.sendSedNumber = 0
        self.recvSedNumber = 0
        self.recvBuffer = bytes()#buffer to catch hdlc frames
        self._connected = False

    
    # go throw decorator
    def getAvailablePorts(self):
        return self._physicalLayer.getAvailablePorts()
    
    # go throw decorator
    def getAvailableBaudrates(self):
        return self._physicalLayer.getAvailableBaudrates()
    
    # Connect to the specified port at the specified baudrate
    # @param str port : like "COM1", ideally from getAvailablePorts()
    # @param int baudrate : ideally from getAvailableBaudrates()
    # @param void slot(bytes) : the QT Slot to be connected to newFrame
    # @return bool : true if the connection succeed, false otherwise
    def connect(self, port, baudrate, slot = None):
        assert not self._connected
        res = self._physicalLayer.connect(port, baudrate, self._dataReceived)
        if slot != None:
            self._newFrame.connect(slot)
        self._connected = True
        return res
        
    # to be call after having connect() to close the line
    def disconnect(self): 
        assert self._connected
        self._physicalLayer.disconnect()
        try: #because an assert is generated if disconnect is called with no connection
            self._newFrame.disconnect() 
        except:
            print("Warning : ArdHdlc._newFrame has been disconnected with no existing connection.")
            pass #so there is no worry
        self._reset()

    # @return bool : True if connected
    def isConnected(self):
        return self._isConnected

    # Pass your serialized message in parameter
    # so that it's framed and sent on serial link
    # @return bool : True if message sent successfully
    def sendMsg(self, msg):
        assert self._connected
        # TODO sequence id is not used yet, as Teleop doesn't require any robustness, 
        # in order to use this com with a robot to robot com, it'll be required
        frame = yahdlc.frame_data(msg, yahdlc.FRAME_DATA, self.sendSedNumber)
        #---DEBUG---- print("HLDC frame : 0x[%s]" % frame.hex())
        self.sendSedNumber = (self.sendSedNumber + 1) % self.NB_SEQ_NUMBERS
        return self._physicalLayer.write(frame) == len(frame)

    # a QT signal is sent by ArdSerial in this slot when data are ready to be read
    @pyqtSlot()
    def _dataReceived(self):
        assert self._connected
            #---DEBUG--- print("ArdHdlc received data")
        self.recvBuffer += self._physicalLayer.readAll()
        
        #YAHDLC can't manage buffer that are bigger than 520o :
        if 512 + 8 < len(self.recvBuffer):
            print("Error : receive buffer overshoot, data lost.")
            self.recvBuffer = self.recvBuffer[-520:] #it means : keep the last 520 bytes 

        #read input buffer until it's cleared
        bytesToRead = 666
        while 0 < bytesToRead:
            bytesToRead = self._readOneHdlcFrame()
                
            
    #read one hdlc frame and delete read data from self.recvBuffer
    #@return int : remaining bytes to read (you should call the function once again), or 0 in case of error
    def _readOneHdlcFrame(self):
                # Try to decode the data recevied from the serial line
        try:
            data, type, seq_no = yahdlc.get_data(self.recvBuffer)
        
        #the message is not complete, wait next data to retry to parse the buffer, it's a normal condition (and a bad yahdlc python binding design ...)
        except yahdlc.MessageError:
            return 00
        
        #checksum failed, message is ignored
        except yahdlc.FCSError:
            print("yahdlc error in decoding frame : checksum failed.")
            #print("hdlc type=" + str(type) + " seq=" + str(seq_no) + " frame=[" + str(data) + "]") 
            #TODO : clean the recv buffer and adapt the return code
            return 0
            
        #unknown error, this is really bad. Input buffer is cleaned
        except:
            print("yahdlc error in decoding frame : unknown error, received buffer reset, possible data loss.")
            self.recvBuffer = bytes()
            yahdlc.get_data_reset()
            print("--- SIZE --- : " + str(len(self.recvBuffer)))
            return 0
            
        #this is the normal case where an hdlc frame has been successfully decoded
        else:
                #---DEBUG--- print(" XXXX Frame decoded success !!!! XXXX")
            #as yahdlc python bindings hide some data we are force to use DYI a lot ...
            #recompose the raw frame (decause some escape char may provoke differences in in and out buffers ...)
            originalData = yahdlc.frame_data(data, type, seq_no)
                #---DEBUG--- print("originalData : " + str(originalData))
                #---DEBUG--- print("before : " + str(self.recvBuffer))
                #---DEBUG--- print("recv len=" + str(len(self.recvBuffer)) + " find at index : " + str(self.recvBuffer.find(originalData)) + " data len=" + str(len(originalData)))
            #find in the input buf for the 'decoded original frame'
            #remove read data before the 'decoded original frame' and the decoded original frame' from the input buffer 
            nbBytesUsed = self.recvBuffer.find(originalData) + len(originalData)
            unreadBytes = len(self.recvBuffer) - nbBytesUsed
            self.recvBuffer = self.recvBuffer[nbBytesUsed:]
            yahdlc.get_data_reset()
                #---DEBUG--- print(" after : " + str(self.recvBuffer))
                #---DEBUG--- print("hdlc type=" + str(type) + " seq=" + str(seq_no) + " frame=[" + str(data) + "]") 
            
            #finally, inform the listener a message has been received.
            if type == yahdlc.FRAME_DATA:
                self._newFrame.emit(data)
                
            return unreadBytes
        

