# Software package for external heart assist device actuation with Maxon EPOS2

For heart assist device experiments at the Institute of Fluid Dynamics, both a Maxon controller and a Maxon electric motor are employed. The motor drives a gear, which again drives as spindle that translates rotary motion into linear motion. Between the heart assisting patches moving toward each other and the spindle a Bowden cable used. This system allows for a lean patch design and can easily be wrapped around the heart. The Maxon controller is operated through this package.

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

## Run

```
./HeartrateController ...
-h   : this help
-n   : [ARG] number of program repetitions (default 1)
-pm  : profile mode
-pd    : [ARG] distance to move (in mm, default 10)
-ipm : interpolated position mode
-ia    : [ARG] amplitude (in mm, default 20)
-ip    : [ARG] period, time for one contraction (in ms, default 1000)

example: ./HeartrateController -ipm -n 2
```
