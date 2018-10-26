//
//  HADController.hpp
//  HADSoftware
//
//  Created by Thomas Kummer on 26.10.18.
//  Copyright © 2018 Institute of Fluid Dynamics. All rights reserved.
//

#ifndef HADController_hpp
#define HADController_hpp



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

typedef void* HANDLE;
typedef int BOOL;




#include <stdio.h>


class HADController
{
public:
    
    HADController(){}
    ~HADController(){}
    
    void setup()
    {
        clearFault();
        setParameters();
        getDeviceErrorCode();
    }
    
    void move(const int& distance)
    {
        moveCentimeter(distance);
        printPosition();
    }
    
    
protected:
  
    void* KeyHandle;

    void setParameters()
    {
        // Set protocol stack settings
        unsigned int pErrorCodeProtocol;
        auto protocol = VCS_SetProtocolStackSettings(KeyHandle, 1e6, 500, &pErrorCodeProtocol);
        cout << protocol << " " << pErrorCodeProtocol << endl;
        
        // Disable state
        unsigned int pErrorDisableState;
        VCS_SetDisableState(KeyHandle, 1, &pErrorDisableState);
        
        // Set motor type
        unsigned int pErrorCodeMotor;
        auto motorType = VCS_SetMotorType(KeyHandle, 1, 10, &pErrorCodeMotor);
        cout << motorType << " " << pErrorCodeMotor << endl;
        
        // Set motor parameters !! units unsure (mA, mA, s/10)
        unsigned int pErrorEc;
        auto ecMotor = VCS_SetEcMotorParameter(KeyHandle, 1, 6210, 10000, 310, 1, &pErrorEc);
        cout << ecMotor << " " << pErrorEc << endl;
        
        // Set sensor type
        unsigned int pErrorSensorType;
        auto sensorType = VCS_SetSensorType(KeyHandle, 1, 1, &pErrorSensorType);
        cout << sensorType << " " << pErrorSensorType << endl;
        
        // Set sensor parameter
        unsigned int pErrorSensorParam;
        auto sensorParam = VCS_SetIncEncoderParameter(KeyHandle, 1, 500, 0, &pErrorSensorParam);
        cout << sensorParam << " " << pErrorSensorParam << endl;
        
        // Set position regulator gain
        unsigned int pErrorPosRegGain;
        auto positionRegGain = VCS_SetPositionRegulatorGain(KeyHandle, 1, 798, 3151, 1078, &pErrorPosRegGain);
        cout << positionRegGain << " " << pErrorPosRegGain << endl;
        
        // Set sensor parameter
        unsigned int pErrorVelRegGain;
        auto velocityRegGain = VCS_SetVelocityRegulatorGain(KeyHandle, 1, 2874, 532, &pErrorVelRegGain);
        cout << velocityRegGain << " " << pErrorVelRegGain << endl;
        
        // Set sensor parameter
        unsigned int pErrorCurrentRegGain;
        auto currentRegGain = VCS_SetCurrentRegulatorGain(KeyHandle, 1, 335, 60, &pErrorCurrentRegGain);
        cout << currentRegGain << " " << pErrorCurrentRegGain << endl;
        
        // Set enable state
        unsigned int pErrorState;
        auto state = VCS_SetEnableState(KeyHandle, 1, &pErrorState);
        cout << state << " " << pErrorState << endl;
    }
    
    //activate Profile Position Mode
    void activateProfileMode()
    {
        unsigned int pErrorProfilePosMode;
        auto profilePosMode = VCS_ActivateProfilePositionMode(KeyHandle, 1, &pErrorProfilePosMode);
        cout << profilePosMode << " " << pErrorProfilePosMode << endl;
        
        // Set sensor parameter
        unsigned int pErrorSetPosProfile;
        auto posProfile = VCS_SetPositionProfile(KeyHandle, 1, 22000, 15000, 15000, &pErrorSetPosProfile); // Node ID, position profile velocity,..acceleration,..deceleration
        std::cout << posProfile << " " << pErrorSetPosProfile << std::endl;
    }
    
    //clear fault state (red LED==unresponisve state)
    void clearFault()
    {    unsigned int pErrorCode;
        auto clearFault = VCS_ClearFault(KeyHandle, 1, &pErrorCode);
        if (!clearFault)
            cout<<"Clear Fault Error: "<<pErrorCode<<endl;
        this_thread::sleep_for(chrono::milliseconds(4000));
    }
    
    void getDeviceErrorCode()
    {
        unsigned int pErrorCode;
        unsigned int pDeviceErrorCode;
        unsigned char DeviceNbError=GetNbDeviceError();
        for(unsigned int ErrorNumber=1; ErrorNumber<=DeviceNbError; ErrorNumber++)
        {
            auto GetErrorCode=VCS_GetDeviceErrorCode(KeyHandle, 1, ErrorNumber, &pDeviceErrorCode, &pErrorCode);
            if(!GetErrorCode)
                cout<<"GetErrorCode Error: "<<pErrorCode<<endl;
            cout<<"Device Error Code: "<<pDeviceErrorCode<<endl;
        }
    }
    
    //move cylinder backward. Move at least by 1000 at a time, works well (smaller steps won't be recognized)
    bool moveCentimeter(const int& distance)
    {
        int PositionIs = PositionIs_Fct();
        int NewPosition = PositionIs - distance;
        bool moved = SetPosition_ProfileMode(NewPosition);
        if(moved)
            return 1;
        else
            return 0;
    }
    
    //print position to console
    void printPosition()
    {
        unsigned int PositionIsError;
        int PositionIs;
        auto GetPositionIs = VCS_GetPositionIs(KeyHandle, 1, &PositionIs, &PositionIsError);
        cout << GetPositionIs << " " << PositionIsError <<" Position: " << PositionIs << endl;
    }

    //Return Device Error Code
    void GetDeviceErrorCode()
    {
        unsigned int pErrorCode;
        unsigned int pDeviceErrorCode;
        unsigned char DeviceNbError=GetNbDeviceError();
        for(unsigned int ErrorNumber=1; ErrorNumber<=DeviceNbError; ErrorNumber++)
        {
            auto GetErrorCode=VCS_GetDeviceErrorCode(KeyHandle, 1, ErrorNumber, &pDeviceErrorCode, &pErrorCode);
            if(!GetErrorCode)
                cout<<"GetErrorCode Error: "<<pErrorCode<<endl;
            cout<<"Device Error Code: "<<pDeviceErrorCode<<endl;
        }
    }
    
};

#endif /* HADController_hpp */
