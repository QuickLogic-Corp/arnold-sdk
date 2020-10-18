#!/usr/bin/python
import linecache
import serial, time, sys
#initialization and open the port

#possible timeout values:
#    1. None: wait forever, block call
#    2. 0: non-blocking mode, return immediately
#    3. x, x is bigger than 0, float allowed, timeout block call
#usage hserial <COM2>

if (len(sys.argv) <  2):
    print ("Usage:", sys.argv[0], "[/dev/ttyUSB0], [115200], [logfile.txt]")
ser = serial.Serial()
ser.port = "/dev/ttyUSB0"
logfile = 'logfile.txt'
if (len(sys.argv) > 1 ) :
    ser.port = sys.argv[1] 
ser.baudrate = 115200
if (len(sys.argv) > 2 ) :
    ser.baudrate = sys.argv[2]
if (len(sys.argv) > 3):
    logfile = sys.argv[3]
ser.bytesize = serial.EIGHTBITS #number of bits per bytes
ser.parity = serial.PARITY_NONE #set parity check: no parity
ser.stopbits = serial.STOPBITS_ONE #number of stop bits
#ser.timeout = None          #block read
ser.timeout = 1            #block read for  
#ser.timeout = 2              #timeout block read
ser.xonxoff = False     #disable software flow control
ser.rtscts = False     #disable hardware (RTS/CTS) flow control
ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
ser.writeTimeout = 0     #timeout for write

try: 
   ser.open()

except Exception as e:
  print ("error open serial port: " + str(e))
  exit()

def PrintException():
  exc_type, exc_obj, tb = sys.exc_info()
  f = tb.tb_frame
  lineno = tb.tb_lineno
  filename = f.f_code.co_filename
  linecache.checkcache(filename)
  line = linecache.getline(filename, lineno, f.f_globals)
#  print 'Exception in ({}, Line {} "{}"): {}'.format(filename, lineno,line.strip(),exc_obj)
       
if ser.isOpen():
  try:
    ser.flushInput() #flush input buffer, discarding all its contents
    ser.flushOutput()#flush output buffer, aborting current output
    print ("Ready to Download Bitstream")
    log = open(logfile,'w') # open file for logging overwite
    print ("Logging to", logfile)
    while True :
        try:
            x = ser.read_until(b'\n')
            if x[0:3] == b'ExIt' :
                log.close()
                exit()
            if len(x) > 0 :
                bitstream = x.decode().split()
                if bitstream[0] == "LoAd":
                    count = 0
                    ser.flushInput() #flush input buffer, discarding all its contents
                    ser.flushOutput() #flush output buffer, aborting current outp
                    bits = open('FourBytePattern.ArcticPro2.'+bitstream[1],'r')
                    print ('Downloading FourBytePattern.ArcticPro2.'+bitstream[1])
                    while True :
                        line = bits.readline()
                        if line :
                            count = count + 1
                            if (count > 3) :
                                items = line.split()

                                ser.write( (int(items[0],16)).to_bytes(4,byteorder="little"))
                        else:
                            break
                    bits.close()
                    print("Download complete", count, "Lines loaded")
                else :
                    try:
                        print (x.decode()[:-1])
                        log.write(x.decode())
                        log.flush()
                    except Exception as e1:
                        print ("error communicating.2: " + str(e1))
                        continue
                   
        except Exception as e1:
            print ("error communicating.3: " + str(e1))
            continue
    ser.close()

  except Exception as e1:
    PrintException()
    print ("error communicating...: " + str(e1))
 
  except KeyboardInterrupt:
    pass

else:
    print ("cannot open serial port ")
