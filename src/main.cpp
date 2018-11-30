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


#define mm * -1600

//Get and print out object: all objects can be found in Firmware Specification (Object library)
void GetObject(void* KeyHandle, unsigned int ObjectIndex, unsigned int ObjectSubIndex, unsigned int NbOfBytesToRead)
{
    unsigned int pErrorCode;
    unsigned int pNbOfBytesRead;
    void* PData;
    
    auto getObject= VCS_GetObject(KeyHandle, 1, ObjectIndex, ObjectSubIndex, &PData,  NbOfBytesToRead, &pNbOfBytesRead, &pErrorCode);
//    if (!getObject)
//        cout<<"Get Object Error: "<<pErrorCode<<endl;
//    cout<<"Object Data: "<<PData<<endl;
//    //    cout<<"Number of Bytes read: "<< pNbOfBytesRead<<endl;
}

//Set object: all objects can be found in Firmware Specification (Object library)
void SetObject(void* KeyHandle, unsigned int ObjectIndex, unsigned int ObjectSubIndex, unsigned int Data, unsigned int NbOfBytesToWrite)
{
    unsigned int pErrorCode;
    unsigned int pNbOfBytesWritten;
    
    auto setObject= VCS_SetObject(KeyHandle, 1, ObjectIndex, ObjectSubIndex, &Data, sizeof(Data), &pNbOfBytesWritten, &pErrorCode);
//    if (!setObject)
//        cout<<"Set Object Error: "<<pErrorCode<<endl;
//    cout<<"New Object Data: "<<Data<<endl;
//    cout<<"Number of Bytes written: "<< pNbOfBytesWritten<<endl;
}


int main(int argc, char** argv)
{
    HeartrateController hrc;

    hrc.readCmdLineArguments(argc, argv);

    if ( hrc.cmdLineParser().feature("-h") ) {hrc.printUsage(); exit(0);}
        
    hrc.setup();
    
    hrc.setMotionModeArgsFromCmdLine();

    hrc.activateMotionMode();
    
//        SetObject(hrc.keyHandle(), 0x6062,0x00,0,4);
//        GetObject(hrc.keyHandle(), 0x6064,0x00,4);
//        GetObject(hrc.keyHandle(), 0x6062,0x00,4);
    
    //hrc.run();
    
    unsigned int n(1);
    if ( hrc.cmdLineParser().map().count("-n") > 0 ) n = hrc.cmdLineParser()["-n"];
    for (unsigned int i(0); i < n; ++i) hrc.run();
    
    
    hrc.printPosition();
}
