//
//  InterpolatedPositionMode.hpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#ifndef InterpolatedPositionMode_hpp
#define InterpolatedPositionMode_hpp

#include <iostream>
#include <fstream>
#include <assert.h>



// namespace HADController {

class InterpolatedPositionMode : public MotionMode {
public:
    
    InterpolatedPositionMode(){}
    
    virtual ~InterpolatedPositionMode(){}
    
    //activate interpolated position mode
    void activateMode()
    {
        unsigned int pErrorActivate;
        unsigned int pErrorClearBuffer;
        
        auto IPMode = VCS_ActivateInterpolatedPositionMode(KeyHandle, 1, &pErrorActivate);
        if (!IPMode)
            std::cout<<"Activate IP Mode Error: "<<pErrorActivate<<std::endl;
        
        // auto setIpmBuffer = VCS_SetIpmBufferParameter(KeyHandle, 1, -100, 100, &pErrorIntPosMode);
        auto ClearIpmBuffer = VCS_ClearIpmBuffer(KeyHandle, 1, &pErrorClearBuffer);
        if (!ClearIpmBuffer)
            std::cout<<"Clear Buffer Error: "<<pErrorClearBuffer<<std::endl;
        
    }
    
    //move cylinder backward. Move at least by 1000 at a time, works well (smaller steps won't be recognized)
    void run()
    {
        auto function = readArgument("-if", 0);

        auto distance = readArgument("-ia", -20.0) mm;
        auto period = readArgument("-ip", 1000.0);
        auto timestep = readArgument("-it", period/58.);
        auto runtime = readArgument("-irt", period);
        auto resolution = readArgument("-ir", 500);
        auto timeout = readArgument("ito", (period > 2000 ? period - 1000 : period - 700));

        runIPM(function, distance, period, timestep, runtime, resolution, timeout);
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
        
        std::cout<<GetStatus<<": Status "<<pTrajectoryRunning<<": TrajectoryRunning "<<pIsUnderflowWarning<<": UFlowWarnig "<<pIsOverflowWarning<<": OFlowWarning "
        <<pIsVelocityWarning<<": VelocityWarning "<<pIsAccelerationWarning<<": AccelerationWarning "<<pIsUnderflowError<<": UFlowError "<<pIsOverflowError<<": OFlowError "
        <<pIsVelocityError<<": VelocityError "<<pIsAccelerationError<<": AccelerationError "<<pErrorCode<<": ErrorCode"<<std::endl;
    }
    
    
protected:
    
//    //move cylinder to x using InterpolatedPositionMode (ActivateInterpolatedPositionMode must be called first)
//    bool SetPosition_InterpolatedPositionMode(int position)
//    {
//        // Move to position
//        unsigned int pErrorMoveToPos;
//        bool absoluteMovement = true;
//        bool immediately = false;
//        bool MoveToPos;
//
//        if (position >= MainPositions.Min && position <= MainPositions.Max)
//        {
//            MoveToPos = VCS_MoveToPosition(KeyHandle, 1, position, absoluteMovement, immediately, &pErrorMoveToPos);
//            std::cout <<"MoveToPos: "<< MoveToPos << " ErrorCode:  "  << pErrorMoveToPos << std::endl;
//            Wait();
//            printPosition();
//            return 1;
//        }
//        else
//        {    std::cout <<"Out of bounds! No further movement in this direction!";
//            return 0;
//        }
//    }
    
    //Interpolated position mode
    bool runIPM(int function, double Amplitude, double Periode, double dt, double RunTime, double Resolution, double& timeout)
    {
        int PointNbr=1;
        double time=dt;
        unsigned int pErrorAddPvt, pErrorStartTrajectory;
        
        //start with point 0
        auto addPvt = VCS_AddPvtValueToIpmBuffer(KeyHandle, 1, 0, 0, dt, &pErrorAddPvt);
        if(!addPvt)
            std::cout<<"Add PVT-0 Error: "<<pErrorAddPvt<<std::endl;
        else
            // std::cout<<"PointNumber: "<<0<<" P: "<<0<<" T: "<<200<<" V: "<<0<<std::endl;

        
        while (time<=RunTime)
        {
            PTV ptv;
            switch (function) {
                case 0:
                    ptv = GetPTVsin(Amplitude,PointNbr,Periode,dt,Resolution);
                    break;
                case 1:
                    ptv = GetPTVsin2(Amplitude,PointNbr,Periode,dt,Resolution);
                default:
                    break;
            }

            if (ptv.P<=MainPositions.Max&&ptv.P>=MainPositions.Min)
            {
                auto addPvt = VCS_AddPvtValueToIpmBuffer(KeyHandle, 1, ptv.P, ptv.V, ptv.T, &pErrorAddPvt);
                if(!addPvt)
                    std::cout<<"Add PVT-while Error: "<<pErrorAddPvt<<std::endl;
            }
            else
            {
                std::cout<<"Out of Bounds! No further Movement in this direction!"<<std::endl;
                break;
            }
            PointNbr+=1;
            time+=dt;
        }
        
        //end point with last position value
        PTV ptv;
        switch (function) {
            case 0:
                ptv = GetPTVsin(Amplitude,PointNbr,Periode,dt,Resolution);
                break;
            case 1:
                ptv = GetPTVsin2(Amplitude,PointNbr,Periode,dt,Resolution);
            default:
                break;
        }

        if (ptv.P<=MainPositions.Max&&ptv.P>=MainPositions.Min)
        {
            auto addPvt = VCS_AddPvtValueToIpmBuffer(KeyHandle, 1, ptv.P, 0, 0, &pErrorAddPvt);
            if(!addPvt)
                std::cout<<"Add PVT-endl Error: "<<pErrorAddPvt<<std::endl;
        }
        else
        {
            std::cout<<"Out of Bounds! No further Movement in this direction!"<<std::endl;
            return 0;
        }
        
        std::cout << "Trajectory has been computed" << std::endl;
        
        auto StartIpmTraj = VCS_StartIpmTrajectory(KeyHandle, 1, &pErrorStartTrajectory);

        if (!StartIpmTraj)
            std::cout<<"StartIPModeTrajectory Error: "<<pErrorStartTrajectory<<std::endl;
        else
            std::cout<<"Starting Trajectory..."<<std::endl;
        
        
        unsigned int Timeout = timeout; //max waiting time in ms
        unsigned int pErrorCode;

        auto WaitForTarget= VCS_WaitForTargetReached(KeyHandle, 1, Timeout, &pErrorCode);
    }
    
    //Get buffer parameters for ipm
    void IpmBufferParameter()
    {
        unsigned short pUnderflowWarningLimit;
        unsigned short pOverflowWarningLimit;
        unsigned int pMaxBufferSize;
        unsigned int pErrorCode;
        
        auto GetParameter = VCS_GetIpmBufferParameter(KeyHandle, 1, &pUnderflowWarningLimit, &pOverflowWarningLimit, &pMaxBufferSize, &pErrorCode);
        
        std::cout<<GetParameter<<": GetParameter "<<pUnderflowWarningLimit<<": UFlowWarninglimit "<<pOverflowWarningLimit<<": OFlowWarningLimit "
        <<pMaxBufferSize<<": MaxBufferSize "<<pErrorCode<<": ErrorCode"<<std::endl;
    }
    
    //Get IPMode PTV sin(t)
    PTV GetPTVsin(double Amplitude,double PointNumber,double Periode, double dt, double Resolution)
    {
        Amplitude *= 0.5;
        int P= (int) -(Amplitude*sin(PointNumber*dt*2.0*M_PI/Periode-M_PI/2.0))-Amplitude;
        int T= (int) dt;
        int V= (int) -(Amplitude*2.0*M_PI/Periode*cos(PointNumber*dt*2.0*M_PI/Periode-M_PI/2.0))*1000.0/(4.0*Resolution)*60.0;

        std::cout<<"  \t[0] PointNumber: " << PointNumber << "  \tT: " << T << "  \tP: " << P << "  \tV: " << V<< std::endl;
        return{P,T,V};
    }
    
    //Get IPMode PTV sin^2(t)
    PTV GetPTVsin2(double Amplitude,double PointNumber,double Periode, double dt, double Resolution)
    {
        Periode *= 2.0;
        int t = (int) PointNumber * dt;
        int P = (int) - Amplitude * std::pow(std::sin(2.0 * M_PI * t / Periode), 2.0);
        int T = (int) dt;
        int V = (int) - 2.0 * M_PI * Amplitude * std::sin(4.0 * M_PI * t / Periode) / Periode * 60000.0 / (4.0 * Resolution);
        
        std::cout<<"  \t[1]PointNumber: " << PointNumber << "  \tT: " << T << "  \tP: " << P << "  \tV: " << V<< std::endl;
        return{P,T,V};
    }
    
};


// }

REGISTER(MotionMode, InterpolatedPositionMode);

#endif /* InterpolatedPositionMode_hpp */

