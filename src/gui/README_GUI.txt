Execution of the GUI for the HeartbeatController

	heartbeat_gui.py will only work with python versions 3+, but not with python2.7
	
	Execute the program in command line with "$ python3 heartbeat_gui.py"



Configuration of the interface with the motor controller program

	There are two possible configurations for the interaction depending on where the folder HADSoftware is located
	If folder HADSoftware is in the same place as heartbeat_gui.py, use function file.vad()
	If folder HADSoftware is in a different location, use function file.vadex()
	
	To switch between the two functions, go to openExe() in heartbeat_gui.py (lines 275, 276, 282 & 283) and uncomment the required function (add #) and comment the other (remove #)
	
	When using file.vadex() for the first time, open the file.py header file and define the complete path to HADSoftware in line 21



Activating setup mode

	By default the GUI will start in touchscreen & fullscreen mode
	Therefore, no cursor and menu bars will be visible

	When working on a non-touch device however, a setup mode can be activated
	In setup mode, the program will ask if you would like to work in touchscreen & fullscreen mode before opening the GUI
	
	To activate the setup mode, open heartbeat_gui.py and uncomment lines 43 & 44


		
Data Transmission between GUI & motor controller program

	Data is transmitted to the Heartbeatcontroller through a pipe
	The command must be transmitted in the following way:
	
	self.process.stdin.write('command to send as a string followed by \n')
	self.process.stdin.flush()

