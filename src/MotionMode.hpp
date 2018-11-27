//
//  MotionMode.hpp
//  HADSoftware
//
//  Created by Thomas Kummer on 24.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#ifndef MotionMode_hpp
#define MotionMode_hpp

#include "Factory.hpp"


struct Positions
{
    int Min;
    int Max;
    int Mid;
};


class MotionMode {
public:
    
    
    MotionMode(){}
    
    virtual ~MotionMode(){}
    
    void setKeyHandle(void* keyHandle)
    {
        KeyHandle = keyHandle;
    }
    
    virtual void activateMode() = 0;
    
    virtual void run(const int& distance) = 0;
    
    
protected:
    
    void* KeyHandle;
    Positions MainPositions={-500000, 500000, 0};

    int PositionIs_Fct()
    {
        unsigned int PositionIsError;
        int PositionIs;
        auto GetPositionIs = VCS_GetPositionIs(KeyHandle, 1, &PositionIs, &PositionIsError);
        return PositionIs;
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
    
    //print position to console
    void printPosition()
    {
        unsigned int PositionIsError;
        int PositionIs;
        auto GetPositionIs = VCS_GetPositionIs(KeyHandle, 1, &PositionIs, &PositionIsError);
        cout << GetPositionIs << " " << PositionIsError <<" Position: " << PositionIs << endl;
    }

};

#endif /* MotionMode_hpp */
