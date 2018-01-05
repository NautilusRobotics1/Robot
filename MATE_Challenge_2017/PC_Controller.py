"""
Controller.py
David Chau

Based off code from:
http://robots.dacloughb.com/project-1/logitech-game-pad/
http://yameb.blogspot.com/2013/01/gamepad-input-in-python.html
"""

import socket               
import pygame
import time
import pickle
'''
Gets joystick data and prints it
'''
pygame.init()
j = pygame.joystick.Joystick(0)
j.init()
print 'Initialized Joystick : %s' % j.get_name()
"""
Returns a vector of the following form:
[LThumbstickX, LThumbstickY, Unknown Coupled Axis???, 
RThumbstickX, RThumbstickY, 
Button 1/X, Button 2/A, Button 3/B, Button 4/Y, 
Left Bumper, Right Bumper, Left Trigger, Right Triller,
Select, Start, Left Thumb Press, Right Thumb Press]

Note:
No D-Pad.
Triggers are switches, not variable. 
Your controller may be different
"""

def get():
    out = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
    it = 0 #iterator
    pygame.event.pump()
    
    #Read input from the two joysticks       
    for i in range(0, j.get_numaxes()):
        out[it] = str("{0:.2f}".format(j.get_axis(i)))
        it+=1
    #Read input from buttons
    for i in range(0, j.get_numbuttons()):
        out[it] = str("{0:.2f}".format(j.get_button(i)))
        it+=1
    return out

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)        
    host = '192.168.137.48' # ip of raspberry pi 
    port = 12345               
    s.connect((host, port))
except:
    print 'Try connecting again'

def numCov(speed):
    if(speed[0] != '-'):
        speed = '+' + speed
    return speed

while True:    
    value = str(get()[0:4])
    #print value
    C_Input = get()
    speed1 = numCov(str(C_Input[0]))
    speed2 = numCov(str(C_Input[1]))
    s.sendall(speed1)
    s.sendall(speed2)
    print speed1, ':', speed2
    #s.send(str(controller_List[0]))
    #s.send(str(controller_List[1]))
    #s.send(str(controller_List[2]))
    #s.send(value4)
    time.sleep(0.03) #delay for the raspberry pi server    
    
