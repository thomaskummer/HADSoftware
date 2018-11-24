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


#define mm * -1600

// namespace HADController {

class ProfileMode {
public:
    
    ProfileMode(){}
    
    virtual ~ProfileMode(){}
    
    
    //activate Profile Position Mode
    void activateProfileMode()
    {
        unsigned int pErrorProfilePosMode;
        auto profilePosMode = VCS_ActivateProfilePositionMode(KeyHandle, 1, &pErrorProfilePosMode);
        cout << profilePosMode << " " << pErrorProfilePosMode << endl;
        
        // Set sensor parameter
        InputFileParser inputFile("data");
        std::cout << "Sensor input a: " << inputFile["a"] << std::endl;
        
        unsigned int pErrorSetPosProfile;
        auto posProfile = VCS_SetPositionProfile(KeyHandle, 1, 22000, 15000, 15000, &pErrorSetPosProfile); // Node ID, position profile velocity,..acceleration,..deceleration
        std::cout << posProfile << " " << pErrorSetPosProfile << std::endl;
    }
    
    //move cylinder backward. Move at least by 1000 at a time, works well (smaller steps won't be recognized)
    bool move(const int& distance)
    {
        int PositionIs = PositionIs_Fct();
        int NewPosition = PositionIs - distance;
        bool moved = SetPosition_ProfileMode(NewPosition);
        std::cout << "From " << PositionIs << " to " << NewPosition << " by " << distance << " mm" << std::endl;
        if(moved)
            return 1;
        else
            return 0;
    }
    
    
protected:
    
    void* KeyHandle;
    Positions MainPositions={0, 205000, 137000};
    
    
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
    
    //wait until movement is finished; does not work with position mode!
    void Wait()
    {
        unsigned int Timeout = 3000; //max waiting time in ms
        unsigned int pErrorCode;
        
        auto WaitForTarget= VCS_WaitForTargetReached(KeyHandle, 1, Timeout, &pErrorCode);
        if (!WaitForTarget)
            cout<<"Error in Wait Function! Error Code: "<<pErrorCode<<endl;
    }
    
};


// }


#endif /* ProfileMode_hpp */

