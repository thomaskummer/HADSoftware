//=================
//Engine
//Jonas Conrad
//=================

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

	struct Positions
	{
		int Min;
		int Max;
		int Mid;
	};
	Positions MainPositions={-60000,2100,-30000};

	struct PTV
	{
		int P, T, V;
	};

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
	
// SETUP FUNCTIONS======================================================================================================

	//call to set engine parameters
	void SetAll()
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
	
	//activate position mode
	void ActivatePositionMode()
	{
		unsigned int pErrorCodeMode;
		auto PositionMode = VCS_ActivatePositionMode(KeyHandle, 1, &pErrorCodeMode);
	}

	//activate Profile Position Mode
	void ActivateProfileMode()
	{
		unsigned int pErrorProfilePosMode;
		auto profilePosMode = VCS_ActivateProfilePositionMode(KeyHandle, 1, &pErrorProfilePosMode);
		cout << profilePosMode << " " << pErrorProfilePosMode << endl;

		// Set sensor parameter
		unsigned int pErrorSetPosProfile;
		auto posProfile = VCS_SetPositionProfile(KeyHandle, 1, 22000, 15000, 15000, &pErrorSetPosProfile); // Node ID, position profile velocity,..acceleration,..deceleration
		std::cout << posProfile << " " << pErrorSetPosProfile << std::endl;
	}

	//activate interpolated position mode
	void ActivateIPMode()
	{
		unsigned int pErrorActivate;
		unsigned int pErrorClearBuffer;
	
		auto IPMode = VCS_ActivateInterpolatedPositionMode(KeyHandle, 1, &pErrorActivate);
		if (!IPMode)
			cout<<"Activate IP Mode Error: "<<pErrorActivate<<endl;	
	
		// auto setIpmBuffer = VCS_SetIpmBufferParameter(KeyHandle, 1, -100, 100, &pErrorIntPosMode);
		auto ClearIpmBuffer = VCS_ClearIpmBuffer(KeyHandle, 1, &pErrorClearBuffer);
		if (!ClearIpmBuffer)
			cout<<"Clear Buffer Error: "<<pErrorClearBuffer<<endl;

	}

	void SetDigitalOutputs(unsigned int ActivationKey)
	{	
		unsigned int SetOutputError;
		auto SetOutputs = VCS_SetAllDigitalOutputs(KeyHandle, 1, ActivationKey, &SetOutputError);
		cout << SetOutputs << " " << SetOutputError << endl;	
	}

	//Set Operation Mode
	void SetOperationMode(signed int pMode)
	{
		unsigned int pErrorCode;

		auto SetMode = VCS_SetOperationMode(KeyHandle, 1, pMode, &pErrorCode);
	
		cout<<SetMode<<"-> SetMode "<<pErrorCode<<"-> ErrorCode"<<endl;
	}

	//Get and print out object: all objects can be found in Firmware Specification (Object library) 
	void GetObject(unsigned int ObjectIndex, unsigned int ObjectSubIndex, unsigned int NbOfBytesToRead)
	{
		unsigned int pErrorCode;
		unsigned int pNbOfBytesRead;
		void* PData;

		auto getObject= VCS_GetObject(KeyHandle, 1, ObjectIndex, ObjectSubIndex, &PData,  NbOfBytesToRead, &pNbOfBytesRead, &pErrorCode);
		if (!getObject)
			cout<<"Get Object Error: "<<pErrorCode<<endl;
		cout<<"Object Data: "<<PData<<endl;
	//	cout<<"Number of Bytes read: "<< pNbOfBytesRead<<endl;	
	}
	
	//Set object: all objects can be found in Firmware Specification (Object library) 
	void SetObject(unsigned int ObjectIndex, unsigned int ObjectSubIndex, unsigned int Data, unsigned int NbOfBytesToWrite)
	{
		unsigned int pErrorCode;
		unsigned int pNbOfBytesWritten;
		
		auto setObject= VCS_SetObject(KeyHandle, 1, ObjectIndex, ObjectSubIndex, &Data, sizeof(Data), &pNbOfBytesWritten, &pErrorCode);
		if (!setObject)
			cout<<"Set Object Error: "<<pErrorCode<<endl;
		cout<<"New Object Data: "<<Data<<endl;
		cout<<"Number of Bytes written: "<< pNbOfBytesWritten<<endl;		
	}

        //Set current position to new zero position (home position)
        bool SetCurrentToHome()
        {
            PrintPosition();
            int NewHome=PositionIs_Fct();
            SetObject(0x2081,0x00,NewHome,sizeof(NewHome));
            cout<<"Current Position is now new Home Position"<<endl;
            Reset();
            PrintPosition();
        }  



//MOVEMENT RANGE LIMITING FUNCTIONS===================================================================================================================

	//set Main positions (movement liomits) from current position (current position must be middle position) 
	void SetMainPositionsFromCurrent()
	{
		cout<<"Hit 'Enter' if the current positions is the Max position. Insert anything else if not to accept approximated values."<<endl;
		if (cin.get() == '\n')
		{
			MainPositions.Mid=PositionIs_Fct();//-30000;
			MainPositions.Min=PositionIs_Fct()-30000;
			MainPositions.Max=PositionIs_Fct()+30000;
			cout<<"Min: "<<MainPositions.Min<<" Mid: "<<MainPositions.Mid<<" Max: "<<MainPositions.Max<<endl;
		}
		else
		{
			MainPositions.Min=-60000;
			MainPositions.Max=100;
			MainPositions.Mid=30000;
			cout<<"APPROXIMATIONS:Min: "<<MainPositions.Min<<" Mid: "<<MainPositions.Mid<<" Max: "<<MainPositions.Max<<endl;
		}
		
	}
	
	//Find Main Positions (movement limits) using sensors, only to use in between sensors (Movement Range)
	bool SetMainPositions()
	{
		//set initial values for mains, not actual limits! (approximated)
		MainPositions.Min=-80000;
		MainPositions.Max=80000;
		cout<<"Cylinder in approved motion range (in between sensors)? If so, press ENTER to continue; if not, enter 'n' to abort.";
		if (cin.get() != '\n')
			return 0;
		
		bool AtMin=MinReached(GetLimitReached());
		bool AtMax=MaxReached(GetLimitReached());

		while (!AtMin)
		{
			MoveXBackward(1000); 
			AtMin=MinReached(GetLimitReached());
		}
		int PositionMin=PositionIs_Fct();

		while (!AtMax)
		{
			MoveXForward(1000);
			AtMax=MaxReached(GetLimitReached());
		}
		int PositionMax=PositionIs_Fct();

		int PositionMid=abs(PositionMax-PositionMin)/2+PositionMin;

		this->MainPositions = {PositionMin, PositionMax, PositionMid};
		cout<<"Min: "<<MainPositions.Min<<" Max: "<<MainPositions.Max<<" Mid: "<<MainPositions.Mid<<endl;
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
		cout <<"Digital Input Error State:" << GetDigitalInput << " " << ErrorInput << endl;
		cout <<"Input 7:" << DigitalInput7 <<" Input 8:"<<DigitalInput8<< endl;

		return (DigitalInput);
	}

	//Minimal Position reached?
	bool MinReached(unsigned short LimitReached)
	{
		LimitReached = LimitReached & 0x100;
		return (LimitReached);
	}
	
	//Maximal Position reached?
	bool MaxReached(unsigned short LimitReached)
	{
		LimitReached = LimitReached & 0x200;
		return (!LimitReached);
	}





//ERROR HANDLINg & STATE ANALYZING FUNCTIONS===========================================================================================================	
        
        //Reset Device
        void Reset()
        {
            unsigned int pErrorCode;
            if(VCS_ResetDevice(KeyHandle,1,&pErrorCode))
                cout <<"Device reset"<<endl;
            else
                cout<<"Reset Error: "<<pErrorCode<<endl;
        }     

        //Restore default parameters
        void Restore()
        {
            unsigned int pErrorCode;
            if(VCS_Restore(KeyHandle,1,&pErrorCode))
                    cout<<"Default Parameters restored"<<endl;
            else
                cout<<"Restore Error: "<<pErrorCode<<endl;
        }

	//clear fault state (red LED==unresponisve state)
	void ClearFault()
	{	unsigned int pErrorCode;
		auto clearFault = VCS_ClearFault(KeyHandle, 1, &pErrorCode);
		if (!clearFault)
			cout<<"Clear Fault Error: "<<pErrorCode<<endl;
		this_thread::sleep_for(chrono::milliseconds(4000));		
	}
	
	//Get Faul State Error Code
	void GetFaultState()
	{	
		unsigned int pErrorCode;
		int  pIsInFault;

		auto GetState= VCS_GetFaultState(KeyHandle, 1, &pIsInFault, &pErrorCode);
		if (!GetState)
			cout<<"Getting Fault State Error: "<<pErrorCode<<endl;
		if (pIsInFault)
			cout<<"Device in Fault State"<<endl;
		else 
			cout<<"Device not in Fault State"<<endl;

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
	

	//Read out current movement state
	void GetMovementState()
	{	
		unsigned int pErrorCode;
		int pTargetReached;

		auto State = VCS_GetMovementState(KeyHandle, 1, &pTargetReached, &pErrorCode);

		cout <<"State Function: "<<State<<" ErrorCode: "<<pErrorCode<<" Target Reached "<<pTargetReached<<endl;
	}	

	//print position to console
	void PrintPosition()
	{
		unsigned int PositionIsError;
		int PositionIs;
		auto GetPositionIs = VCS_GetPositionIs(KeyHandle, 1, &PositionIs, &PositionIsError);
		cout << GetPositionIs << " " << PositionIsError <<" Position: " << PositionIs << endl;
	}

	//give out position
	int PositionIs_Fct()
	{
		unsigned int PositionIsError;
		int PositionIs;
		auto GetPositionIs = VCS_GetPositionIs(KeyHandle, 1, &PositionIs, &PositionIsError);
		return PositionIs;
	}

	//Get status of interpolated position mode
	void IpmStatus()
	{
		int pTrajectoryRunning;
		int pIsUnderflowWarning;
		int pIsOverflowWarning;
		int pIsVelocityWarning;
		int pIsAccelerationWarning;
		int pIsUnderflowError;
		int pIsOverflowError;
		int pIsVelocityError;
		int pIsAccelerationError;
		unsigned int pErrorCode;
		
		auto GetStatus = VCS_GetIpmStatus(KeyHandle, 1, &pTrajectoryRunning, &pIsUnderflowWarning, &pIsOverflowWarning, &pIsVelocityWarning, &pIsAccelerationWarning, 
		&pIsUnderflowError, &pIsOverflowError, &pIsVelocityError, &pIsAccelerationError, &pErrorCode);

		cout<<GetStatus<<": Status "<<pTrajectoryRunning<<": TrajectoryRunning "<<pIsUnderflowWarning<<": UFlowWarnig "<<pIsOverflowWarning<<": OFlowWarning "
		<<pIsVelocityWarning<<": VelocityWarning "<<pIsAccelerationWarning<<": AccelerationWarning "<<pIsUnderflowError<<": UFlowError "<<pIsOverflowError<<": OFlowError "
		<<pIsVelocityError<<": VelocityError "<<pIsAccelerationError<<": AccelerationError "<<pErrorCode<<": ErrorCode"<<endl;
	}

	//Get buffer parameters for ipm
	void IpmBufferParameter()
	{
		unsigned short pUnderflowWarningLimit;
		unsigned short pOverflowWarningLimit;
		unsigned int pMaxBufferSize;
		unsigned int pErrorCode;	

		auto GetParameter = VCS_GetIpmBufferParameter(KeyHandle, 1, &pUnderflowWarningLimit, &pOverflowWarningLimit, &pMaxBufferSize, &pErrorCode);

		cout<<GetParameter<<": GetParameter "<<pUnderflowWarningLimit<<": UFlowWarninglimit "<<pOverflowWarningLimit<<": OFlowWarningLimit "
		<<pMaxBufferSize<<": MaxBufferSize "<<pErrorCode<<": ErrorCode"<<endl;
	} 
	
	//Get Operation Mode
	void GetOperationMode()
	{
		char pMode;
		unsigned int pErrorCode;

		auto GetMode = VCS_GetOperationMode(KeyHandle, 1, &pMode, &pErrorCode);
	
		cout<<GetMode<<"-> GetMode "<<static_cast<signed>(pMode)<<" -> Mode "<<pErrorCode<<"-> ErrorCode"<<endl;
	}

	//Get IPMode PTV
	PTV GetPTV(int Amplitude,int PointNumber,int Periode, int dt, int Resolution)
	{
		int P=(Amplitude*sin(PointNumber*dt*2*M_PI/Periode-M_PI/2))+Amplitude;
		int T=dt;
		int V=(Amplitude*2*M_PI/Periode*cos(PointNumber*dt*2*M_PI/Periode-M_PI/2))*1000/(4*Resolution)*60;
		
		cout<<"PointNumber: "<<PointNumber<<" P: "<<P<<" T: "<<T<<" V: "<<V<<endl;	
		return{P,T,V};
	}



//MOVEMENT FUNCTIONS===================================================================================================

	//wait until movement is finished; does not work with position mode!
	void Wait()
	{	
		unsigned int Timeout = 3000; //max waiting time in ms
		unsigned int pErrorCode;
	
		auto WaitForTarget= VCS_WaitForTargetReached(KeyHandle, 1, Timeout, &pErrorCode);
		if (!WaitForTarget)
			cout<<"Error in Wait Function! Error Code: "<<pErrorCode<<endl;
	}
		
	//move cylinder to position x
	void SetPosition(int position)
	{	
		//activate position mode
		unsigned int pErrorCodeMode;
		auto PositionMode = VCS_ActivatePositionMode(KeyHandle, 1, &pErrorCodeMode);
		
		unsigned int pErrorPosMust (0);
		if (position>=MainPositions.Min && position<=MainPositions.Max)
		{
			auto setPosMust = VCS_SetPositionMust(KeyHandle, 1, position, &pErrorPosMust);
			cout <<"Set Position: "<< setPosMust << " Error Code: " << pErrorPosMust << endl;
		}
		else	
			cout <<"Out of bounds! No further movement in this direction!";
		
		//GetMovementState();
		Wait();	
		//GetMovementState();
	}

	//move cylinder to x using ProfileMode (ActivateProfileMode must be called first)
	bool SetPosition_ProfileMode(int position)
	{
		// Move to position
		unsigned int pErrorMoveToPos;
		bool absoluteMovement = true;
		bool immediately = false;
		bool  MoveToPos;
	
		if (position>=MainPositions.Min && position<=MainPositions.Max)
		{
			MoveToPos = VCS_MoveToPosition(KeyHandle, 1, position, absoluteMovement, immediately, &pErrorMoveToPos);	
			cout <<"MoveToPos: "<< MoveToPos << " ErrorCode:  "  << pErrorMoveToPos << endl;
			Wait();
			PrintPosition();
			return 1;
		}
		else
		{	cout <<"Out of bounds! No further movement in this direction!";
		  	return 0;
		}
	}
	
	//move cylinder to x using IPMode (ActivateIPMode must be called first)
	bool SetPosition_IPMode(int position)
	{	
		unsigned int pErrorAddPvt;
		unsigned int pErrorStartTrajectory;
		unsigned int pErrorStopTrajectory;
	
		int dtPeak=200; //Timestep in Ms, 1<=dt<=255
		int PosInterval=500;
		double PeakVelocity=750;
		
		int CurrentPosition=PositionIs_Fct();
		int distance=abs(position-CurrentPosition);
		unsigned int Points=floor(distance/PosInterval);
		long double NewPosition;
		double velocity;
		int dt;
		if (CurrentPosition>position)
		{	
			PosInterval*=(-1);
			PeakVelocity*=(-1);
		}
		cout<<"#Points: "<<Points<<endl;		
	
		if (Points<1)
			cout<<"Not enough input points to create trajectory"<<endl;		

		

		for(int i=1; i<=Points; i=i+1)
		{
			NewPosition=CurrentPosition+i*PosInterval;
			velocity=PeakVelocity*sin(M_PI*i/Points);
		//	dt=abs(dtPeak*cos(M_PI*i/Points))+50; //1<=dt<=255
			dt=dtPeak;
			auto addPvt = VCS_AddPvtValueToIpmBuffer(KeyHandle, 1, NewPosition, velocity, dt, &pErrorAddPvt);
			if(!addPvt)
				cout<<"Add Point Error: "<<pErrorAddPvt<<endl;
			
			cout<<"NewPos: "<<NewPosition<<endl;
			cout<<"Velocity: "<<velocity<<endl;
			cout<<"timestep: "<<dt<<endl;			
		}	
	
		if (position>MainPositions.Min && position<MainPositions.Max)
		{
			auto StartIpmTraj = VCS_StartIpmTrajectory(KeyHandle, 1, &pErrorStartTrajectory);
			if (!StartIpmTraj)
			{
				cout<<"StartIPModeTrajectory Error: "<<pErrorStartTrajectory;
				return 1;
			}
			else 
				cout<<"Starting Trajectory"<<endl;
			Wait();
			this_thread::sleep_for(chrono::milliseconds(3000));		
			auto StopIpmTraj=VCS_StopIpmTrajectory(KeyHandle, 1, &pErrorStopTrajectory);
			if (!StopIpmTraj)
				cout<<"StopIPModeTrajectory Error: "<<pErrorStopTrajectory;
		}
		else
		{
			cout <<"Out of bounds! No further movement in this direction!";
			return 0;
		}	

	}				

	//move cylinder forward. Move at least by 1000 at a time, works well (smaller steps won't be recognized)
	bool MoveXForward(int ForwardMovement)
	{
		int PositionIs = PositionIs_Fct();
		int NewPosition = PositionIs+ForwardMovement;
		bool moved =SetPosition_ProfileMode(NewPosition);
		if(moved)
			return 1;
		else 
			return 0;
	
	}

	//move cylinder backward. Move at least by 1000 at a time, works well (smaller steps won't be recognized)
	bool MoveXBackward(int BackwardMovement)
	{
		int PositionIs = PositionIs_Fct();
		int NewPosition = PositionIs-BackwardMovement;
		bool moved = SetPosition_ProfileMode(NewPosition);
		if(moved)
			return 1;
		else 
			return 0;
	}

	//go to minimal extension position, must be done inside movement range (between sensors)
	void GoToMin()
	{
		SetPosition_ProfileMode(MainPositions.Min);
		Wait();
		PrintPosition();
		cout << "Minimal extension position reached! No further movements in this direction!!" << endl;
	}

	//go to maximal extension position, must be done inside movement range (between sensors)
	void GoToMax()
	{
		SetPosition_ProfileMode(MainPositions.Max);
		Wait();
		PrintPosition();
		cout << "Maximal extension position reached! No further movements in this direction!!" << endl;
	}

	//go to middle extension position
	void GoToMid()
	{
		SetPosition_ProfileMode(MainPositions.Mid);
		Wait();
		PrintPosition();
		cout << "Middle extension position reached." << endl;
	}





//MOVEMENT MODES=================================================================================================================
	
	//position mode
	void PositionMode()
	{
		unsigned int pErrorActPosMode;
		auto actPosMode = VCS_ActivatePositionMode(KeyHandle, 1, &pErrorActPosMode);
		cout << actPosMode << " " << pErrorActPosMode << endl;

		bool LimitReached = (GetLimitReached());
		long double position = 0;
		long double t(0);
		int dt (100);
		while (t < 100000 && !LimitReached && position <= MainPositions.Max && position >=MainPositions.Min)
		{
			LimitReached = (GetLimitReached());
			
			cout << "t = " << t << " ms" << endl;
		
			position = 5000 * pow(sin(t/800*3.14), 2);
			cout << position << endl;
			
			SetPosition(position);
		
			PrintPosition();	
		
			this_thread::sleep_for(chrono::milliseconds(dt));		
			t += dt;
			
		}
	}

	//profile position mode: start from Max, move backwards (contraction) and then forwards (relaxation) until time has run out
	void ProfilePositionMode()
	{
	//	ActivateProfileMode();	
			unsigned int Hub;
		cout<<"Insert stroke distance (1'000 to 60'000): "<<endl;
		cin>>Hub;	
		GoToMax();
		MoveXBackward(1000);

		int go=true;
		int dt=100;
		int t=0;
		bool movedF=1;
		bool movedB=1;
		cout<<"Starting Movement (Profile Position Mode)"<<endl;
		while (go&&t<=1000&&movedB&&movedF)
		{
			movedB = MoveXBackward(Hub);
			movedF = MoveXForward(Hub);
		//	this_thread::sleep_for(chrono::milliseconds(dt));
			t+=dt;
			cout<<"time: "<<t<<endl;
		}
		
		GoToMid();	
	}

	void ProfilePositionMode2()
	{
		int distance=25000;
		int dt=1;
		int tTotal=1000;
		int t=dt;
		int StartPosition=PositionIs_Fct();
		int position;
		int dx;
		bool moved=true;
		
		while (t<=tTotal&&moved)
		{
			dx=distance*sin(2*M_PI*t/tTotal);
			position=StartPosition+dx;
			moved=SetPosition_ProfileMode(position);
			this_thread::sleep_for(chrono::milliseconds(dt));
			cout<<"time: "<<t<<" position: "<<position<<" dx "<<dx<<endl;
			t+=dt;
		}
	
	}
	
	//Interpolated position mode
	bool InterpolatedPositionMode(int Amplitude, int Periode, int dt, int RunTime, int Resolution)
	{
		int PointNbr=1;
		int time=dt;
		unsigned int pErrorAddPvt, pErrorStartTrajectory;
		
		//start with point 0
		auto addPvt = VCS_AddPvtValueToIpmBuffer(KeyHandle, 1, 0, 0, dt, &pErrorAddPvt);		
		if(!addPvt)
			cout<<"Add PVT Error: "<<pErrorAddPvt<<endl;
		else 
			cout<<"PointNumber: "<<0<<" P: "<<0<<" T: "<<200<<" V: "<<0<<endl;

		
		while (time<=RunTime)
		{
			auto PTV=GetPTV(Amplitude,PointNbr,Periode,dt,Resolution);
			if (PTV.P<=MainPositions.Max&&PTV.P>=MainPositions.Min)
			{		
				auto addPvt = VCS_AddPvtValueToIpmBuffer(KeyHandle, 1, PTV.P, PTV.V, PTV.T, &pErrorAddPvt);
				if(!addPvt)
					cout<<"Add PVT Error: "<<pErrorAddPvt<<endl;
			}
			else
			{
				cout<<"Out of Bounds! No further Movement in this direction!"<<endl;
				break;
			}
			PointNbr+=1;
			time+=dt;
		}
		
		//end point with last position value
		auto PTV=GetPTV(Amplitude,PointNbr,Periode,dt,Resolution);
		if (PTV.P<=MainPositions.Max&&PTV.P>=MainPositions.Min)
		{		
			auto addPvt = VCS_AddPvtValueToIpmBuffer(KeyHandle, 1, PTV.P, 0, 0, &pErrorAddPvt);
			if(!addPvt)
				cout<<"Add PVT Error: "<<pErrorAddPvt<<endl;
		}
		else 
		{
			cout<<"Out of Bounds! No further Movement in this direction!"<<endl;
			return 0;
		}


		auto StartIpmTraj = VCS_StartIpmTrajectory(KeyHandle, 1, &pErrorStartTrajectory);
		if (!StartIpmTraj)
			cout<<"StartIPModeTrajectory Error: "<<pErrorStartTrajectory<<endl;
		else 
			cout<<"Starting Trajectory"<<endl;
	}


	// Interpolated position mode
	void InterpolatedPositionModeOld()
	{
		unsigned int pErrorIntPosMode;
		auto actIntPos = VCS_ActivateInterpolatedPositionMode(KeyHandle, 1, &pErrorIntPosMode);
		cout << actIntPos << " " << pErrorIntPosMode << endl;

		// auto setIpmBuffer = VCS_SetIpmBufferParameter(KeyHandle, 1, -100, 100, &pErrorIntPosMode);
		auto clearIpmBuffer = VCS_ClearIpmBuffer(KeyHandle, 1, &pErrorIntPosMode);

		// Add pvt value to buffer
		bool LimitReached = GetLimitReached();
		long double position = 0;
		long double t(0);
		int dt (500);
		unsigned int pErrorAddPvt;
		while (t <= 15000 && !LimitReached && position <= MainPositions.Max && position >= MainPositions.Min)
		{
			LimitReached = GetLimitReached();

			cout << "t = " << t << "   ";
			position = 5000 * sin(t/800*3.14);
			double velocity = 10000 * 3.14 / 800 * cos(t/800*3.14);
			auto addPvt = VCS_AddPvtValueToIpmBuffer(KeyHandle, 1, position, velocity, t, &pErrorAddPvt);
			cout << addPvt << " " << pErrorAddPvt <<" Position:"<<position  << endl;	
			t +=dt;	
		}
		
		auto startIpmTraj = VCS_StartIpmTrajectory(KeyHandle, 1, &pErrorAddPvt);
		cout << startIpmTraj << " " << pErrorAddPvt << endl; 
	}
	
	
protected:
	
	void* KeyHandle;	

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


};
