"""
Controller.py
David Chau

Sources:
http://robots.dacloughb.com/project-1/logitech-game-pad/
http://yameb.blogspot.com/2013/01/gamepad-input-in-python.html
https://docs.python.org/2/library/socket.html#socket-objects
https://docs.python.org/2/library/pickle.html
"""

import socket               
import pygame
import time
import cPickle
'''
Gets joystick data and prints it
'''
pygame.init()
j = pygame.joystick.Joystick(0)
j.init()
print 'Initialized Joystick : %s' % j.get_name()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)        
host = '192.168.137.9' # ip of raspberry pi 
port = 12345

pi_connection = False
print 'Connecting...'
while pi_connection is False:
    try:
        s.connect((host, port))
        pi_connection = True
    except:
        pass
  
time.sleep(3)
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
        out[it] = (1500 + 400*float(format(round(j.get_axis(i),4))))
        it+=1
    #Read input from buttons
    for i in range(0, j.get_numbuttons()):
        out[it] = j.get_button(i)
        it+=1
    return out

while True:
    value = get()
    print value
    data = cPickle.dumps(value,0)
    s.send(data)
    time.sleep(0.04) #delay for the raspberry pi server

