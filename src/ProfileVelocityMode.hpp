//
//  ProfileVelocityMode.hpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#ifndef ProfileVelocityMode_hpp
#define ProfileVelocityMode_hpp

#include <iostream>
#include <fstream>
#include <assert.h>


namespace HeartrateControllerSpace {

class ProfileVelocityMode : public MotionMode {
public:
    
    ProfileVelocityMode(){}
    
    virtual ~ProfileVelocityMode(){}
    
    void activateMode()
    {
        unsigned int pErrorProfilePosMode;
        auto profilePosMode = VCS_ActivateProfileVelocityMode(KeyHandle, 1, &pErrorProfilePosMode);
        //std::cout << profilePosMode << " " << pErrorProfilePosMode << std::endl;
        
        // Set sensor parameter
//        InputFileParser inputFile("data");
//        std::cout << "Sensor input a: " << inputFile["a"] << std::endl;
        
        unsigned int pErrorSetPosProfile;
        auto posProfile = VCS_SetVelocityProfile(KeyHandle, 1, 100, 100, &pErrorSetPosProfile); // Node ID, position profile velocity,..acceleration,..deceleration
        //std::cout << posProfile << " " << pErrorSetPosProfile << std::endl;
    }

    //move cylinder backward. Move at least by 1000 at a time, works well (smaller steps won't be recognized)
    void run(const int& offset = 0)
    {
        auto speed = readArgument("-vs", 10) mm;
        int PositionIs = PositionIs_Fct();
        int NewPosition = PositionIs - speed;
        if (speed == 0) NewPosition *= 0;
        bool moved = SetPosition_ProfileVelocityMode(speed);
        //std::cout << "From " << PositionIs << " to " << NewPosition << " by " << distance/(1600) << " mm" << std::endl;
//        if(moved)
//            return 1;
//        else
//            return 0;
    }
    
    
protected:
    

    
    //move cylinder to x using ProfileVelocityMode (ActivateProfileVelocityMode must be called first)
    bool SetPosition_ProfileVelocityMode(int speed)
    {
        // Move to position
        unsigned int pErrorMoveToPos;
        bool absoluteMovement = true;
        bool immediately = false;
        bool MoveToPos;
        
//        if (position >= MainPositions.Min && position <= MainPositions.Max)
//        {
            MoveToPos = VCS_MoveWithVelocity(KeyHandle, 1, speed, &pErrorMoveToPos);
            //std::cout <<"MoveToPos: "<< MoveToPos << " ErrorCode:  "  << pErrorMoveToPos << std::endl;
            Wait();
            //printPosition();
            return 1;
//        }
//        else
//        {    std::cout <<"Out of bounds! No further movement in this direction!";
//            return 0;
//        }
    }
    
};


}

REGISTER(MotionMode, ProfileVelocityMode);

#endif /* ProfileVelocityMode_hpp */

