'''
RaspPi.py
David Chau
sources:
https://github.com/nanpy/nanpy-firmware
https://github.com/nanpy/nanpy
'''

from nanpy import (ArduinoApi, SerialManager)
from nanpy import Servo
import socket
import time
import cPickle

#ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = '192.168.137.90' #ip of raspberry pi
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

def forword_reverse(speed):
    #forword
    if speed < 1500:
        motor_1.writeMicroseconds(speed)
        motor_2.writeMicroseconds(speed)
        #print 'forword', speed
    #reverse
    if speed > 1500:
        motor_3.writeMicroseconds(speed)
        motor_4.writeMicroseconds(speed)
        #print 'backword', speed
   
def up_down(speed):
    motor_5.writeMicroseconds(speed)
    motor_6.writeMicroseconds(speed)
    #print speed
     

def pitch(speed):
    motor_5.writeMicroseconds()
    motor_6.writeMicroseconds()
'''
def left_right(speed):
    if speed > 1500: #right turn
        motor_2.writeMicroseconds(speed)
        motor_3.writeMicroseconds(speed)
        #print 'right', speed
    if speed < 1500:
        motor_1.writeMicroseconds(speed)
        motor_4.writeMicroseconds(speed)
        #print'left', speed

def half_throttle(cont_input):
    thrust = 1500 + 250 * cont_input
    return thrust

def full_throtle(cont_input):
    thrust = 1500 + 500 * cont_input
    return thrust
'''
while True:
    value = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
    data = c.recv(200)
    try:
        value = cPickle.loads(data)
    except:
        pass
    #print (value)
    
    #left_right(value[0])
    forword_reverse(value[0:4])
    up_down(value[3])
   
    

   
