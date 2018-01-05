import serial
import socket
import signal
import sys

# Initializing Socket Communication
ser = serial.Serial('/dev/ttyACMO',9600, timeout=1)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Main Thread
while True:
	# Error Handling
	signal.signal(signal.SIGTERM, signal_term_handler)

	# Motor Code
    readFromLaptop()
    writeToArduino()

	# Sensor Code
    msg = readFromArduino()
    # To be completed with Laptop's Socket (IP + port)
    sock.sendto(msg, ("dns.or.ip.of.server", pick_a_port_number))

# Baseline from: https://github.com/pyserial/pyserial/issues/216
# Pi to Laptop from: https://raspberrypi.stackexchange.com/questions/14518/sending-live-data-readings-from-raspberry-pi-to-laptop

# returns bytes
def readFromArduino():
	# Example reads
	# x = ser.read() # Read 1 byte
	# y = ser.read(10) # Reading 10 bytes
	# line = ser.readline() # Reading until EOL

	# Reading Sensor Data
    # Bytes 0-11 are from Accelerometer
    # Byte 12 from Pressure
    # Byte 13 from Altitude
    # Byte 14-25 from Gyroscope
    # Bye 26 from Azimuth
    # Byte 27 from Temp
	message =  ser.read(27)

	return message;
	# Learned via: http://pyserial.readthedocs.io/en/latest/shortintro.html

def writeToArduino():
	# TODO

def readFromLaptop():
	# TODO: David Chau

# Error Handling
def signal_term_handler(signal, frame):
	print 'got SIGTERM'
	serial.flush_in_buffer()
	sys.exit(0)	

	# Handling a Kill Signal from: https://nattster.wordpress.com/2013/06/05/catch-kill-signal-in-python/




