'''
ServerTest.py
David Chau
Based on:
https://raspberrypi.stackexchange.com/questions/13425/server-and-client-between-pc-and-raspberry-pi
'''

import socket               

s = socket.socket()        
host = '192.168.137.53'# ip of raspberry pi 
port = 12345               
s.connect((host, port))
print(s.recv(1024))
s.close()
