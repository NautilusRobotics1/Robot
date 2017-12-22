import serial
import socket

ser = serial.Serial('/dev/ttyACMO',9600, timeout=1)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

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

def readFromArduino():

	#reading = ser.readline()
	return message;

def writeToArduino():
	# TODO

def readFromLaptop():
	# TODO: David Chau