//
//  main.cpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

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
#include "CommandLineParser.hpp"

#define mm * -1600

//enum mode { profilePosition, position, interpolatedPosition };


int main(int argc, char** argv)
{
    CommandLineParser cmdLineParser(argc, argv);

    std::cout << "====================================================" << std::endl;
    std::cout << "Heartrate-controller for Maxon EPOS2 70/10" << std::endl;
    std::cout << "Institute of fluid dynamics - ETH Zurich" << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << "\nType \"run\" to start-up the controller or \"help\" for more information. << std::endl;

    
    std::cout << "\nDefine the mode of motion (for help type h or help) \n>>> ";
    std::string mode;
    while ( std::cin >> mode )
    {
        if ( mode.compare("exit") == 0 || mode.compare("quit") == 0 || mode.compare("q") == 0 )
        {
            std::cout << ">>> Exiting heartrate-controller.";
            break;
        }
        
        
        std::istringstream iss(mode);
        std::string a, b, c;
        
        if (!(iss >> a >> b >> c)) continue;
        
        if ( a.compare("move") == 0 )
        {
            HADController controller;
            controller.activateProfileMode();
            auto val = cmdLineParser("-m");
            controller.move(b mm);
            controller.printSpindlePosition();
        }
        
        std::cout << "Define the mode of motion (for help type h or help) \n>>> ";
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
