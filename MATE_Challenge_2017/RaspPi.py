import socket
import serial
import time
import struct

#ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = '192.168.137.48' #ip of raspberry pi
port = 12345
s.bind((host, port))

s.listen(5)
c, addr = s.accept()

def convert(speed):
  if(speed[0] == '+'):
    speed = speed[1:5]
  return speed

while True:
  control_LR = c.recv(5)
  control_UP = c.recv(5)
  #control_LR2 = getValue()
  #value = convert(control_LR)
  #value = control_LR
  print control_LR, ':', control_UP#, ':', control_LR2




