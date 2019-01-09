#!/usr/bin/env python3

import subprocess
from time import sleep
import os
''' defines the functions to open an executable in the same folder as the python file and opens a stdin-pipe for it '''

def exe(text, config=''):
	path = os.path.realpath(text)
	print(path)
	a=subprocess.Popen([path, config], stdin=subprocess.PIPE, universal_newlines=True)
	return a

def vad():
	path = os.path.realpath('') +'/HADSoftware/build/HeartbeatController'
	#path= '/home/alexmeier/Downloads/HADSoftware/build/HeartbeatController'
	a=subprocess.Popen([path, '-gui'], stdin=subprocess.PIPE, universal_newlines=True)
	#print(path)
	return a

def vadex():
	#path = os.path.realpath('') +'/HADSoftware/build/HeartbeatController'
	path= '/home/thomas/Dokumente/HADSoftware/build/HeartbeatController'
	a=subprocess.Popen([path, '-gui'], stdin=subprocess.PIPE, universal_newlines=True)
	#print(path)
	return a

def input(text):
	a=subprocess.Popen([text])
	return a
