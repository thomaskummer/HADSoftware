//
//  ProfileMode.hpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#ifndef ProfileMode_hpp
#define ProfileMode_hpp

#include <iostream>
#include <fstream>
#include <assert.h>


#define mm * -1600

struct Positions
{
    int Min;
    int Max;
    int Mid;
};

// namespace HADController {

class ProfileMode {
public:
    
    ProfileMode(){}
    
    virtual ~ProfileMode(){}
    
    void setup()
    {
        close();
        open();
        
        clearFault();
        setParameters();
        getDeviceErrorCode();
    }
    
    
    //print position to console
    void printPosition()
    {
        unsigned int PositionIsError;
        int PositionIs;
        auto GetPositionIs = VCS_GetPositionIs(KeyHandle, 1, &PositionIs, &PositionIsError);
        cout << GetPositionIs << " " << PositionIsError <<" Position: " << PositionIs << endl;
    }

    
protected:
    
    void* KeyHandle;
    Positions MainPositions={0, 205000, 137000};
    
    
    // Close all devices
    void close()
    {
        unsigned int pErrorCode = 0;
        auto closed = VCS_CloseAllDevices(&pErrorCode);
        cout << closed << " " << pErrorCode << endl;
    }
    
    // Open device
    void open()
    {
        unsigned int pErrorCodeOpen;
        
        string deviceNameStr = "EPOS2";
        char* deviceNameCharPtr = const_cast<char*> (deviceNameStr.c_str());
        string protocolStackNameStr = "MAXON SERIAL V2";
        char* protocolStackNameCharPtr = const_cast<char*> (protocolStackNameStr.c_str());
        string interfaceNameStr = "USB";
        char* interfaceNameCharPtr = const_cast<char*> (interfaceNameStr.c_str());
        string portNameStr = "USB0";
        char* portNameCharPtr = const_cast<char*> (portNameStr.c_str());
        
        KeyHandle = VCS_OpenDevice(deviceNameCharPtr, protocolStackNameCharPtr, interfaceNameCharPtr, portNameCharPtr, &pErrorCodeOpen);
        cout << KeyHandle << " " << pErrorCodeOpen << endl;
    }
    
    //clear fault state (red LED==unresponisve state)
    void clearFault()
    {
        unsigned int pErrorCode;
        auto clearFault = VCS_ClearFault(KeyHandle, 1, &pErrorCode);
        if (!clearFault) cout<<"Clear Fault Error: "<<pErrorCode<<endl;
        this_thread::sleep_for(chrono::milliseconds(4000));
    }
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

    //Return Number of Device Errors
    unsigned char GetNbDeviceError()
    {
        unsigned int pErrorCode;
        unsigned char pNbDeviceError;
        auto ErrorNb= VCS_GetNbOfDeviceError(KeyHandle, 1, &pNbDeviceError, &pErrorCode);
        if(!ErrorNb)
            cout<<"Error getting Number of Device Error: "<<pErrorCode<<endl;
        cout<<"Number of Device Error: "<<static_cast<signed>(pNbDeviceError)<<endl;
        return pNbDeviceError;
    }
    
    int PositionIs_Fct()
    {
        unsigned int PositionIsError;
        int PositionIs;
        auto GetPositionIs = VCS_GetPositionIs(KeyHandle, 1, &PositionIs, &PositionIsError);
        return PositionIs;
    }
    
};


// }


#endif /* ProfileMode_hpp */

