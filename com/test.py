import ardHdlc
import ctypes


def encodeDecode(msg):
    #print("original msg = " + str(msg))
    ardHdlc.resetState()
    
    frame = ardHdlc.createDataFrame(msg, ardHdlc.FRAME_DATA, 0)
    #print("encoded frame = " + str(frame))
    
    data, inBuf, type, seq = ardHdlc.decodeFrame(frame)
    #print("type = " + str(type) + " seq = " + str(seq))
    #print("decode frame = " + str(data))
    #print(inBuf)
    if not msg == data: #and inBuf == b''
        print("encodeDecode: Error with msg = " + str(msg))
        return False
    else:
        return True
    
def testValidMessages():
    msgEmpty = b''
    msgSimple = b'1234'
    msgMax = bytes(range(256))
    msgMax += bytes(range(253))
    
    res = True
    res &= encodeDecode(msgEmpty)
    res &= encodeDecode(msgSimple)
    res &= encodeDecode(msgMax)
    return res

def test2ValidMessages():
    res = True
    msg1 = b'1234'
    msg2 = b'5678'
    msg3 = b'1289'
    frame1 = ardHdlc.createDataFrame(msg1, ardHdlc.FRAME_DATA, 0)
    frame2 = ardHdlc.createDataFrame(msg2, ardHdlc.FRAME_DATA, 0)
    frame3 = ardHdlc.createDataFrame(msg3, ardHdlc.FRAME_DATA, 0)
    
    inBuf = frame1 + frame2 + frame3
    
    #parse first message
    data, inBuf, type, seq = ardHdlc.decodeFrame(inBuf)
    res &= data == msg1
    res &= inBuf == (frame2 + frame3)
    #parse second message
    data, inBuf, type, seq = ardHdlc.decodeFrame(inBuf)
    res &= data == msg2
    res &= inBuf == frame3
    #parse third message
    data, inBuf, type, seq = ardHdlc.decodeFrame(inBuf)
    res &= data == msg3
    res &= inBuf == b''
    return res

def test2HalfMessages():
    res = True
    msg = b'123456789'
    frame = ardHdlc.createDataFrame(msg, ardHdlc.FRAME_DATA, 0)
    frame1 = frame[0:6]
    frame2 = frame[7:]
    
    #read first msg part
    try:
        data, inBuf, type, seq = ardHdlc.decodeFrame(frame1)
    except ardHdlc.NoMessage as e:
        pass
    else:
        print("testFCS should not succeed to parse msg")
        return False
    
    #read second msg part
    data, inBuf, type, seq = ardHdlc.decodeFrame(frame)
    res &= data == msg
    res &= inBuf == b''
    return res

def testFCS():
    fcsErrorFrame = b'~\xff\x10\x06\xe1~00000'
    try:
        data, inBuf, type, seq = ardHdlc.decodeFrame(fcsErrorFrame)
    except ardHdlc.FCSError as e:
        #print(e.buffer)
        return True
    except Exception as e: 
        print(str(e))
        return False
    else:
        print("testFCS should not succeed to parse msg")
        return False
    
def testTooShort():
    fcsErrorFrame = b'~\xff\x10\xe1~000'
    try:
        data, type, seq = ardHdlc.decodeFrame(fcsErrorFrame)
    except ardHdlc.TooShort as e:
        #print(e.buffer)
        return True
    except Exception as e: 
        print(str(e))
        return False

res = True
res &= testValidMessages()
res &= test2ValidMessages()
res &= test2HalfMessages()
res &= testFCS()
res &= testTooShort()

if res:
    print("---Test success ---")
else:
    print("---Test failed ---")