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
            cout<<"Activate IP Mode Error: "<<pErrorActivate<<endl;
        
        // auto setIpmBuffer = VCS_SetIpmBufferParameter(KeyHandle, 1, -100, 100, &pErrorIntPosMode);
        auto ClearIpmBuffer = VCS_ClearIpmBuffer(KeyHandle, 1, &pErrorClearBuffer);
        if (!ClearIpmBuffer)
            cout<<"Clear Buffer Error: "<<pErrorClearBuffer<<endl;
        
    }
    
    //move cylinder backward. Move at least by 1000 at a time, works well (smaller steps won't be recognized)
    void run(const int& distance)
    {
        runIPM(-20000, 1000, 250, 10000, 500);
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
    
    
protected:
    
    //move cylinder to x using InterpolatedPositionMode (ActivateInterpolatedPositionMode must be called first)
    bool SetPosition_InterpolatedPositionMode(int position)
    {
        // Move to position
        unsigned int pErrorMoveToPos;
        bool absoluteMovement = true;
        bool immediately = false;
        bool MoveToPos;
        
        if (position >= MainPositions.Min && position <= MainPositions.Max)
        {
            MoveToPos = VCS_MoveToPosition(KeyHandle, 1, position, absoluteMovement, immediately, &pErrorMoveToPos);
            cout <<"MoveToPos: "<< MoveToPos << " ErrorCode:  "  << pErrorMoveToPos << endl;
            Wait();
            printPosition();
            return 1;
        }
        else
        {    cout <<"Out of bounds! No further movement in this direction!";
            return 0;
        }
    }
    
    //Interpolated position mode
    bool runIPM(int Amplitude, int Periode, int dt, int RunTime, int Resolution)
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
    
};


// }

REGISTER(MotionMode, InterpolatedPositionMode);

#endif /* InterpolatedPositionMode_hpp */

