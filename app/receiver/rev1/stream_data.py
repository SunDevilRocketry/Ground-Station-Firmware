import serial
import time

# Open the serial port
serObj = serial.Serial()
serObj.baudrate = 921600 
serObj.port = "/dev/ttyUSB0"
serObj.timeout = 1
serObj.open()

# Start the timer
#serObj.write( b'\x00')

while True:

	data = serObj.read().decode('ascii')
	if ( data = b'\x01'):
		print( "Ping recevied" )

