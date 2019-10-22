//
//  ProfilePositionMode.hpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#ifndef ProfilePositionMode_hpp
#define ProfilePositionMode_hpp

#include <iostream>
#include <fstream>
#include <assert.h>


namespace HeartrateControllerSpace {

class ProfilePositionMode : public MotionMode {
public:
    
    ProfilePositionMode(){}
    
    virtual ~ProfilePositionMode(){}
    
    //activate Profile Position Mode
    void activateMode()
    {
        m_acc_sys = 7000;
        m_acc_dia = 2500;
        
        unsigned int pErrorProfilePosMode;
        auto profilePosMode = VCS_ActivateProfilePositionMode(KeyHandle, 1, &pErrorProfilePosMode);
        if (!profilePosMode) std::cout << profilePosMode << " " << pErrorProfilePosMode << std::endl;
        
        unsigned int pErrorSetPosProfile;
        auto posProfile = VCS_SetPositionProfile(KeyHandle, 1, 2200, 15000, 15000, &pErrorSetPosProfile); // Node ID, position profile velocity,..acceleration,..deceleration
        if (!posProfile) std::cout << posProfile << " " << pErrorSetPosProfile << std::endl;
    }

    //move cylinder backward. Move at least by 1000 at a time, works well (smaller steps won't be recognized)
    void run(const int& offset = 0)
    {
        using namespace std::chrono;
        steady_clock::time_point t1 = steady_clock::now();
        
        int distance = readArgument("-ppa", 10) mm;
        int frequency = readArgument("-ppf", 30);
        
        unsigned int pErrorSetPosProfile1;
        auto posProfile1 = VCS_SetPositionProfile(KeyHandle, 1, 10000, m_acc_sys, m_acc_sys, &pErrorSetPosProfile1);
        
        SetPosition_ProfileMode(distance);
        // std::cout << "From " << PositionIs/(1600) << " to " << NewPosition/(1600) << " by " << -distance/(1600) << " mm" << std::endl;
        
        unsigned int pErrorSetPosProfile2;
        auto posProfile2 = VCS_SetPositionProfile(KeyHandle, 1, 2200, m_acc_dia, m_acc_dia, &pErrorSetPosProfile2);
        
        SetPosition_ProfileMode(-distance);
        
    
        steady_clock::time_point t2 = steady_clock::now();
        duration<double> time_span = 1000. * duration_cast<duration<double>>(t2 - t1);
        const double dt = time_span.count();
        const double bpm = 60000./dt;
        std::cout << bpm << " , ";
        
        int bpmDiff = frequency - int(bpm);
        std::cout << m_acc_sys << " , " << bpmDiff << " / ";
        m_acc_sys += 120 * bpmDiff;
        m_acc_dia += 85 * bpmDiff;

    }
    
    
protected:
    
    int m_acc_sys;
    int m_acc_dia;
    
    //move cylinder to x using ProfileMode (ActivateProfileMode must be called first)
    void SetPosition_ProfileMode(int position, bool absoluteMovement = false, bool immediately = false)
    {
        unsigned int pErrorMoveToPos;

        auto moveToPos = VCS_MoveToPosition(KeyHandle, 1, position, absoluteMovement, immediately, &pErrorMoveToPos);
        if (!moveToPos) std::cout <<"MoveToPos: "<< moveToPos << " ErrorCode:  "  << pErrorMoveToPos << std::endl;
        Wait();
    }
    
};


}

REGISTER(MotionMode, ProfilePositionMode);

#endif /* ProfilePositionMode_hpp */

