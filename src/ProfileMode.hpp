//
//  ProfileMode.hpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#ifndef ProfileMode_hpp
#define ProfileMode_hpp

#include <iostream>
#include <fstream>
#include <assert.h>



// namespace HADController {

class ProfileMode : public MotionMode {
public:
    
    ProfileMode(){}
    
    virtual ~ProfileMode(){}
    
    //activate Profile Position Mode
    void activateMode()
    {
        unsigned int pErrorProfilePosMode;
        auto profilePosMode = VCS_ActivateProfilePositionMode(KeyHandle, 1, &pErrorProfilePosMode);
        std::cout << profilePosMode << " " << pErrorProfilePosMode << std::endl;
        
        // Set sensor parameter
//        InputFileParser inputFile("data");
//        std::cout << "Sensor input a: " << inputFile["a"] << std::endl;
        
        unsigned int pErrorSetPosProfile;
        auto posProfile = VCS_SetPositionProfile(KeyHandle, 1, 22000, 15000, 15000, &pErrorSetPosProfile); // Node ID, position profile velocity,..acceleration,..deceleration
        std::cout << posProfile << " " << pErrorSetPosProfile << std::endl;
    }

    //move cylinder backward. Move at least by 1000 at a time, works well (smaller steps won't be recognized)
    void run()
    {
        if ( m_clp.map().count("-pm") > 0 )
        auto distance = m_args["-d"] mm;
        int PositionIs = PositionIs_Fct();
        int NewPosition = PositionIs - distance;
        bool moved = SetPosition_ProfileMode(NewPosition);
        std::cout << "From " << PositionIs << " to " << NewPosition << " by " << distance << " mm" << std::endl;
//        if(moved)
//            return 1;
//        else
//            return 0;
    }
    
    
protected:
    

    
    //move cylinder to x using ProfileMode (ActivateProfileMode must be called first)
    bool SetPosition_ProfileMode(int position)
    {
        // Move to position
        unsigned int pErrorMoveToPos;
        bool absoluteMovement = true;
        bool immediately = false;
        bool MoveToPos;
        
        if (position >= MainPositions.Min && position <= MainPositions.Max)
        {
            MoveToPos = VCS_MoveToPosition(KeyHandle, 1, position, absoluteMovement, immediately, &pErrorMoveToPos);
            std::cout <<"MoveToPos: "<< MoveToPos << " ErrorCode:  "  << pErrorMoveToPos << std::endl;
            Wait();
            printPosition();
            return 1;
        }
        else
        {    std::cout <<"Out of bounds! No further movement in this direction!";
            return 0;
        }
    }
    
};


// }

REGISTER(MotionMode, ProfileMode);

#endif /* ProfileMode_hpp */

