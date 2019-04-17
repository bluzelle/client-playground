#!/usr/bin/python3
from socket import *
import sys


port=1246
s=socket(AF_INET, SOCK_DGRAM)
s.bind(('',port))
data,address=s.recvfrom(1024)
print((data.decode('utf-8')).rstrip())
s.close()