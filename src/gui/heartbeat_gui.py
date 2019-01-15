#!/usr/bin/env python3
''' 
version 1.07 of GUI for HeartbeatController
created by Alexander Meier on 23.12.2018
'''

'''
README:
execute this program in command line with "$ python3 heartbeat_gui.py"

if folder HADSoftware is in the same place as this file, set use function file.vad()
if not, use function file.vadex() and  define the path to the HeartrateController in function file.vadex()


data is transmitted to the Heartbeatcontroller through a pipe
the command must be transmitted in the following way:
self.process.stdin.write('command to send as a string followed by \n')
self.process.stdin.flush()
'''

'''
this version includes:
updated look and layout
auto button size
chosing between opening an exe in the same folder or at a specific location
giving values to exe NOT THROUGH VECTOR BUT WITH WORDS
basic implementation of mode tabs
ipm mode implemented
pm mode implemented
continuous mode implemented
auto mode implemented
'''

from time import sleep
''' imports tkinter widget classes'''
import tkinter as tk
''' imports pop-up windows '''
from tkinter import messagebox
''' imports the header file with function for opening the heartrate controller program'''
import file 
'''this allows to configure the gui before opening
activate import and mysetup to use this feature'''
import setup
#mysetup=setup.setup()



class HeartbeatGui(tk.Frame):

    def __init__(self, master=None, size=[800, 480], touch=False, fs=True):
        tk.Frame.__init__(self, master, bg='white smoke')
        self.grid()
        ''' setup variables and functions'''
        self.color = 'white smoke'
        if fs:
            ''' runs the application in full screen mode if fs=True '''
            self.master.wm_attributes('-fullscreen', 'true')
        self.width = size[0]
        self.height = size[1]
        self.rows = 7
        self.columns = 5
        self.sizeSet(self.width, self.height, self.rows, self.columns)
        if touch:
            ''' disables the cursor if touch=true to improve user experience on touchscreen device'''
            self.master.config(cursor='none')
        
        ''' variables associated with various buttons & sliders '''
        self.NOTSTOP = tk.BooleanVar(value=False)
        self.modeVar = tk.IntVar(value=1)
        self.pdVar = tk.IntVar(value=0)
        self.ipmmodeVar = tk.IntVar(value=0)
        self.ipmamplitudeVar = tk.IntVar(value=10)
        self.hrVar = tk.IntVar(value=60)
        self.startstopVar=tk.BooleanVar(value=False)
        self.exeopenVar = tk.BooleanVar(value=False)
        self.continuousSpeedVar = tk.IntVar(value=2)
        
        ''' name of program to execute'''
        self.program='HeartbeatController'
        
        ''' general widgets'''
        self.emergencySelect()
        self.tabSelect()
        self.topHeader()
        self.openSelect()
        self.startSelect()
        
        '''continuous mode widgets'''
        self.continuousSelect()
        self.continuousHeader()
        
        '''profile mode widgets'''
        self.pmSelect()
        self.pmHeader()
        
        '''ipm mode widgets'''
        self.ipmmodeSelect()
        self.ipmmodeHeader()
        self.hrSelect()
        self.hrHeader()
        self.ampSelect()
        self.ampHeader()
        
        '''auto mode widgets'''
        self.autoSelect()
        self.autoHeader()
        
        '''close button'''
        self.quitSelect()
        '''binds the action of pressing the 'q' key with closing the GUI'''
        self.bind_all('<KeyPress-q>', self.quitForce)
    
    def sizeSet(self, width, height, rows, columns):
        ''' adjusts the size of the rows and columns'''
        rowheight = height/rows
        columnwidth = width/columns
        for i in range(rows):
            for j in range(columns):
                self.grid_rowconfigure(i, minsize=rowheight)
                self.grid_columnconfigure(j, minsize=columnwidth)
    
    def emergencySelect(self):
        ''' the emergency stop button'''
        def emergencyStop():
            '''checks if the exe is open if so sends a stop order'''
            if self.exeopenVar.get():
                self.NOTSTOP.set(True)
#                self.process.stdin.write('stop\n')
#                self.process.stdin.write('\n')
#                self.process.stdin.flush()
                self.process.kill()
                self.startstopVar.set(False)
                
                
            else:
                messagebox.showinfo('Information', 'Heart beat controller is not running.')
        self.emergencyButton = tk.Button(self, text='Emergency Stop', command=emergencyStop, bg='red', activebackground='red', highlightbackground=self.color)
        self.emergencyButton.grid(row=4, column=4, rowspan=2, sticky=tk.N+tk.E+tk.S+tk.W)
    
    def tabSelect(self):
        '''the widgets for mode selection'''
        def getContinuous():
            '''loads all widgets needed for continuous mode and removes others'''
            if self.pmLabel.winfo_ismapped():
                self.pmSlider.grid_remove()
                self.pmLabel.grid_remove()
            if self.startButton.winfo_ismapped():
                self.startButton.grid_remove()
            if self.ipmmodeLabel.winfo_ismapped():
                self.ipmmodeButton1.grid_remove()
                self.ipmmodeButton2.grid_remove()
                self.ipmmodeButton3.grid_remove()
                self.ipmmodeButton4.grid_remove()
                self.ipmmodeLabel.grid_remove()
                self.hrSlider.grid_remove()
                self.hrLabel.grid_remove()
                self.ampSlider.grid_remove()
                self.ampLabel.grid_remove()
            if self.autoLabel.winfo_ismapped():
                self.autoButton1.grid_remove()
                self.autoButton2.grid_remove()
                self.autoLabel.grid_remove()
            self.continuousButton1.grid()
            self.continuousButton2.grid()
            self.continuousSpeedButton1.grid()
            self.continuousSpeedButton2.grid()
            self.continuousLabel1.grid()
            self.continuousLabel2.grid()
            
        def getPM():
            '''loads all widgets needed for profile mode and removes others'''
            if self.continuousLabel1.winfo_ismapped():
                self.continuousButton1.grid_remove()
                self.continuousButton2.grid_remove()
                self.continuousSpeedButton1.grid_remove()
                self.continuousSpeedButton2.grid_remove()
                self.continuousLabel1.grid_remove()
                self.continuousLabel2.grid_remove()
            if self.ipmmodeLabel.winfo_ismapped():
                self.ipmmodeButton1.grid_remove()
                self.ipmmodeButton2.grid_remove()
                self.ipmmodeButton3.grid_remove()
                self.ipmmodeButton4.grid_remove()
                self.ipmmodeLabel.grid_remove()
                self.hrSlider.grid_remove()
                self.hrLabel.grid_remove()
                self.ampSlider.grid_remove()
                self.ampLabel.grid_remove()
            if self.autoLabel.winfo_ismapped():
                self.autoButton1.grid_remove()
                self.autoButton2.grid_remove()
                self.autoLabel.grid_remove()
            self.pdVar.set(0)
            self.pmLabel.grid()
            self.pmSlider.grid()
            if self.exeopenVar.get():
                self.startButton.grid()
            
        def getIPM():
            '''loads all widgets needed for ipm mode and removes others'''
            if self.continuousLabel1.winfo_ismapped():
                self.continuousButton1.grid_remove()
                self.continuousButton2.grid_remove()
                self.continuousSpeedButton1.grid_remove()
                self.continuousSpeedButton2.grid_remove()
                self.continuousLabel1.grid_remove()
                self.continuousLabel2.grid_remove()
            if self.pmLabel.winfo_ismapped():
                self.pmSlider.grid_remove()
                self.pmLabel.grid_remove()
            if self.autoLabel.winfo_ismapped():
                self.autoButton1.grid_remove()
                self.autoButton2.grid_remove()
                self.autoLabel.grid_remove()
            self.ipmmodeButton1.grid()
            self.ipmmodeButton2.grid()
            self.ipmmodeButton3.grid()
            self.ipmmodeButton4.grid()
            self.ipmmodeLabel.grid()
            self.hrSlider.grid()
            self.hrLabel.grid()
            self.ampSlider.grid()
            self.ampLabel.grid()
            if self.exeopenVar.get():
                self.startButton.grid()
            
        def getAuto():
            '''loads all widgets needed for auto mode and removes others'''
            if self.continuousLabel1.winfo_ismapped():
                self.continuousButton1.grid_remove()
                self.continuousButton2.grid_remove()
                self.continuousSpeedButton1.grid_remove()
                self.continuousSpeedButton2.grid_remove()
                self.continuousLabel1.grid_remove()
                self.continuousLabel2.grid_remove()
            if self.pmLabel.winfo_ismapped():
                self.pmSlider.grid_remove()
                self.pmLabel.grid_remove()
            if self.ipmmodeLabel.winfo_ismapped():
                self.ipmmodeButton1.grid_remove()
                self.ipmmodeButton2.grid_remove()
                self.ipmmodeButton3.grid_remove()
                self.ipmmodeButton4.grid_remove()
                self.ipmmodeLabel.grid_remove()
                self.hrSlider.grid_remove()
                self.hrLabel.grid_remove()
                self.ampSlider.grid_remove()
                self.ampLabel.grid_remove()
            if self.startButton.winfo_ismapped():
                self.startButton.grid_remove()
            self.autoLabel.grid()
            self.autoButton1.grid()
            self.autoButton2.grid()
            
        self.tabButton1=tk.Radiobutton(self, text='Motion by touch', command=getContinuous, variable=self.modeVar, value=1, indicatoron=False, selectcolor=self.color, offrelief=tk.RAISED, bg='grey88', activebackground='grey88',highlightbackground=self.color)
        self.tabButton4=tk.Radiobutton(self, text='Motion by distance', command=getPM, variable=self.modeVar, value=2, indicatoron=False, selectcolor=self.color, offrelief=tk.RAISED, bg='grey88', activebackground='grey88', highlightbackground=self.color)
        self.tabButton3=tk.Radiobutton(self, text='Heart contraction', command=getIPM, variable=self.modeVar, value=3, indicatoron=False, selectcolor=self.color, offrelief=tk.RAISED, bg='grey88', activebackground='grey88', highlightbackground=self.color)
        self.tabButton2=tk.Radiobutton(self, text='Motion to limit sensors', command=getAuto, variable=self.modeVar, value=4, indicatoron=False, selectcolor=self.color, offrelief=tk.RAISED, bg='grey88', activebackground='grey88', highlightbackground=self.color)
        
        self.tabButton1.grid(row=0, column=0, sticky=tk.N+tk.E+tk.S+tk.W)
        self.tabButton2.grid(row=0, column=2, sticky=tk.N+tk.E+tk.S+tk.W)
        self.tabButton3.grid(row=0, column=3, sticky=tk.N+tk.E+tk.S+tk.W)
        self.tabButton4.grid(row=0, column=1, sticky=tk.N+tk.E+tk.S+tk.W)
        
    def topHeader(self):
        '''a general widget'''
        self.topLabel=tk.Label(self,text='', anchor=tk.W, bg=self.color) # Define your configuration:
        self.topLabel.grid(row=1, column=0, sticky=tk.N+tk.E+tk.S+tk.W)
    
    def openSelect(self):
        '''the button to open the exe '''
        def openExe():
            if self.exeopenVar.get():
                if self.process.poll()==None:
                    self.process.terminate()
                if self.program=='HeartbeatController':
                    self.process = file.vadex()
                    #self.process = file.vad()
                else:
                    self.process = file.exe(self.program)
            else:
                self.exeopenVar.set(True)
                if self.program=='HeartbeatController':
                    self.process = file.vadex()
                    #self.process = file.vad()
                else:
                    self.process = file.exe(self.program)
                self.openButton.grid_remove()
                sleep(6)
                if self.modeVar.get()==2:
                    self.startButton.grid()
                elif self.modeVar.get()==3:
                    self.startButton.grid()

        self.openButton = tk.Button(self,text='Open\n '+self.program, command=openExe, bg=self.color, activebackground=self.color)
        self.openButton.grid(row=1, column=4, sticky=tk.N+tk.E+tk.S+tk.W)
        
    def startSelect(self):
        '''the start button, needed for ipm and pm mode'''
        def startstopSend():
            if self.exeopenVar.get():
                if self.startstopVar.get():
                    '''if the motor is already working, this stops the movement'''
                    self.process.stdin.write('Finish\n')
                    self.process.stdin.flush()
                    self.startstopVar.set(False)
                    self.startButton.config(text='Launch')
                else:
                    '''if the motor is not working, sends a command according to the selected mode'''
                    if self.modeVar.get()==1:
                        None
                        #self.process.stdin.write('Install instruction\n')
                        #self.process.stdin.flush()
                        #self.startstopVar.set(True)
                        #self.startButton.config(text='Stop')
                    elif self.modeVar.get()==2:
                        self.process.stdin.write('move '+str(self.pdVar.get())+'\n')
                        self.process.stdin.flush()
                        #self.startstopVar.set(True)
                        #self.startButton.config(text='Stop')
                    elif self.modeVar.get()==3:
                        self.process.stdin.write('ipm -'+str(self.ipmamplitudeVar.get())+' '+str(int(60000/self.hrVar.get()))+' '+str(self.ipmmodeVar.get())+'\n')
                        self.process.stdin.flush()
                        self.startstopVar.set(True)
                        self.startButton.config(text='Stop')
                    elif self.modeVar.get()==4:
                        None
                        #self.process.stdin.write('Finish instruction\n')
                        #self.process.stdin.flush()
                        #self.startstopVar.set(True)
                        #self.startButton.config(text='Stop')
            else:
                messagebox.showinfo('Error', 'No program open, please open program first')
                

        self.startButton = tk.Button(self, text='Start', command=startstopSend, bg=self.color, activebackground=self.color)
        self.startButton.grid(row=2, column=4, sticky=tk.N+tk.E+tk.S+tk.W)
        self.startButton.grid_remove()

    
    def continuousSelect(self):
        '''the buttons for speed and direction of continuous mode'''
        self.continuousButton1=tk.Button(self, text='Open device', bg=self.color, activebackground=self.color)
        self.continuousButton1.grid(row=5, column=1, sticky=tk.N+tk.E+tk.S+tk.W)
        self.continuousButton2=tk.Button(self, text='Close device', bg=self.color, activebackground=self.color)
        self.continuousButton2.grid(row=5, column=2, sticky=tk.N+tk.E+tk.S+tk.W)
        self.continuousSpeedButton1=tk.Radiobutton(self, text='Slow',variable=self.continuousSpeedVar, indicatoron=False, value=2, bg=self.color, activebackground=self.color, selectcolor='white')
        self.continuousSpeedButton1.grid(row=3, column=1, sticky=tk.N+tk.E+tk.S+tk.W)
        self.continuousSpeedButton2=tk.Radiobutton(self, text='Fast',variable=self.continuousSpeedVar, indicatoron=False, value=4, bg=self.color,  activebackground=self.color, selectcolor='white')
        self.continuousSpeedButton2.grid(row=3, column=2, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=1:
            self.continuousButton1.grid_remove()
            self.continuousButton2.grid_remove()
            self.continuousSpeedButton1.grid_remove()
            self.continuousSpeedButton2.grid_remove()
        self.continuousButton1.bind('<Button-1>', self.sendopen)
        self.continuousButton1.bind('<ButtonRelease-1>', self.sendopenstop)
        self.continuousButton2.bind('<Button-1>', self.sendclose)
        self.continuousButton2.bind('<ButtonRelease-1>', self.sendclosestop)
    
    def continuousHeader(self):
        '''the labels for continuous mode'''
        self.continuousLabel1=tk.Label(self, text='Select velocity', bg=self.color, activebackground=self.color)
        self.continuousLabel1.grid(row=2, column=1, columnspan=2, sticky=tk.N+tk.E+tk.S+tk.W)
        self.continuousLabel2=tk.Label(self, text='Touch to move', bg=self.color, activebackground=self.color)
        self.continuousLabel2.grid(row=4, column=1, columnspan=2, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=1:
            self.continuousLabel1.grid_remove()
            self.continuousLabel2.grid_remove()
    
    def pmSelect(self):
        '''the slider for pm mode distance'''
        self.pmSlider=tk.Scale(self, from_=-80, to=80, variable=self.pdVar, orient=tk.HORIZONTAL, resolution=5, bg=self.color, activebackground=self.color, highlightbackground=self.color)
        self.pmSlider.grid(row=3, column=1, columnspan=3, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=2:
            self.pmSlider.grid_remove()
    
    def pmHeader(self):
        '''the label for pm mode'''
        self.pmLabel=tk.Label(self, text='Motion distance [mm]', bg=self.color)
        self.pmLabel.grid(row=2, column=1, columnspan=3, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=2:
            self.pmLabel.grid_remove()
    
    def ipmmodeSelect(self):
        '''the buttons for selection of the ipm function'''
        self.ipmmodeButton1=tk.Radiobutton(self,text='20 %',variable=self.ipmmodeVar, value=0,  command=None, indicatoron=False, bg=self.color, activebackground=self.color, selectcolor='white')
        self.ipmmodeButton2=tk.Radiobutton(self,text='30 %',variable=self.ipmmodeVar, value=1,  command=None, indicatoron=False, bg=self.color, activebackground=self.color, selectcolor='white')
        self.ipmmodeButton3=tk.Radiobutton(self,text='40 %',variable=self.ipmmodeVar, value=2,  command=None, indicatoron=False, bg=self.color, activebackground=self.color, selectcolor='white')
        self.ipmmodeButton4=tk.Radiobutton(self,text='50 %',variable=self.ipmmodeVar, value=3,  command=None, indicatoron=False, bg=self.color, activebackground=self.color, selectcolor='white')
        self.ipmmodeButton1.grid(row=3, column=0, sticky=tk.N+tk.E+tk.S+tk.W)
        self.ipmmodeButton2.grid(row=4, column=0, sticky=tk.N+tk.E+tk.S+tk.W)
        self.ipmmodeButton3.grid(row=5, column=0, sticky=tk.N+tk.E+tk.S+tk.W)
        self.ipmmodeButton4.grid(row=6, column=0, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=3:
            self.ipmmodeButton1.grid_remove()
            self.ipmmodeButton2.grid_remove()
            self.ipmmodeButton3.grid_remove()
            self.ipmmodeButton4.grid_remove()
        
    def ipmmodeHeader(self):
        '''the label of the ipm function'''
        self.ipmmodeLabel=tk.Label(self,text='Select systole/diastoly ratio', bg=self.color)
        self.ipmmodeLabel.grid(row=2, column=0, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=3:
            self.ipmmodeLabel.grid_remove()
        
    def hrSelect(self):
        '''the slider for selection of the ipm heart rate'''
        self.hrSlider = tk.Scale(self,from_=45,to=120, variable=self.hrVar, orient=tk.HORIZONTAL, resolution=15, bg=self.color, highlightbackground=self.color)
        self.hrSlider.grid(row=3,column=1,columnspan=3, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=3:
            self.hrSlider.grid_remove()
        
    def hrHeader(self):
        '''the label of the ipm heart rate'''
        self.hrLabel=tk.Label(self,text='Set heart rate [bpm]', bg=self.color)
        self.hrLabel.grid(row=2, column=1, columnspan=3, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=3:
            self.hrLabel.grid_remove()
        
    def ampSelect(self):
        '''the slider for selection of the ipm amolitude'''
        self.ampSlider = tk.Scale(self,from_=10,to=40, variable=self.ipmamplitudeVar, orient=tk.HORIZONTAL, resolution=5, bg=self.color, highlightbackground=self.color)
        self.ampSlider.grid(row=6,column=1,columnspan=3, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=3:
            self.ampSlider.grid_remove()
        
    def ampHeader(self):
        '''the label of the ipm amplitude'''
        self.ampLabel=tk.Label(self,text='Set amplitude [mm]', bg=self.color)
        self.ampLabel.grid(row=5, column=1, columnspan=3, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=3:
            self.ampLabel.grid_remove()
        
    def autoSelect(self):
        '''the buttons for automatic mode'''
        self.autoButton1=tk.Button(self, text='Open device completely', command=self.autoOpen, bg=self.color, activebackground=self.color, highlightbackground=self.color)
        self.autoButton2=tk.Button(self, text='Close device completely', command=self.autoClose, bg=self.color, activebackground=self.color, highlightbackground=self.color)
        self.autoButton1.grid(row=3, column=1, sticky=tk.N+tk.E+tk.S+tk.W)
        self.autoButton2.grid(row=3, column=2, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=4:
            self.autoButton1.grid_remove()
            self.autoButton2.grid_remove()

    def autoHeader(self):
        '''the label for automatic mode'''
        self.autoLabel=tk.Label(self, text='Press to automatically move to limits', bg=self.color)
        self.autoLabel.grid(row=2, column=1, columnspan=2, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=4:
            self.autoLabel.grid_remove()
    
    def quitSelect(self):
        '''the quit button'''
        def quitDialogue():
            '''creates a pop up window asking for confirmation'''
            quitVar=messagebox.askokcancel("Confirm", "Are you sure to quit?")
            '''if closing is confirmed, terminates the HeartbeatController and the GUI'''
            if quitVar:
                if self.exeopenVar.get():
                    if self.process.poll()==None:
                        self.process.stdin.write('exit\n')
                        self.process.stdin.flush()
                self.quit()
                
        self.quitButton = tk.Button(self, text='Quit', command=quitDialogue, bg=self.color, activebackground=self.color)
        self.quitButton.grid(row=6, column=4, sticky=tk.N+tk.E+tk.S+tk.W)

    def quitForce(self, event):
        '''closes the HeartbeatController and the GUI without asking for confirmation'''
        if self.exeopenVar.get():
            if self.process.poll()==None:
                self.process.stdin.write('exit\n')
                self.process.stdin.flush()
                #self.process.terminate()
        
        self.quit()

    def sendopen(self, event):
        '''sends the command to start continuously opening'''
        if self.exeopenVar.get():
            self.process.stdin.write('cont-plus-push ' + str(self.continuousSpeedVar.get()) + '\n')
            self.process.stdin.flush()
        else:
            None
    def sendopenstop(self, event):
        '''sends the command to stop continuously opening'''
        if self.exeopenVar.get():
            self.process.stdin.write('cont-plus-release\n')
            self.process.stdin.flush()
        else:
            None
    def sendclose(self, event):
        '''sends the command to start continuously closing'''
        if self.exeopenVar.get():
            self.process.stdin.write('cont-minus-push ' + str(self.continuousSpeedVar.get()) + '\n')
            self.process.stdin.flush()
        else:
            None
    def sendclosestop(self, event):
        '''sends the command to stop continuously closing'''
        if self.exeopenVar.get():
            self.process.stdin.write('cont-minus-release\n')
            self.process.stdin.flush()
        else:
            None
            
    def autoOpen(self):
        '''sends the command to open until end position'''
        if self.exeopenVar.get():
            self.process.stdin.write('home-plus\n')
            self.process.stdin.flush()
        else:
            None
            
    def autoClose(self):
        '''sends the command to close until end position'''
        if self.exeopenVar.get():
            self.process.stdin.write('home-minus\n')
            self.process.stdin.flush()
        else:
            None



'''the root window where the GUI is placed in'''
root = tk.Tk()
'''gets the size of the screen'''
w=root.winfo_screenwidth()
h=root.winfo_screenheight()
s = [w, h]

'''if setup mode is activated, starts the GUI with mysetup parameters, else starts with default parameters'''
if 'mysetup' in locals():
    gui = HeartbeatGui(master=root, size=s, touch=mysetup[0], fs=mysetup[1])
else:
    gui = HeartbeatGui(master=root, size=s)

gui.master.title('HeartbeatGUI')
gui.mainloop()

