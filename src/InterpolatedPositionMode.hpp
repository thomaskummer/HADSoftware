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
#include <time.h>


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
        if (m_firstTimestep) {
            m_frequency = std::vector<double> (2, 5000);
            m_amplitude = 1.; //readArgument("-ia", -20.0);
            m_sysFrac= readArgument("-ias", 0.3);
            m_firstTimestep = false;
        }
        
        auto function = readArgument("-if", 1);
        auto async = readArgument("-ias", 0.35);

        auto distance = readArgument("-ia", -20.0) mm;
        auto period = readArgument("-ip", 1000.0);

        const double period_diff = period - m_frequency[0];
        if ( period_diff > 0 )
        {
            m_frequency[0] += std::min(period_diff , 800.);
        }
        else
        {
            m_frequency[0] += std::max(period_diff , -800.);
        }
//        timeout = m_frequency[0] -700;
        
        auto timestep = readArgument("-it", m_frequency[0]/40.);
        auto runtime = readArgument("-irt", m_frequency[0]);
        auto resolution = readArgument("-ir", 480);
        auto timeout = readArgument("-ito", m_frequency[0]); //(period > 2000 ? period - 1000 : period - 700));
//        timeout -= 100.;
        
        using namespace std::chrono;
        steady_clock::time_point t1 = steady_clock::now();

        // Smooth out frequency
//        for (int i(m_frequency.size() - 1); i >= 0; --i) m_frequency[i] = m_frequency[i-1];
//        m_frequency[0] = period;
//        double avg_frequency (0.);
//        for (int i(0); i < m_frequency.size(); ++i) avg_frequency += m_frequency[i] / m_frequency.size();
//        m_frequency[0] = avg_frequency;

        
        runIPM(function, distance, m_frequency[0], async, timestep, runtime, resolution, timeout, offset);
        
        steady_clock::time_point t2 = steady_clock::now();
        duration<double> time_span = 1000. * duration_cast<duration<double>>(t2 - t1);
        const double dt = time_span.count();
        std::cout << "It took " << dt << " ms - instead of " << period << std::endl;
        
//        m_frequency[0] += 0.3 * (period - dt);
//        gtp["-ip"] += 0.1 * ( periodDesired - dt );
        
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
    
    std::vector<double> m_frequency;
    double m_amplitude;
    double m_sysFrac;
    bool m_firstTimestep = true;
    
    //Interpolated position mode
    bool runIPM(int function, double Amplitude, double Periode, double async, double dt, double runTime, double Resolution, double& timeout, const int& offset = 0)
    {
        unsigned int pErrorAddPvt, pErrorStartTrajectory;

        int PointNbr=1;
        double time (0);
        
        unsigned int pErrorClearBuffer;
        auto ClearIpmBuffer = VCS_ClearIpmBuffer(KeyHandle, 1, &pErrorClearBuffer);
        
        m_ptvVec.clear();
//        if (m_ptvVec.size() < 1)
//        {
            for (unsigned int i(0); i <= 40; ++i)
            {
                PTV ptv = motionTypeFunction(Amplitude, i, Periode, async, dt, Resolution, offset, function);
                m_ptvVec.push_back(ptv);
                //std::cout << time << " - " << i << "-th point added " << std::endl;
                time = i*dt;
            }
            
            m_ptvVec[40].T = 0;
//        }

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
    
    PTV motionTypeFunction (double Amplitude,int i,double Periode, double async, double dt, double Resolution, const int& offset, const int function)
    {
        PTV ptv;
        ptv = GetPTVsin2asymetric(Amplitude,i,Periode,async,dt,Resolution, offset);
        
//        switch (function)
//        {
//            case 0:
//                ptv = GetPTVsin2asymetric(Amplitude,i,Periode,0.2,dt,Resolution, offset);
//                break;
//            case 1:
//                ptv = GetPTVsin2asymetric(Amplitude,i,Periode,0.3,dt,Resolution, offset);
//                break;
//            case 2:
//                ptv = GetPTVsin2asymetric(Amplitude,i,Periode,0.4,dt,Resolution, offset);
//                break;
//            case 3:
//                ptv = GetPTVsin2asymetric(Amplitude,i,Periode,0.5,dt,Resolution, offset);
//                break;
//            default:
//                break;
//        }
        //std::cout << ptv.P << " : " << ptv.V << " : " << ptv.T << std::endl;
        return ptv;
    }
    
    PTV GetPTVsin2asymetric(double Amplitude,double PointNumber,double Periode, double sysDiasFrac, double dt, double Resolution, const int& offset)
    {
        int t, P, T, V;
        t = (int) PointNumber * dt;
        
        if (t <= Periode * sysDiasFrac )
        {
            Periode *= 2.0;
            Periode *= sysDiasFrac * 2.;
            
            P = (int) - Amplitude * std::pow(std::sin(2.0 * M_PI * t / Periode ), 2.0) + offset;
            T = (int) dt;
            V = (int) - 2.0 * M_PI * Amplitude * std::sin(4.0 * M_PI * t / Periode) / Periode * 60000.0 / (4.0 * Resolution);
            // std::cout << t << " a " << P << " " << V << " " << Periode << std::endl;
            
        }
        
        if (t > Periode * sysDiasFrac )
        {
            Periode *= 2.0;
            t += Periode * (0.5 - sysDiasFrac);// * (1/2);
            Periode *= (1 - sysDiasFrac) * 2.;
            
            P = (int) - Amplitude * std::pow(std::sin(2.0 * M_PI * t / Periode), 2.0) + offset;
            T = (int) dt;
            V = (int) - 2.0 * M_PI * Amplitude * std::sin(4.0 * M_PI * t / Periode) / Periode * 60000.0 / (4.0 * Resolution);
            // std::cout << t << " b " << P << " " << V << " " << Periode << std::endl;
            
        }
        
        return {P,T,V};
    }
    
};

}

REGISTER(MotionMode, InterpolatedPositionMode);

#endif /* InterpolatedPositionMode_hpp */

