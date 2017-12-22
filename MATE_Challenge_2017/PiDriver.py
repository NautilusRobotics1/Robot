import serial
ser = serial.Serial('/dev/ttyACMO',9600, timeout=1)

while True:
	reading = ser.readline()
	#print(reading)
	#TODO: Do Something

# Baseline from: https://github.com/pyserial/pyserial/issues/216