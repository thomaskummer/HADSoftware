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
#include <iomanip>
#include <vector>

#include "CommandLineParser.hpp"
#include "InputFileParser.hpp"
#include "GUIThreadParser.hpp"
#include "MotionMode.hpp"
#include "ProfileMode.hpp"
#include "ProfileVelocityMode.hpp"
#include "InterpolatedPositionMode.hpp"


#define mm * -1600


namespace HeartrateControllerSpace {

class HeartrateController {
public:
    
    HeartrateController() : m_absPos(0)
    {
        std::cout << "\n====================================================" << std::endl;
        std::cout << "Heartrate-controller for Maxon EPOS2 70/10" << std::endl;
        std::cout << "Institute of fluid dynamics - ETH Zurich" << std::endl;
        std::cout << "====================================================\n" << std::endl;
    }
    
    virtual ~HeartrateController(){}
    
    void readCmdLineArguments(int argc, char** argv)
    {
        m_clp = CommandLineParser();
        m_clp.readCommandLine(argc, argv);
    }
    
    void setup()
    {
        if ( m_clp.feature("-re") ) {reset(); exit(0);}
        
        if ( m_clp.feature("-h") ) {printUsage(); exit(0);}
        
        close();
        open();
        
        clearFault();
        setParameters();
        getDeviceErrorCode();
    }
    
    void setMotionMode(const std::string& motionMode)
    {
        // PRINT_FACTORY(MotionMode);
        m_motionMode = CREATE(MotionMode, motionMode);
        m_motionMode->setKeyHandle(KeyHandle);
    }
    
    void setMotionModeFromCmdLine()
    {
        // PRINT_FACTORY(MotionMode);

        std::string motionMode;
        if ( m_clp.feature("-pm") ) motionMode = "ProfileMode";
        if ( m_clp.feature("-ipm") ) motionMode = "InterpolatedPositionMode";

        m_motionMode = CREATE(MotionMode, motionMode);
        std::cout << "Motion mode set to '" << motionMode << "'" << std::endl;

        m_motionMode->setKeyHandle(KeyHandle);
        
        m_motionMode->setArguments(m_clp.map());
    }
    
    void activateMotionMode()
    {
        m_motionMode->activateMode();
    }
    
    void run()
    {
        int absPos = PositionIs_Fct();
        unsigned int n(1);
        if ( m_clp.feature("-n") ) n = m_clp["-n"];
        for (unsigned int i(0); i < n; ++i) m_motionMode->run(absPos);
    }

    void printPosition()
    {
        unsigned int PositionIsError;
        int PositionIs;
        auto GetPositionIs = VCS_GetPositionIs(KeyHandle, 1, &PositionIs, &PositionIsError);
        // std::cout << GetPositionIs << " " << PositionIsError <<" Position: " << PositionIs << std::endl;
        std::cout << "Motion completed - new coordinate is " << PositionIs << std::endl;
    }
    
    void runConrollerFromCmdLine()
    {
        setup();
        
        setMotionModeFromCmdLine();
        
        activateMotionMode();
        
        run();
        
        printPosition();
    }
    
    void runControllerFromGUI()
    {
        setup();

        GUIThreadParser gtp;
        std::thread gtpThread( &GUIThreadParser::readGUIActions , &gtp ); // std::thread gtpThread( (GUIThreadParser()) );
        
        sleep(1);
        
        while (gtp.waitingForInput())
        {
            
            if ( gtp.taskSubmitted() )
            {
                gtp.taskReceived();
                
                // move
                if ( gtp.interface(0) )
                {
                    setMotionMode("ProfileMode");
                    m_motionMode->setArguments(gtp.map());
                    activateMotionMode();
                    run();
                    gtp.interface(0) = 0;
                }
                
                // ipm
                if ( gtp.interface(2) )
                {
                    setMotionMode("InterpolatedPositionMode");
                    m_motionMode->setArguments(gtp.map());
                    activateMotionMode();
                    while (gtp.keepRunning()) run();
                    gtp.interface(2) = 0;
                }
                
                // continuous-plus-push
                if ( gtp.interface(8) )
                {
                    setMotionMode("ProfileVelocityMode");
                    m_motionMode->setArguments(gtp.map());
                    activateMotionMode();
                    run();
                    gtp.interface(8) = 0;
                }
                
                // continuous-minus-push & home
                if ( gtp.interface(12) )
                {
                    setMotionMode("ProfileVelocityMode");
                    m_motionMode->setArguments(gtp.map());
                    activateMotionMode();
                    run();
                    gtp.interface(8) = 0;
                }
                
                // continuous-plus-release & continuous-minus-release
                if ( gtp.interface(10) )
                {
                    unsigned int pErrorMoveToPos;
                    bool halt = VCS_HaltVelocityMovement(keyHandle(), 1, &pErrorMoveToPos);
                    gtp.interface(10) = 0;
                }
                    
                // help
                if ( gtp.interface(4) )
                {
                    printInteractiveHelp();
                    gtp.interface(4) = 0;
                }
                
                // reset
                if ( gtp.interface(6) )
                {
                    reset();
                    gtp.interface(6) = 0;
                }

            }
            
            if ( gtp.keepRunning() )
            {
                if ( sensorOne() || sensorTwo() )
                {
                    std::cout << "sensor state: [ " << sensorOne() << " : " << sensorTwo() << " ]" << std::endl;
                    unsigned int pErrorMoveToPos;
                    
                    char* pOperationMode;
                    bool om = VCS_GetOperationMode(keyHandle(), 1, pOperationMode, &pErrorMoveToPos);
                    
                    if ( *pOperationMode == '1' )
                    {
                        bool pmStop = VCS_HaltPositionMovement(keyHandle(), 1, &pErrorMoveToPos);
                    }

                    if ( *pOperationMode == '3' )
                    {
                        bool vmStop = VCS_HaltVelocityMovement(keyHandle(), 1, &pErrorMoveToPos);
                        gtp["-vs"] = 0.;
                    }

                    if ( *pOperationMode == '7' )
                    {
                        bool ipmStop = VCS_StopIpmTrajectory(keyHandle(), 1, &pErrorMoveToPos);
                    }
                    
                    gtp.keepRunning() = false;
                }
            }
            
            sleep (0.01);
        }

        gtpThread.join();
    }

    
    void* keyHandle()
    {
        return KeyHandle;
    }

    MotionMode* motionMode()
    {
        return m_motionMode;
    }
    
    const CommandLineParser& cmdLineParser()
    {
        return m_clp;
    }
    
    void reset()
    {
        unsigned int pErrorCode;
        if(VCS_ResetDevice(KeyHandle,1,&pErrorCode))
            std::cout << "device reset" << std::endl;
        else
            std::cout << "reset error: " << pErrorCode<<std::endl;
        
        setup();
    }

    void printUsage()
    {
        std::cout << "HeartrateController help" << std::endl;
        std::cout << "\t-h   : this help" << std::endl;
        std::cout << "\t-n   : [ARG] number of program repetitions (default 1)" << std::endl;
        std::cout << "\t-pm  : profile mode"  << std::endl;
        std::cout << "\t-pd    : [ARG] distance to move (in mm, default 10)"  << std::endl;
        std::cout << "\t-ipm : interpolated position mode"  << std::endl;
        std::cout << "\t-if    : [ARG] motion type (0 for sin(t), 1 for sin^2(t), default 0)" << std::endl;
        std::cout << "\t-ia    : [ARG] amplitude (in mm, default 20)" << std::endl;
        std::cout << "\t-ip    : [ARG] period, time for one contraction (in ms, default 1000)" << std::endl;
        std::cout << "\n\texample: ./HeartrateController -ipm if 1 -ia -40 -n 2"  << std::endl;
    }
    
    void printInteractiveHelp()
    {
        std::cout << ">>> HeartrateController help" << std::endl;
        std::cout << "\tmove  : ARG1 is the distance in mm, if \n\t\tARG1 is set to 0, the piston \n\t\tmoves to the absolute origin." << std::endl;
        std::cout << "\tipm   : interpolated position mode. \n\t\tARG1 is the amplitude in mm (default -10), \n\t\tARG2 is the period in ms (default 1000), \n\t\tARG3 is the motion type with 0 for \n\t\tsin and 1 for sin^2 (default 1)." << std::endl;
        std::cout << "\tstop  : terminates motion when cycle is finished."  << std::endl;
        std::cout << "\texit  : exits this control tool."  << std::endl;
    }
    
    
protected:

    void* KeyHandle;
    Positions MainPositions={-500000, 500000, 0};
    
    MotionMode* m_motionMode;
    CommandLineParser m_clp;

    int m_absPos;
    
    // Close all devices
    void close()
    {
        unsigned int pErrorCode = 0;
        auto closed = VCS_CloseAllDevices(&pErrorCode);
        //std::cout << closed << " " << pErrorCode << std::endl;
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
        //std::cout << KeyHandle << " " << pErrorCodeOpen << std::endl;
        std::cout << "EPOS2-Controller launched" << std::endl;
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
        //std::cout << protocol << " " << pErrorCodeProtocol << std::endl;
        
        // Disable state
        unsigned int pErrorDisableState;
        VCS_SetDisableState(KeyHandle, 1, &pErrorDisableState);
        
        // Set motor type
        unsigned int pErrorCodeMotor;
        auto motorType = VCS_SetMotorType(KeyHandle, 1, 10, &pErrorCodeMotor);
        //std::cout << motorType << " " << pErrorCodeMotor << std::endl;
        
        // Set motor parameters !! units unsure (mA, mA, s/10)
        unsigned int pErrorEc;
        auto ecMotor = VCS_SetEcMotorParameter(KeyHandle, 1, 6210, 10000, 310, 1, &pErrorEc);
        //std::cout << ecMotor << " " << pErrorEc << std::endl;
        
        // Set sensor type
        unsigned int pErrorSensorType;
        auto sensorType = VCS_SetSensorType(KeyHandle, 1, 1, &pErrorSensorType);
        //std::cout << sensorType << " " << pErrorSensorType << std::endl;
        
        // Set sensor parameter
        unsigned int pErrorSensorParam;
        auto sensorParam = VCS_SetIncEncoderParameter(KeyHandle, 1, 500, 0, &pErrorSensorParam);
        //std::cout << sensorParam << " " << pErrorSensorParam << std::endl;
        
        // Set position regulator gain
        unsigned int pErrorPosRegGain;
        auto positionRegGain = VCS_SetPositionRegulatorGain(KeyHandle, 1, 798, 3151, 1078, &pErrorPosRegGain);
        //std::cout << positionRegGain << " " << pErrorPosRegGain << std::endl;
        
        // Set sensor parameter
        unsigned int pErrorVelRegGain;
        auto velocityRegGain = VCS_SetVelocityRegulatorGain(KeyHandle, 1, 2874, 532, &pErrorVelRegGain);
        //std::cout << velocityRegGain << " " << pErrorVelRegGain << std::endl;
        
        // Set sensor parameter
        unsigned int pErrorCurrentRegGain;
        auto currentRegGain = VCS_SetCurrentRegulatorGain(KeyHandle, 1, 335, 60, &pErrorCurrentRegGain);
        //std::cout << currentRegGain << " " << pErrorCurrentRegGain << std::endl;
        
        // Set enable state
        unsigned int pErrorState;
        auto state = VCS_SetEnableState(KeyHandle, 1, &pErrorState);
        //std::cout << state << " " << pErrorState << std::endl;
        
        std::cout << "EPOS2-Controller parameters set" << std::endl;
        std::cout << "Current absolut position: " << PositionIs_Fct() << std::endl;

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
        if (static_cast<signed>(pNbDeviceError) > 0)
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
    
    //integration of limit switch
    unsigned short GetLimitReached()
    {
        //=====================================================
        //DigitalInput is a 16 bit value for the 16 input pins  of the position controller.
        //If any input is detected on a pin, the corresponding bit is written as a 1, otherwise as a zero.
        //To check for all inputs if they are activated or not, bit masking is used.
        //=====================================================
        
        unsigned short DigitalInput7;
        unsigned short DigitalInput8;
        unsigned int ErrorInput;
        unsigned short DigitalInput;
        auto GetDigitalInput=VCS_GetAllDigitalInputs(KeyHandle, 1, &DigitalInput, &ErrorInput);
        DigitalInput7 = DigitalInput & 0x100;
        DigitalInput8 = DigitalInput & 0x200;
        if (!GetDigitalInput) std::cout <<"Digital Input, Error State:" << GetDigitalInput << ", " << ErrorInput << std::endl;
        //std::cout <<"Input 7: " << DigitalInput7 <<" ,  Input 8: "<<DigitalInput8<< std::endl;
        
        return (DigitalInput);
    }
    
    //Minimal Position reached?
    bool sensorOne()
    {
        unsigned short LimitReached = GetLimitReached();
        LimitReached = LimitReached & 0x100;
        return (LimitReached);
    }
    
    //Maximal Position reached?
    bool sensorTwo()
    {
        unsigned short LimitReached = GetLimitReached();
        LimitReached = LimitReached & 0x200;
        return (LimitReached);
    }
    
};


}


#endif /* HeartrateController_hpp */

