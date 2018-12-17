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


namespace HeartrateControllerSpace {

class InterpolatedPositionMode : public MotionMode {
public:
    
    InterpolatedPositionMode(){}
    
    virtual ~InterpolatedPositionMode(){}
    
    //activate interpolated position mode
    void activateMode()
    {
//        IpmStatus();
//        IpmBufferParameter();
        unsigned int pErrorActivate;
        unsigned int pErrorClearBuffer;
        
        auto IPMode = VCS_ActivateInterpolatedPositionMode(KeyHandle, 1, &pErrorActivate);
        // if (!IPMode)
            //std::cout<<"Activate IP Mode Error: "<<pErrorActivate<<std::endl;
        
        // auto setIpmBuffer = VCS_SetIpmBufferParameter(KeyHandle, 1, -100, 100, &pErrorIntPosMode);
        auto ClearIpmBuffer = VCS_ClearIpmBuffer(KeyHandle, 1, &pErrorClearBuffer);
        // if (!ClearIpmBuffer)
            //std::cout<<"Clear Buffer Error: "<<pErrorClearBuffer<<std::endl;
        
    }
    
    //move cylinder backward. Move at least by 1000 at a time, works well (smaller steps won't be recognized)
    void run(const int& offset = 0)
    {
        auto function = readArgument("-if", 1);
        auto async = readArgument("-ias", 0.3);

        auto distance = readArgument("-ia", -20.0) mm;
        auto period = readArgument("-ip", 1000.0);
        auto timestep = readArgument("-it", period/40.);
        auto runtime = readArgument("-irt", period);
        auto resolution = readArgument("-ir", 500);
        auto timeout = readArgument("-ito", (period > 2000 ? period - 1000 : period - 700));

        runIPM(function, distance, period, async, timestep, runtime, resolution, timeout, offset);
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
    
    std::vector<PTV> m_ptvVec;
    
    //Interpolated position mode
    bool runIPM(int function, double Amplitude, double Periode, double async, double dt, double runTime, double Resolution, double& timeout, const int& offset = 0)
    {
        unsigned int pErrorAddPvt, pErrorStartTrajectory;

        int PointNbr=1;
        double time (0);
        
        if (m_ptvVec.size() < 1)
        {
            for (unsigned int i(0); i <= runTime/dt; ++i)
            {
                PTV ptv = motionTypeFunction(Amplitude,i,Periode, async,dt,Resolution, offset, function);
                m_ptvVec.push_back(ptv);
                //std::cout << time << " - " << i << "-th point added " << std::endl;
                time = i*dt;
            }
            
            m_ptvVec[40].T = 0;
        }

        bool addPvt;
        for (auto& ptv : m_ptvVec)
        {
            addPvt = VCS_AddPvtValueToIpmBuffer(KeyHandle, 1, ptv.P, ptv.V, ptv.T, &pErrorAddPvt);
        }
        if(!addPvt) std::cout << "add to ipm-buffer error: " << pErrorAddPvt << std::endl;

        auto StartIpmTraj = VCS_StartIpmTrajectory(KeyHandle, 1, &pErrorStartTrajectory);
        if (!StartIpmTraj) std::cout << "StartIPModeTrajectory Error: " << pErrorStartTrajectory << std::endl;
        
        
        unsigned int Timeout = timeout;
        unsigned int pErrorCode;
        //sleep(1.2);
        auto WaitForTarget= VCS_WaitForTargetReached(KeyHandle, 1, Timeout+5000, &pErrorCode);
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
    
    PTV motionTypeFunction (double Amplitude,int i,double Periode, double async, double dt, double Resolution, const int& offset, const int function)
    {
        PTV ptv;
        switch (function)
        {
            case 0:
                ptv = GetPTVsin(Amplitude,i,Periode,dt,Resolution, offset);
                break;
            case 1:
                ptv = GetPTVsin2(Amplitude,i,Periode,dt,Resolution, offset);
                break;
            case 2:
                ptv = GetPTVabsT(Amplitude,i,Periode,dt,Resolution, offset);
                break;
            case 3:
                ptv = GetPTVasyncAbsT(Amplitude,i,Periode, async, dt,Resolution, offset);
                break;
            default:
                break;
        }
        //std::cout << ptv.P << " : " << ptv.V << " : " << ptv.T << std::endl;
        return ptv;
    }
    
    //Get IPMode PTV sin(t)
    PTV GetPTVsin(double Amplitude,double PointNumber,double Periode, double dt, double Resolution, const int& offset)
    {
        Amplitude *= 0.5;
        int P= (int) -(Amplitude*sin(PointNumber*dt*2.0*M_PI/Periode-M_PI/2.0))-Amplitude + offset;
        int T= (int) dt;
        int V= (int) -(Amplitude*2.0*M_PI/Periode*cos(PointNumber*dt*2.0*M_PI/Periode-M_PI/2.0))*1000.0/(4.0*Resolution)*60.0;

        //std::cout<<"  \t[0] PointNumber: " << PointNumber << "  \tT: " << T << "  \tP: " << P << "  \tV: " << V<< std::endl;
        return{P,T,V};
    }
    
    //Get IPMode PTV sin^2(t)
    PTV GetPTVsin2(double Amplitude,double PointNumber,double Periode, double dt, double Resolution, const int& offset)
    {
        Periode *= 2.0;
        int t = (int) PointNumber * dt;
        int P = (int) - Amplitude * std::pow(std::sin(2.0 * M_PI * t / Periode), 2.0) + offset;
        int T = (int) dt;
        int V = (int) - 2.0 * M_PI * Amplitude * std::sin(4.0 * M_PI * t / Periode) / Periode * 60000.0 / (4.0 * Resolution);
        
        //std::cout<<"  \t[1]PointNumber: " << PointNumber << "  \tT: " << T << "  \tP: " << P << "  \tV: " << V<< std::endl;
        return{P,T,V};
    }
    
    //Get IPMode PTV |t|
    PTV GetPTVabsT(double Amplitude,double PointNumber,double Periode, double dt, double Resolution, const int& offset)
    {
        int t = (int) PointNumber * dt;
        int P= (int) - Amplitude * std::abs(t - 0.5*Periode) / (0.5*Periode) + Amplitude;
        int T= (int) dt;
        int V= (int) - Amplitude * std::abs(t - 0.5*Periode) / ((t - 0.5*Periode) * 0.5*Periode);
        
        std::cout<<"  \t[0] PointNumber: " << PointNumber << "  \tT: " << T << "  \tP: " << P << "  \tV: " << V<< std::endl;
        return{P,T,V};
    }
    
    //Get IPMode PTV asyn |t|
    PTV GetPTVasyncAbsT(double Amplitude,double PointNumber,double Periode, double async, double dt, double Resolution, const int& offset)
    {
        int t = (int) PointNumber * dt;
        int P= (int) - Amplitude * std::abs(t - 0.5*Periode) / (0.5*Periode) + Amplitude;
        int T= (int) dt;
        int V= (int) - Amplitude * std::abs(t - 0.5*Periode) / ((t - 0.5*Periode) * 0.5*Periode);
        
        std::cout<<"  \t[0] PointNumber: " << PointNumber << "  \tT: " << T << "  \tP: " << P << "  \tV: " << V<< std::endl;
        return{P,T,V};
    }

};

}

REGISTER(MotionMode, InterpolatedPositionMode);

#endif /* InterpolatedPositionMode_hpp */

