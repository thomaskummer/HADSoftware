# Heartrate-Controller package for Maxon EPOS2

For heart assist device experiments at the Institute of Fluid Dynamics, both a Maxon controller and a Maxon electric motor are employed. The motor drives a gear, which again drives as spindle that translates rotary motion into linear motion. Between the heart assist patches which move toward each other and the spindle, a Bowden cable used. This system allows for a lean patch design and can easily be wrapped around the heart. The Maxon controller is operated through this C++ package.

## Prerequisites

For compiling this package, the EPOS-Linux-Library has to be installed. 

[Epos-Linux-Library](https://www.maxonmotor.com/maxon/view/product/control/Positionierung/375711)


## Download

```
git clone https://github.com/thomaskummer/HADSoftware.git
```

## Build

```
cd HADSoftware
./config
cd build
make
```

## Run & commandline arguments

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

## Interactive mode

```
./HeartrateController -gui

help: interactive commands explained

ipm: interpolated position mode (standard mode for asisting the heart)
    - [ARG-1] amplitude (in mm; negative for heart contraction)
    - [ARG-2] period or time for one cycle (in ms)
    - [ARG-3] motion mode (0: sin(t), 1 : sin^2(t), 2:|x|; default: 1)
    - [ARG-4] asymmetric parameter a for mode 2 (0 < a < 1; default: 0.5)

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

