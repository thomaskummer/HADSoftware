# Heartrate-Controller framework for Maxon EPOS2

For heart assist device experiments at the Institute of Fluid Dynamics, both a Maxon controller and a Maxon electric motor are employed. The motor drives a gear, which again drives as spindle that translates rotary motion into linear motion. Between the heart assist patches which move toward each other and the spindle, a Bowden cable used. This system allows for a lean patch design and can easily be wrapped around the heart. The Maxon controller is operated through this C++ package.


## Download  framework

By cloning the following git repository, the EPOS-Linux-Library (from Maxon-Motor), the EPOS 2 controller framework and the graphical user interface for the touchscreen, are downloaded.

```
git clone https://github.com/thomaskummer/HADSoftware.git
```

## Install Maxon's EPOS-Linux-Library

For compiling the package, the EPOS-Linux-Library must be installed first. The library can be found in the doc directory and is installed by executing the "install.sh" file as superuser.

```
cd HADSoftware/doc/EPOS_Linux_Library
sudo bash install.sh
```

The EPOS-Linux-Library has originally be downloaded from:

[Epos-Linux-Library](https://www.maxonmotor.com/maxon/view/product/control/Positionierung/375711)


## Build the EPOS 2 controller framework

Then, the EPOS 2 controller code is configured and built through:

```
cd HADSoftware
./config
cd build
make
```

If the above steps have successfully been completed, the system is ready to be used. There are three different ways of using it. Either through running the graphical user-interface which is found in the HADSoftware head directory, by executing the "HeartrateController" executeable from command-line with certain arguments or by running the interactive command-line mode by running "HeartrateController -gui". Further explanation can be found below.


## 1. Run HeartrateController with commandline arguments

```
./HeartrateController ...
-h   : this help
-n   : [ARG] number of program repetitions (default 1)
-pm  : profile mode
-pd    : [ARG] distance to move (in mm; default 10)
-ipm : interpolated position mode
-if    : [ARG] motion type (0 for sin(t), 1 for sin^2(t), default 0)
-ia    : [ARG] amplitude (in mm; default -20)
-ip    : [ARG] period, time for one contraction (in ms; default 1000)
-gui   : interactive mode (default if no arguments passed; used by the available gui/touchscreen)
         

example: ./HeartrateController -ipm -if 1 -ia 40 -n 2
```


## 2. Run HeartrateController interactive mode

```
./HeartrateController -gui

help: interactive commands explained

ipm: interpolated position mode (standard mode for asisting the heart)
    - [ARG-1] amplitude (in mm; negative for heart contraction)
    - [ARG-2] period or time for one cycle (in ms)
    - [ARG-3] motion mode (0: sin(t), 1 : sin^2(t), 2:|x|; default: 1)
    - [ARG-4] asymmetric parameter "a" for mode 2 (0 < a < 1; default: 0.5)

home-minus: motion in negative direction stopped by a Reed sensor
    - [ARG] motion velocity (default: 2)
    
home-plus: motion in positiv direction stopped by a Reed sensor
    - [ARG] motion velocity (default: 2)
    
home-stop: stop home-minus or home-plus motion

move: moving a certain distance
    - [ARG] distance (in cm; +/- sign matter; no default)

stop: all modes are stopped after completion

reset: EPOS 2 controller reset

exit: leave interactive mode
```

## 3. Run HeartrateController graphical-user-interface

```
cd HADSoftware
./GraphicalUserInterface.py
```
