"""
Controller.py
David Chau

Based off code from:
http://robots.dacloughb.com/project-1/logitech-game-pad/
http://yameb.blogspot.com/2013/01/gamepad-input-in-python.html
"""

import socket               
import pygame

'''
Gets joystick data and prints it
'''
pygame.init()
j = pygame.joystick.Joystick(0)
j.init()
print 'Initialized Joystick : %s' % j.get_name()

'''
Connects to raspberry pi server
'''
s = socket.socket()        
host = '192.168.137.53'# ip of raspberry pi 
port = 12345               
s.connect((host, port))
s.listen(5)
'''
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
'''

while True:
    c = s.accept()
    out = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
    it = 0 #iterator
    pygame.event.pump()
    
    #Read input from the two joysticks       
    for i in range(0, j.get_numaxes()):
        out[it] = j.get_axis(i)
        it+=1
    #Read input from buttons
    for i in range(0, j.get_numbuttons()):
        out[it] = j.get_button(i)
        it+=1
    c.send(out);    
    print(out)


