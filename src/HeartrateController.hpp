//
//  HeartrateController.hpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#ifndef HeartrateController_hpp
#define HeartrateController_hpp

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdio.h>

#include "CommandLineParser.hpp"
#include "InputFileParser.hpp"
#include "MotionMode.hpp"
#include "ProfileMode.hpp"
#include "InterpolatedPositionMode.hpp"


#define mm * -1600


// namespace HADController {

class HeartrateController {
public:
    
    HeartrateController(){}
    
    virtual ~HeartrateController(){}
    
    void readCmdLineArguments(int argc, char** argv)
    {
        m_clp = CommandLineParser(argc, argv);
    }
    
    void setup()
    {
        close();
        open();
        
        clearFault();
        setParameters();
        getDeviceErrorCode();
    }
    
    void setMotionMode(const std::string& motionMode)
    {
        PRINT_FACTORY(MotionMode);
        m_motionMode = CREATE(MotionMode, motionMode);
        m_motionMode->setKeyHandle(KeyHandle);
    }
    
    void setMotionModeFromCmdLine()
    {
        PRINT_FACTORY(MotionMode);

        if ( m_clp.map().count("-pm") > 0 ) m_motionMode = CREATE(MotionMode, "ProfileMode");
        if ( m_clp.map().count("-ipm") > 0 ) m_motionMode = CREATE(MotionMode, "InterpolatedPositionMode");

        m_motionMode->setKeyHandle(KeyHandle);
        
        m_motionMode->readArguments(m_clp.map());
    }
    
    void activateMotionMode()
    {
        m_motionMode->activateMode();
    }
    
    void run()
    {
        m_motionMode->run();
    }

    void printPosition()
    {
        unsigned int PositionIsError;
        int PositionIs;
        auto GetPositionIs = VCS_GetPositionIs(KeyHandle, 1, &PositionIs, &PositionIsError);
        std::cout << GetPositionIs << " " << PositionIsError <<" Position: " << PositionIs << std::endl;
    }
    
    void* keyHandle()
    {
        return KeyHandle;
    }

    MotionMode* motionMode()
    {
        return m_motionMode;
    }
    
    
protected:
    
    void* KeyHandle;
    Positions MainPositions={-500000, 500000, 0};
    
    MotionMode* m_motionMode;
    CommandLineParser m_clp;

    
    // Close all devices
    void close()
    {
        unsigned int pErrorCode = 0;
        auto closed = VCS_CloseAllDevices(&pErrorCode);
        std::cout << closed << " " << pErrorCode << std::endl;
    }
    
    // Open device
    void open()
    {
        unsigned int pErrorCodeOpen;
        
        std::string deviceNameStr = "EPOS2";
        char* deviceNameCharPtr = const_cast<char*> (deviceNameStr.c_str());
        std::string protocolStackNameStr = "MAXON SERIAL V2";
        char* protocolStackNameCharPtr = const_cast<char*> (protocolStackNameStr.c_str());
        std::string interfaceNameStr = "USB";
        char* interfaceNameCharPtr = const_cast<char*> (interfaceNameStr.c_str());
        std::string portNameStr = "USB0";
        char* portNameCharPtr = const_cast<char*> (portNameStr.c_str());
        
        KeyHandle = VCS_OpenDevice(deviceNameCharPtr, protocolStackNameCharPtr, interfaceNameCharPtr, portNameCharPtr, &pErrorCodeOpen);
        std::cout << KeyHandle << " " << pErrorCodeOpen << std::endl;
    }
    
    //clear fault state (red LED==unresponisve state)
    void clearFault()
    {
        unsigned int pErrorCode;
        auto clearFault = VCS_ClearFault(KeyHandle, 1, &pErrorCode);
        if (!clearFault) std::cout<<"Clear Fault Error: "<<pErrorCode<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    }
        void setParameters()
    {
        // Set protocol stack settings
        unsigned int pErrorCodeProtocol;
        auto protocol = VCS_SetProtocolStackSettings(KeyHandle, 1e6, 500, &pErrorCodeProtocol);
        std::cout << protocol << " " << pErrorCodeProtocol << std::endl;
        
        // Disable state
        unsigned int pErrorDisableState;
        VCS_SetDisableState(KeyHandle, 1, &pErrorDisableState);
        
        // Set motor type
        unsigned int pErrorCodeMotor;
        auto motorType = VCS_SetMotorType(KeyHandle, 1, 10, &pErrorCodeMotor);
        std::cout << motorType << " " << pErrorCodeMotor << std::endl;
        
        // Set motor parameters !! units unsure (mA, mA, s/10)
        unsigned int pErrorEc;
        auto ecMotor = VCS_SetEcMotorParameter(KeyHandle, 1, 6210, 10000, 310, 1, &pErrorEc);
        std::cout << ecMotor << " " << pErrorEc << std::endl;
        
        // Set sensor type
        unsigned int pErrorSensorType;
        auto sensorType = VCS_SetSensorType(KeyHandle, 1, 1, &pErrorSensorType);
        std::cout << sensorType << " " << pErrorSensorType << std::endl;
        
        // Set sensor parameter
        unsigned int pErrorSensorParam;
        auto sensorParam = VCS_SetIncEncoderParameter(KeyHandle, 1, 500, 0, &pErrorSensorParam);
        std::cout << sensorParam << " " << pErrorSensorParam << std::endl;
        
        // Set position regulator gain
        unsigned int pErrorPosRegGain;
        auto positionRegGain = VCS_SetPositionRegulatorGain(KeyHandle, 1, 798, 3151, 1078, &pErrorPosRegGain);
        std::cout << positionRegGain << " " << pErrorPosRegGain << std::endl;
        
        // Set sensor parameter
        unsigned int pErrorVelRegGain;
        auto velocityRegGain = VCS_SetVelocityRegulatorGain(KeyHandle, 1, 2874, 532, &pErrorVelRegGain);
        std::cout << velocityRegGain << " " << pErrorVelRegGain << std::endl;
        
        // Set sensor parameter
        unsigned int pErrorCurrentRegGain;
        auto currentRegGain = VCS_SetCurrentRegulatorGain(KeyHandle, 1, 335, 60, &pErrorCurrentRegGain);
        std::cout << currentRegGain << " " << pErrorCurrentRegGain << std::endl;
        
        // Set enable state
        unsigned int pErrorState;
        auto state = VCS_SetEnableState(KeyHandle, 1, &pErrorState);
        std::cout << state << " " << pErrorState << std::endl;
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
                std::cout<<"GetErrorCode Error: "<<pErrorCode<<std::endl;
            std::cout<<"Device Error Code: "<<pDeviceErrorCode<<std::endl;
        }
    }

    //Return Number of Device Errors
    unsigned char GetNbDeviceError()
    {
        unsigned int pErrorCode;
        unsigned char pNbDeviceError;
        auto ErrorNb= VCS_GetNbOfDeviceError(KeyHandle, 1, &pNbDeviceError, &pErrorCode);
        if(!ErrorNb)
            std::cout<<"Error getting Number of Device Error: "<<pErrorCode<<std::endl;
        std::cout<<"Number of Device Error: "<<static_cast<signed>(pNbDeviceError)<<std::endl;
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


#endif /* HeartrateController_hpp */
