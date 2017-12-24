import serial
import socket

# Initializing Socket Communication
ser = serial.Serial('/dev/ttyACMO',9600, timeout=1)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Main Thread
# Todo account for SIGTERM (i.e. kill signal) and flush_in_buffer
while True:

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

	message =  ser.read(27)
		# Reading Sensor Data
	    # Bytes 0-11 are from Accelerometer
	    # Byte 12 from Pressure
	    # Byte 13 from Altitude
	    # Byte 14-25 from Gyroscope
	    # Bye 26 from Azimuth
	    # Byte 27 from Temp

	return message;

def writeToArduino():
	# TODO

def readFromLaptop():
	# TODO: David Chau