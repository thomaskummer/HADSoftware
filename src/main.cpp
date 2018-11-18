//==================
//main
//Jonas Conrad
//==================

#include <iostream>
#include "Definitions.h"
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/time.h>
#include <chrono>
#include <thread>

#include "Engine.hpp"
#include "HADController.hpp"



//enum mode { profilePosition, position, interpolatedPosition };


int main(int argc, char** argv)
{
    
    std::string type = std::string(*argv[1]);
    int dist = int(*argv[2]);

        
    if ( type == "-h" )
    {
        HADController controller;
        controller.setup();

        InputFileParser inputFile("data");
        std::cout << "Motion distance: " << inputFile["a"] << std::endl; // controller.move(inputFile["a"]);
        controller.move(dist);
    }
    
    
//  Engine maxonMotor;
//  maxonMotor.ClearFault();
//  maxonMotor.Restore();
//  maxonMotor.SetAll();
//  maxonMotor.GetDeviceErrorCode();
//  maxonMotor.ActivateProfileMode();
//  maxonMotor.SetPosition_ProfileMode(0);
//	maxonMotor.ActivateIPMode();
//	maxonMotor.SetMainPositionsFromCurrent();
//  maxonMotor.InterpolatedPositionMode(-20000, 1000, 250, 10000, 500);
//	maxonMotor.GetPTV(10000,1,800,50,500);
//  maxonMotor.SetObject(0x6062,0x00,0,4);
//	maxonMotor.GetObject(0x6064,0x00,4);
//	maxonMotor.GetObject(0x6062,0x00,4);
//	maxonMotor.InterpolatedPositionMode();
//	maxonMotor.PositionMode();
//  maxonMotor.MoveXBackward(10000);
//	maxonMotor.MoveXForward(20000);
//  maxonMotor.SetCurrentToHome();
//	maxonMotor.SetMainPositions();
//	maxonMotor.GoToMid();
//	maxonMotor.GoToMax();
//	maxonMotor.MoveXBackward(20000);
//	maxonMotor.ProfilePositionMode();s
//	maxonMotor.GoToMid();
//  maxonMotor.PrintPosition();
    
    if ( type == "-e" )
    {
        Engine maxonMotor;
        maxonMotor.ClearFault();
        maxonMotor.SetAll();
        maxonMotor.GetDeviceErrorCode();
        maxonMotor.ActivateProfileMode();
        maxonMotor.GetObject(0x6064,0x00,4);
        maxonMotor.GetObject(0x6062,0x00,4);
        maxonMotor.MoveXBackward(dist);
        maxonMotor.PrintPosition();
    }
    
}
