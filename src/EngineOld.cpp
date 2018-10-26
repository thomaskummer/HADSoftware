//============================================================================
// Name        : Engine.cpp
// Author      : Thomas Kummer
//============================================================================

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

enum EAppMode
{
    AM_UNKNOWN,
    AM_DEMO,
    AM_INTERFACE_LIST,
    AM_PROTOCOL_LIST,
    AM_VERSION_INFO
};

using namespace std;

void* g_pKeyHandle = 0;
unsigned short g_usNodeId = 1;
string g_deviceName;
string g_protocolStackName;
string g_interfaceName;
string g_portName;
int g_baudrate = 0;
EAppMode g_eAppMode = AM_DEMO;

const string g_programName = "HelloEposCmd";

#ifndef MMC_SUCCESS
    #define MMC_SUCCESS 0
#endif

#ifndef MMC_FAILED
    #define MMC_FAILED 1
#endif

#ifndef MMC_MAX_LOG_MSG_SIZE
    #define MMC_MAX_LOG_MSG_SIZE 512
#endif


class Engine
{
public:
    
    Engine()
    {
        setup();
    }
    
    ~Engine(){}
    
    void setup()
    {
        close();
        open();
    }
    
    void setProtocolStackSettings()
    {
        unsigned int errorCode;
        auto state = VCS_SetProtocolStackSettings(m_keyHandle, 1e6, 500, &errorCode); //baud rate[bit/s], timeout[ms]
        printError("VCS_SetProtocolStackSettings", state, errorCode);
    }
    
    void disableState()
    {
        unsigned int errorCode;
        auto state = VCS_SetDisableState(m_keyHandle, 1, &errorCode);
        printError("VCS_SetDisableState", state, errorCode);
    }
    
    void enableState()
    {
        // Set enable state
        unsigned int errorCode;
        auto state = VCS_SetEnableState(m_keyHandle, 1, &errorCode);
        printError("VCS_SetEnableState", state, errorCode);
    }
    
    void addMotor()
    {
        unsigned int errorCodeType;
        auto stateType = VCS_SetMotorType(m_keyHandle, 1, 10, &errorCodeType); //Node ID of adressed device, Type of motor (doc table)
        printError("VCS_SetMotorType", stateType, errorCodeType);

    
        // Function argument units (mA, mA, s/10)
        unsigned int errorCodeParameter;
        auto stateParameter = VCS_SetEcMotorParameter(m_keyHandle, 1, 6210, 10000, 310, 1, &errorCodeParameter); //Node ID, max. continous current, max. peak current, thermal time constant winding, number of pole pairs, values can be accessed using VCS_GetEcMotorParameter
        printError("VCS_SetEcMotorParameter", stateParameter, errorCodeParameter);
    }
    
    void addSensors()
    {
        unsigned int errorCodeType;
        auto sensorType = VCS_SetSensorType(m_keyHandle, 1, 1, &errorCodeType); //Node ID, sensor type
        printError("VCS_SetSensorType", sensorType, errorCodeType);

        unsigned int errorCodeParam;
        auto sensorParam = VCS_SetIncEncoderParameter(m_keyHandle, 1, 500, 0, &errorCodeParam); //Node ID, Encoder pulse number, sensor polarity
        printError("VCS_SetIncEncoderParameter", sensorParam, errorCodeParam);
    }
    
    void setRegulatorGains()
    {
        unsigned int errorCodePosition;
        auto statePosition = VCS_SetPositionRegulatorGain(m_keyHandle, 1, 798, 3151, 1078, &errorCodePosition); //node ID, position regulator P-gain,..I-gain,..D-gain
        printError("VCS_SetPositionRegulatorGain", statePosition, errorCodePosition);

        unsigned int errorCodeVelocity;
        auto stateVelocity = VCS_SetVelocityRegulatorGain(m_keyHandle, 1, 2874, 532, &errorCodeVelocity); //node ID, P-gain, I-gain
        printError("VCS_SetVelocityRegulatorGain", stateVelocity, errorCodeVelocity);

        unsigned int errorCodeCurrent;
        auto stateCurrent = VCS_SetCurrentRegulatorGain(m_keyHandle, 1, 335, 60, &errorCodeCurrent); //node ID, P-gain, I-gain
        printError("VCS_SetCurrentRegulatorGain", stateCurrent, errorCodeCurrent);

    }
    
    void activateProfilePositionMode()
    {
        unsigned int errorCodeActivate;
        auto stateActivate = VCS_ActivateProfilePositionMode(m_keyHandle, 1, &errorCodeActivate);
        printError("VCS_ActivateProfilePositionMode", stateActivate, errorCodeActivate);

        unsigned int errorCodeSet;
        auto stateSet = VCS_SetPositionProfile(m_keyHandle, 1, 1900, 9300, 9300, &errorCodeSet); // Node ID, position profile velocity,..acceleration,..deceleration
        printError("VCS_SetPositionProfile", stateSet, errorCodeSet);
    }
    
    void runProfilePositionMode()
    {
        unsigned int errorCode;
        bool absoluteMovement = false;
        bool immediately = false;
        auto moveToPos1 = VCS_MoveToPosition(m_keyHandle, 1, -5000, absoluteMovement, immediately, &errorCode);
        auto moveToPos2 = VCS_MoveToPosition(m_keyHandle, 1, 5000, absoluteMovement, immediately, &errorCode);
        
        std::cout << moveToPos1 << " " << moveToPos2 << " " << errorCode << std::endl;
        this_thread::sleep_for(chrono::milliseconds(5000));
    }
    
    void activatePositionMode()
    {
        unsigned int errorCodeActivate;
        auto stateActivate = VCS_ActivatePositionMode(m_keyHandle, 1, &errorCodeActivate);
        printError("VCS_ActivatePositionMode", stateActivate, errorCodeActivate);
    }
    
    void runPositionMode()
    {
        long double t(0);
        int dt (100);
        while (t < 100000)
        {
            std::cout << "t = " << t << " ms" << std::endl;

            long double position = 5000 * std::pow(std::sin(t/800*3.14), 2);
            std::cout << position << std::endl;
            
            unsigned int errorCode (0);
            auto setPosMust = VCS_SetPositionMust(m_keyHandle, 1, position, &errorCode);
            printError("VCS_SetPositionMust", setPosMust, errorCode);

	    unsigned int StateErrorCode (0);
	    int TargetReached;
	    auto MovementState=VCS_GetMovementState(m_keyHandle, 1, &TargetReached, &StateErrorCode);
	    std::cout << " Target reached " << TargetReached  << std::endl;
            printError("VCS_GetMovementState", MovementState , StateErrorCode);
	    std::cout << "StateErrorCode " << StateErrorCode << std::endl;
	    std::cout << "StateErrorCode " << StateErrorCode << std::endl;
	             
            this_thread::sleep_for(chrono::milliseconds(dt));
            t += dt;
        }
    }
    
    void activateInterpolatedPositionMode()
    {
        unsigned int errorCodeActivate;
        auto stateActivate = VCS_ActivateInterpolatedPositionMode(m_keyHandle, 1, &errorCodeActivate);
        printError("VCS_ActivateInterpolatedPositionMode", stateActivate, errorCodeActivate);

        unsigned int errorIpmBuffer;
        // auto setIpmBuffer = VCS_SetIpmBufferParameter(m_keyHandle, 1, -100, 100, &errorIpmBuffer);
        auto stateIpmBuffer = VCS_ClearIpmBuffer(m_keyHandle, 1, &errorIpmBuffer);
        printError("VCS_ClearIpmBuffer", stateIpmBuffer, errorIpmBuffer);
    }
    
    void runInterpolatedPositionMode()
    {
        unsigned int errorCode;
        double dt = 500;
        for (double t (0); t < 2000; t +=dt)
        {
            std::cout << "t = " << t << "   ";
            double position = 10000 * std::sin(t/800*3.14);
            double velocity = 10000 * 3.14 / 800 * std::cos(t/800*3.14);
            auto addPvt = VCS_AddPvtValueToIpmBuffer(m_keyHandle, 1, position, velocity, t, &errorCode);
            std::cout << addPvt << " " << errorCode << std::endl;
        }
        
        auto startIpmTraj = VCS_StartIpmTrajectory(m_keyHandle, 1, &errorCode);
        std::cout << startIpmTraj << " " << errorCode << std::endl;
    }

    
protected:
    
    void* m_keyHandle;
    
    void close()
    {
        unsigned int errorCode = 0;
        auto state = VCS_CloseAllDevices(&errorCode);
        printError("VCS_CloseAllDevices", state, errorCode);
    }
    
    void open()
    {
        std::string deviceNameStr = "EPOS2";
        char* deviceNameCharPtr = const_cast<char*> (deviceNameStr.c_str());
        
        std::string protocolStackNameStr = "MAXON SERIAL V2";
        char* protocolStackNameCharPtr = const_cast<char*> (protocolStackNameStr.c_str());
        
        std::string interfaceNameStr = "USB";
        char* interfaceNameCharPtr = const_cast<char*> (interfaceNameStr.c_str());
        
        std::string portNameStr = "USB0";
        char* portNameCharPtr = const_cast<char*> (portNameStr.c_str());
        
        unsigned int errorCode = 0;
        m_keyHandle = VCS_OpenDevice(deviceNameCharPtr, protocolStackNameCharPtr, interfaceNameCharPtr, portNameCharPtr, &errorCode);
        printError("VCS_OpenDevice", m_keyHandle, errorCode);
    }
    
    template <class inputType, class errorCodeType>
    void printError (std::string functionType, inputType state, errorCodeType errorCode)
    {
        if (state)
        {
            std::cout << "\n" << functionType << " successfull";
        }
        else
        {
            std::cout << "\n" << functionType << " unsuccessful";
            std::cout << "   -->  error code: " << errorCode;
        }
    }
    
};
