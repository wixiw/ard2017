#!/usr/bin/python3
# -*- coding: utf-8 -*-

import ardHdlc
import traceback
from PyQt5.Qt import *
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
    
    MAX_HDLC_SIZE = 512
    
    def __init__(self):
        super().__init__() 
        self._physicalLayer = ArdSerial()
        self._reset()
    
    def _reset(self):
        self.sendSedNumber = 0
        self.recvSedNumber = 0
        self.recvBuffer = bytes()#buffer to catch hdlc frames
        self._connected = False

    def getUnexpectedDisconnect(self):
        return self._physicalLayer.unexpectedDisconnection
    
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
        assert self._connected == False
        res = self._physicalLayer.connect(port, baudrate, self._dataReceived)
        if slot != None:
            self._newFrame.connect(slot)
        self._connected = True
        return res
        
    # to be call after having connect() to close the line
    def disconnect(self): 
        if self._connected:
            self._physicalLayer.disconnect()
            try: #because an assert is generated if disconnect is called with no connection
                self._newFrame.disconnect() 
            except TypeError:
                pass
            except:
                print("Warning : ArdHdlc._newFrame has been disconnected with no existing connection.")
                pass #so there is no worry
            self._reset()

    # @return bool : True if connected
    def isConnected(self):
        return self._connected

    # Pass your serialized message in parameter
    # so that it's framed and sent on serial link
    # @return bool : True if message sent successfully
    def sendMsg(self, msg):
        assert self._connected
        #---DEBUG---- print("msg to send : 0x[%s]" % msg.hex())
        # in order to use this com with a robot to robot com, it'll be required
        try:
            frame = ardHdlc.createDataFrame(msg, ardHdlc.FRAME_DATA, self.sendSedNumber)
        #destination buffer is too short
        except ardHdlc.TooShort:
            print("Destination buffer is too short : " + str(msg))
            return False
        
        #Input parameters are incorrect
        except ValueError:
            assert False
        #Unknown error
        except:
            assert False
            
        #---DEBUG---- print("HLDC frame (seq=" + str(self.sendSedNumber) + ") (len=" + str(len(frame)) + ") : 0x[%s]" % frame.hex())
        self.sendSedNumber = (self.sendSedNumber + 1) % self.NB_SEQ_NUMBERS
        bytesWritten = self._physicalLayer.write(frame)
        #---DEBUG---- print("Bytes written : " + str(bytesWritten))
        return bytesWritten == len(frame)

    # a QT signal is sent by ArdSerial in this slot when data are ready to be read
    @pyqtSlot()
    def _dataReceived(self):
        assert self._connected
        bytesToRead = 666
        
        #read serail line until no data is available        
        while self._physicalLayer.bytesAvailable(): 
            #print("-> Data available : " + str(self._physicalLayer.bytesAvailable()))
            assert len(self.recvBuffer) <= self.MAX_HDLC_SIZE, str(len(self.recvBuffer)) + "<=" + str(self.MAX_HDLC_SIZE)
            #print("-> Room for read : " + str(self.MAX_HDLC_SIZE - len(self.recvBuffer)))
            self.recvBuffer += self._physicalLayer.read(self.MAX_HDLC_SIZE - len(self.recvBuffer))
            bytesToRead = len(self.recvBuffer)
            #---DEBUG--- 
            #print("-> ArdHdlc received data " + str(bytesToRead) + " " + str(self.recvBuffer))
            
            #read input buffer until it's cleared
            while 0 < bytesToRead:
                bytesToRead = self._readOneHdlcFrame()
                #print("-> after parsing " + str(bytesToRead) + " " + str(self.recvBuffer))
                
        #print("-> quit "  + str(bytesToRead) + str(self.recvBuffer))
                
            
    #read one hdlc frame and delete read data from self.recvBuffer
    #@return int : remaining bytes to read (you should call the function once again), or 0 in case of error
    def _readOneHdlcFrame(self):
        #---DEBUG--- print("recv buf (" + str(len(self.recvBuffer)) + ")=" + str(self.recvBuffer)) 
        
        # Try to decode the data recevied from the serial line
        try:
            data, self.recvBuffer, type, seq_no = ardHdlc.decodeFrame(self.recvBuffer)
            #---DEBUG--- print("hdlc type=" + str(type) + " seq=" + str(seq_no) + " frame=[" + str(data) + "]")
            #finally, inform the listener a message has been received.
            if type == ardHdlc.FRAME_DATA:
                self._newFrame.emit(data) 
            return len(self.recvBuffer)
        
        #the message is not complete, wait next data to retry to parse the buffer, it's a normal condition (and a bad hdlc python binding design ...)
        except ardHdlc.NoMessage:
            #---DEBUG--- print("hdlc message decoding in progress.")
            return 0
        
        #a message has been detected but it is too short : msg to send to dustbin
        except ardHdlc.TooShort as e:
            print("hdlc error in decoding frame : msg too little.")
            self.recvBuffer = e.buffer
            return len(self.recvBuffer)
            
        #a message has been detected but with an erronous checksum : msg to send to dustbin
        except ardHdlc.FCSError as e:
            print("hdlc error in decoding frame : checksum failed.")
            self.recvBuffer = e.buffer
            return len(self.recvBuffer)
            
        #the input buffer is too long for the C wrapper
        except ValueError:
            assert False
            
        #unknown error, this is really bad. Input buffer is cleaned
        except Exception as e: 
            print(str(e))
            assert False

