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
#include <assert.h>
#include <map>

#include "Engine.hpp"
#include "HADController.hpp"

#define mm * -1600

//enum mode { profilePosition, position, interpolatedPosition };


int main(int argc, char** argv)
{

    assert (argc >= 2);
    
    std::map<std::string, double> tasks;

    for (int i(1); i < argc; ++i)
    {
        std::istringstream issType (argv[i]);
        std::string type;
        if (iss1 >> type)
        {
            if ( type.compare("-m") == 0 )
            {
                std::istringstream issVal (argv[i++]);
                double val;
                if (issVal >> val)
                {
                    tasks.emplace("move", val);
                }
            }
            
        }
    }
    
    
    std::endl << "Tasks:" << std::endl;
    for (auto& task : tasks) std::cout << task-first() << "\t" << task-second() << std::endl;
    
    
    if ( type.compare("-m") == 0 )
    {
        HADController controller;
        controller.setup();
        controller.activateProfileMode();
        controller.move(val mm);
        controller.printSpindlePosition();
    }
    

// Classes for different activation modes
// Classes for reading sensores
    
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
//	maxonMotor.ProfilePositionMode();
//	maxonMotor.GoToMid();
//  maxonMotor.PrintPosition();
    
    
//    // if ( type.compare("e") == 0 )
//    {
//        Engine maxonMotor;
//        //maxonMotor.Reset();
//        //maxonMotor.setup();
//
//        // setup
//        maxonMotor.ClearFault();
//        maxonMotor.SetAll();
//        maxonMotor.GetDeviceErrorCode();
//
//        // activateProfileMode
//        maxonMotor.ActivateProfileMode();
//
//        // missing above
//        maxonMotor.GetObject(0x6064,0x00,4);
//        maxonMotor.GetObject(0x6062,0x00,4);
//
//        // move
//        maxonMotor.MoveXBackward(val mm);
//        maxonMotor.PrintPosition();
//    }
    
}
