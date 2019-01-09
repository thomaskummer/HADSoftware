#!/usr/bin/env python3

import tkinter as tk
from tkinter import messagebox


def setup():
	''' defines the parameters to give to the GUI '''
	setup = []
	root1 = tk.Tk()
	root1.withdraw()
	'''asks if the user is working on a touch device'''
	istouch=messagebox.askyesno("Setup Step 1", "Are you working on a touchscreen device?")
	'''asks if the user would like to work in full screen mode '''
	isfull=messagebox.askyesno("Setup Step 2", "Do you want to work in full screen mode?")
	root1.destroy()
	setup.append(istouch)
	setup.append(isfull)
	'''returns the setup variables to the main file'''
	return setup

