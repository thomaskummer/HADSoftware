#!/usr/bin/env python3

from time import sleep
import tkinter as tk
from tkinter import messagebox
#import ask
#import setup
#import subprocess
import file
#import os

''' 
prototype version of GUI
updated look and layout
includes:
basic layout
auto button size
opening an exe in the same folder
giving values to exe NOT THROUGH VECTOR BUT WITH WORDS
basic implementation of mode tabs
ipm mode fully implemented functions
pm mode fully implemented functions
install mode continuous move implemented
finish mode no functions implemented
'''

def write(n):
    result = []
    for i in range(n):
        result.append(i)
        #sleep(1)
    print(result)
    return




class Application(tk.Frame):

    def __init__(self, master=None, size=[800, 480], touch=False, fs=False):
        tk.Frame.__init__(self, master, bg='white smoke')
        self.grid()
        ''' setup variables and functions'''
        self.color = 'white smoke'
        if fs:
            self.master.wm_attributes('-fullscreen', 'true')
        self.width = size[0]
        self.height = size[1]
        self.rows = 7
        self.columns = 5
        self.sizeSet(self.width, self.height, self.rows, self.columns)
        if touch:
            self.master.config(cursor='none')
        
        ''' variables to pass to exe'''
        self.NOTSTOP = tk.BooleanVar(value=False)
        self.modeVar = tk.IntVar(value=1)
        self.pdVar = tk.IntVar(value=0)
        self.ipmmodeVar = tk.IntVar(value=0)
        self.ipmamplitudeVar = tk.IntVar(value=10)
        self.hrVar = tk.IntVar(value=60)
        self.contdirVar = tk.IntVar(value=-1)
        self.startstopVar=tk.BooleanVar(value=False)
        self.exeopenVar = tk.BooleanVar(value=False)
        self.continuousSpeedVar = tk.IntVar(value=2)
        
        ''' name of program to execute'''
        #self.program='Temp' #can only take 3 variables at a time
        #self.program='Thread' 
        #self.program='ThreadWithVec' #can only take 3 variables at a time
        #self.program='ThreadWithVec8'
        #self.program='TempInf'
        #self.program='Capture'
        self.program='HeartrateController'
        ''' always on widgets except closeButton'''
        self.emergencySelect()
        self.tabSelect()
        self.topHeader()
        self.openSelect()
        self.startSelect()
        self.writeSelect()
        '''Install widgets'''
        self.continuousSelect()
        self.continuousHeader()
        '''PM widgets'''
        self.pmSelect()
        self.pmHeader()
        '''IPM widgets'''
        self.ipmmodeSelect()
        self.ipmmodeHeader()
        self.hrSelect()
        self.hrHeader()
        self.ampSelect()
        self.ampHeader()
        '''Finish widgets'''
        self.autoSelect()
        self.autoHeader()
        '''everything responsible for closing the GUI'''
        self.quitSelect()
        self.bind_all('<KeyPress-q>', self.quitForce)
    
    def sizeSet(self, width, height, rows, columns):
        rowheight = height/rows
        columnwidth = width/columns
        for i in range(rows):
            for j in range(columns):
                self.grid_rowconfigure(i, minsize=rowheight)
                self.grid_columnconfigure(j, minsize=columnwidth)
    
    def emergencySelect(self):
        def emergencyStop():
            if self.exeopenVar.get():
                self.NOTSTOP.set(True)
                self.process.stdin.write('stop\n')
                self.process.stdin.flush()
                self.startstopVar.set(False)
                #self.startButton.config(text='Start')
                #self.process.stdin.write(str(self.NOTSTOP.get())+'\n')
                #self.process.stdin.flush()
                #self.process.stdin.write(str(self.modeVar.get())+'\n')
                #self.process.stdin.write(str(self.pdVar.get())+'\n')
                #self.process.stdin.write(str(self.ipmmodeVar.get())+'\n')
                #self.process.stdin.write(str(self.ipmamplitudeVar.get())+'\n')
                #self.process.stdin.write(str(self.hrVar.get())+'\n')
                #self.process.stdin.write(str(self.contdirVar.get())+'\n')
                #self.process.stdin.write(str(self.startstopVar.get())+'\n')
                
            else:
                messagebox.showinfo('Information', 'No need to worry, no exe is working')
        self.emergencyButton = tk.Button(self, text='Emergency Stop', command=emergencyStop, bg='red', activebackground='red', highlightbackground=self.color)
        self.emergencyButton.grid(row=4, column=4, rowspan=2, sticky=tk.N+tk.E+tk.S+tk.W)
    
    def tabSelect(self):
        def getContinuous():
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
            
        self.tabButton1=tk.Radiobutton(self, text='Continuous', command=getContinuous, variable=self.modeVar, value=1, indicatoron=False, selectcolor=self.color, offrelief=tk.RAISED, bg='grey88', activebackground='grey88',highlightbackground=self.color)
        self.tabButton2=tk.Radiobutton(self, text='PM', command=getPM, variable=self.modeVar, value=2, indicatoron=False, selectcolor=self.color, offrelief=tk.RAISED, bg='grey88', activebackground='grey88', highlightbackground=self.color)
        self.tabButton3=tk.Radiobutton(self, text='IPM', command=getIPM, variable=self.modeVar, value=3, indicatoron=False, selectcolor=self.color, offrelief=tk.RAISED, bg='grey88', activebackground='grey88', highlightbackground=self.color)
        self.tabButton4=tk.Radiobutton(self, text='Auto', command=getAuto, variable=self.modeVar, value=4, indicatoron=False, selectcolor=self.color, offrelief=tk.RAISED, bg='grey88', activebackground='grey88', highlightbackground=self.color)
        
        self.tabButton1.grid(row=0, column=0, sticky=tk.N+tk.E+tk.S+tk.W)
        self.tabButton2.grid(row=0, column=3, sticky=tk.N+tk.E+tk.S+tk.W)
        self.tabButton3.grid(row=0, column=2, sticky=tk.N+tk.E+tk.S+tk.W)
        self.tabButton4.grid(row=0, column=1, sticky=tk.N+tk.E+tk.S+tk.W)
        
    def topHeader(self):
        self.topLabel=tk.Label(self,text='Define your configuration:', anchor=tk.W, bg=self.color)
        self.topLabel.grid(row=1, column=0, sticky=tk.N+tk.E+tk.S+tk.W)
    
    def openSelect(self):
        def openExe():
            if self.exeopenVar.get():
                if self.process.poll()==None:
                    self.process.terminate()
                if self.program=='HeartrateController':
                    self.process = file.vadex()
                else:
                    self.process = file.exe(self.program)
            else:
                self.exeopenVar.set(True)
                if self.program=='HeartrateController':
                    self.process = file.vadex()
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
        def startstopSend():
            if self.exeopenVar.get():
                if self.startstopVar.get():
                    self.process.stdin.write('stop\n')
                    self.process.stdin.flush()
                    self.startstopVar.set(False)
                    self.startButton.config(text='Start')
                else:
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

    def writeSelect(self):
        def writetoFile():
            f = open("mytext.txt", "w+")
            for i in range(int(self.heartrate.get())):
                f.write("This is line %d\r\n" % (i+1))
            f.close()
            print("Done")
            
        self.writeButton=tk.Button(self, text='Write to textfile', command=writetoFile, bg=self.color, activebackground=self.color)
        #self.writeButton.grid(row=3, column=4, sticky=tk.N+tk.E+tk.S+tk.W)
    
    def continuousSelect(self):
        
        self.continuousButton1=tk.Button(self, text='open', bg=self.color, activebackground=self.color)
        self.continuousButton1.grid(row=5, column=1, sticky=tk.N+tk.E+tk.S+tk.W)
        self.continuousButton2=tk.Button(self, text='close', bg=self.color, activebackground=self.color)
        self.continuousButton2.grid(row=5, column=2, sticky=tk.N+tk.E+tk.S+tk.W)
        self.continuousSpeedButton1=tk.Radiobutton(self, text='slow',variable=self.continuousSpeedVar, indicatoron=False, value=2, bg=self.color, activebackground=self.color, selectcolor='white')
        self.continuousSpeedButton1.grid(row=3, column=1, sticky=tk.N+tk.E+tk.S+tk.W)
        self.continuousSpeedButton2=tk.Radiobutton(self, text='fast',variable=self.continuousSpeedVar, indicatoron=False, value=4, bg=self.color,  activebackground=self.color, selectcolor='white')
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
        self.continuousLabel1=tk.Label(self, text='Select the speed:', bg=self.color, activebackground=self.color)
        self.continuousLabel1.grid(row=2, column=1, columnspan=2, sticky=tk.N+tk.E+tk.S+tk.W)
        self.continuousLabel2=tk.Label(self, text='Press and hold to move:', bg=self.color, activebackground=self.color)
        self.continuousLabel2.grid(row=4, column=1, columnspan=2, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=1:
            self.continuousLabel1.grid_remove()
            self.continuousLabel2.grid_remove()
    
    def pmSelect(self):
        self.pmSlider=tk.Scale(self, from_=-80, to=80, variable=self.pdVar, orient=tk.HORIZONTAL, resolution=5, bg=self.color, activebackground=self.color, highlightbackground=self.color)
        self.pmSlider.grid(row=3, column=1, columnspan=3, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=2:
            self.pmSlider.grid_remove()
    
    def pmHeader(self):
        self.pmLabel=tk.Label(self, text='Choose a distance to move [mm]:', bg=self.color)
        self.pmLabel.grid(row=2, column=1, columnspan=3, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=2:
            self.pmLabel.grid_remove()
    
    def ipmmodeSelect(self):
        self.ipmmodeButton1=tk.Radiobutton(self,text='sin',variable=self.ipmmodeVar, value=0,  command=None, indicatoron=False, bg=self.color, activebackground=self.color, selectcolor='white')
        self.ipmmodeButton2=tk.Radiobutton(self,text='sin²',variable=self.ipmmodeVar, value=1,  command=None, indicatoron=False, bg=self.color, activebackground=self.color, selectcolor='white')
        self.ipmmodeButton3=tk.Radiobutton(self,text='zigzag',variable=self.ipmmodeVar, value=2,  command=None, indicatoron=False, bg=self.color, activebackground=self.color, selectcolor='white')
        self.ipmmodeButton4=tk.Radiobutton(self,text='assymetric',variable=self.ipmmodeVar, value=3,  command=None, indicatoron=False, bg=self.color, activebackground=self.color, selectcolor='white')
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
        self.ipmmodeLabel=tk.Label(self,text='Choose mode:', bg=self.color)
        self.ipmmodeLabel.grid(row=2, column=0, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=3:
            self.ipmmodeLabel.grid_remove()
        
    def hrSelect(self):
        self.hrSlider = tk.Scale(self,from_=45,to=120, variable=self.hrVar, orient=tk.HORIZONTAL, resolution=15, bg=self.color, highlightbackground=self.color)
        self.hrSlider.grid(row=3,column=1,columnspan=3, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=3:
            self.hrSlider.grid_remove()
        
    def hrHeader(self):
        self.hrLabel=tk.Label(self,text='Choose heart rate [bpm]:', bg=self.color)
        self.hrLabel.grid(row=2, column=1, columnspan=3, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=3:
            self.hrLabel.grid_remove()
        
    def ampSelect(self):
        self.ampSlider = tk.Scale(self,from_=10,to=40, variable=self.ipmamplitudeVar, orient=tk.HORIZONTAL, resolution=5, bg=self.color, highlightbackground=self.color)
        self.ampSlider.grid(row=6,column=1,columnspan=3, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=3:
            self.ampSlider.grid_remove()
        
    def ampHeader(self):
        self.ampLabel=tk.Label(self,text='Choose amplitude [mm]:', bg=self.color)
        self.ampLabel.grid(row=5, column=1, columnspan=3, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=3:
            self.ampLabel.grid_remove()
        
    def autoSelect(self):
        
        self.autoButton1=tk.Button(self, text='AutoOpen', command=self.autoOpen, bg=self.color, activebackground=self.color, highlightbackground=self.color)
        self.autoButton2=tk.Button(self, text='AutoClose', command=self.autoClose, bg=self.color, activebackground=self.color, highlightbackground=self.color)
        self.autoButton1.grid(row=3, column=1, sticky=tk.N+tk.E+tk.S+tk.W)
        self.autoButton2.grid(row=3, column=2, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=4:
            self.autoButton1.grid_remove()
            self.autoButton2.grid_remove()

    def autoHeader(self):
        self.autoLabel=tk.Label(self, text='Press to automatically open or close:', bg=self.color)
        self.autoLabel.grid(row=2, column=1, columnspan=2, sticky=tk.N+tk.E+tk.S+tk.W)
        if self.modeVar.get()!=4:
            self.autoLabel.grid_remove()
    
    def quitSelect(self):
        def quitDialogue():
            quitVar=messagebox.askokcancel("Confirm", "Do you really want to quit?")    
            if quitVar:
                if self.exeopenVar.get():
                    if self.process.poll()==None:
                        self.process.stdin.write('exit\n')
                        self.process.stdin.flush()
                        #self.process.terminate()
                self.quit()
                
        self.quitButton = tk.Button(self, text='Quit', command=quitDialogue, bg=self.color, activebackground=self.color)
        self.quitButton.grid(row=6, column=4, sticky=tk.N+tk.E+tk.S+tk.W)

    def quitForce(self, event):
        if self.exeopenVar.get():
            if self.process.poll()==None:
                self.process.stdin.write('exit\n')
                self.process.stdin.flush()
                #self.process.terminate()
        
        self.quit()

    def sendopen(self, event):
        if self.exeopenVar.get():
            self.process.stdin.write('cont-plus-push ' + str(self.continuousSpeedVar.get()) + '\n')
            self.process.stdin.flush()
        else:
            None
    def sendopenstop(self, event):
        if self.exeopenVar.get():
            self.process.stdin.write('cont-plus-release\n')
            self.process.stdin.flush()
        else:
            None
    def sendclose(self, event):
        if self.exeopenVar.get():
            self.process.stdin.write('cont-minus-push ' + str(self.continuousSpeedVar.get()) + '\n')
            self.process.stdin.flush()
        else:
            None
    def sendclosestop(self, event):
        if self.exeopenVar.get():
            self.process.stdin.write('cont-minus-release\n')
            self.process.stdin.flush()
        else:
            None
            
    def autoOpen(self):
        if self.exeopenVar.get():
            self.process.stdin.write('home-plus\n')
            self.process.stdin.flush()
        else:
            None
            
    def autoClose(self):
        if self.exeopenVar.get():
            self.process.stdin.write('home-minus\n')
            self.process.stdin.flush()
        else:
            None

#mysetup=setup.setup()

## to make the size adjustable:
#w=ask.ask_size('Please set your screen width: ')
#h=ask.ask_size('Please set your screen height: ')

## to automatically go to full screen:


root = tk.Tk()
w=root.winfo_screenwidth()
h=root.winfo_screenheight()

s = [w, h]

app = Application(master=root)
#app = Application(master=root, size=s, touch=mysetup[0], fs=mysetup[1])
#app.master.wm_attributes('-fullscreen', 'true')
#if istouch:
#    app.master.config(cursor='none')

app.master.title('final04')
app.mainloop()


