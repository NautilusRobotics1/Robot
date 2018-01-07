'''
RaspPi.py
David Chau
sources:
https://github.com/nanpy/nanpy-firmware
'''

from nanpy import (ArduinoApi, SerialManager)
from nanpy import Servo
import socket
import time
import cPickle

#ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = '192.168.137.173' #ip of raspberry pi
port = 12345
s.bind((host, port))
s.listen(5)
c, addr = s.accept()

try:
    connection = SerialManager()
    ard = ArduinoApi(connection = connection) #auto detect arduino connection
except:
    print 'Arduino not connected'

motor_1 = Servo(11) #front left motor
motor_2 = Servo(10) #front right motor
motor_3 = Servo(9)  #back left motor
motor_4 = Servo(6)  #back right motor
motor_5 = Servo(5)  #top left 
motor_6 = Servo(3)  #top right

def forword(speedL, speedR):
    motor_1.writeMicroseconds()
    motor_2.writeMicroseconds()

def backword(speedL, speedR):
    motor_3.writeMicroseconds()
    motor_4.writeMicroseconds()
   
def up_down(speedL, speedR):
    motor_5.writeMicroseconds()
    motor_6.writeMicroseconds()
     

def pitch(speedL, speedR):
    motor_5.writeMicroseconds()
    motor_6.writeMicroseconds()
    
while True:
    value = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
    data = c.recv(200)
    try:
        value = cPickle.loads(data)
    except:
        pass
    print (value[0:4])
   
