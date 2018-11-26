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
    
    virtual bool move(const int& distance) = 0;
    
    
protected:
    
    void* KeyHandle;
    Positions MainPositions={-500000, 500000, 0};

    
};

#endif /* MotionMode_hpp */
