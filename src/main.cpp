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

#include "HeartrateController.hpp"


#define mm * -1600


int main(int argc, char** argv)
{
    HeartrateController hrc;

    hrc.readCmdLineArguments(argc, argv);

    hrc.setup();
    
    hrc.setMotionModeFromCmdLine();

    hrc.activateMotionMode();

    hrc.run();

    hrc.printPosition();
}
