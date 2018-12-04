//
//  main.cpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
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

#include "HeartrateController.hpp"

using namespace HeartrateControllerSpace;

#define mm * -1600


int main(int argc, char** argv)
{
    HeartrateController hrc;

    std::thread guiInterfaceThread( &HeartrateController::readGuiActions, &hrc, 5 );
    guiInterfaceThread.join();
    
    hrc.readCmdLineArguments(argc, argv);

    hrc.setup();
    
    if (hrc.cmdLineParser().feature("-i"))
    {
        std::string mode;
        std::cout << "\nType 'run' to start-up the controller or 'help' for \nmore information. \n>>> ";
        std::cin >> mode;
        if ( mode.compare("help") == 0 ) std::cout << "This is the still todo help page" << std::endl;
        
        
        std::cout << "\nDefine the mode of motion (for help type h or help) \n>>> ";
        
        while ( std::cin >> mode )
        {
            if ( mode.compare("exit") == 0 || mode.compare("quit") == 0 || mode.compare("q") == 0 )
            {
                std::cout << ">>> Exiting heartrate-controller.";
                break;
            }
            
            std::cout << mode << std::endl;
            
            std::istringstream iss(mode);
            std::string a, b;
            
            if (iss >> a >> b)
            {
                std::cout << "iss: " << a << " " << b << std::endl;
                if ( a.compare("move") == 0 )
                {
                    //controller.move(std::stoi(b) mm);
                    //controller.printSpindlePosition();
                }
            }
            
            std::cout << "Define the mode of motion (for help type h or help) >>> ";
        }
        
        
        
        exit (0);
    }
    
    hrc.setMotionModeFromCmdLine();
    
    hrc.activateMotionMode();
    
    hrc.run();

    hrc.printPosition();
}
