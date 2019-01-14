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

namespace HeartrateControllerSpace {

#define mm * -1600

struct Positions
{
    int Min;
    int Max;
    int Mid;
};

struct PTV
{
    int P, T, V;
};


class MotionMode {
public:
    
    
    MotionMode(){}
    
    virtual ~MotionMode(){}
    
    void setKeyHandle(void* keyHandle)
    {
        KeyHandle = keyHandle;
    }
    
    virtual void setArguments(const std::map<std::string, double>& argMap)
    {
        m_args = argMap;
    }
    
    const double& readArgument(const std::string& arg, const double& defaultArg) const
    {
        if ( m_args.count(arg) > 0 ) return m_args.find(arg)->second;
        else return defaultArg;
    }

    virtual void activateMode() = 0;
    
    virtual void run(const int& offset = 0) = 0;
    
    
protected:
    
    void* KeyHandle;
    Positions MainPositions={-500000, 500000, 0};
    std::map<std::string, double> m_args;

    
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
            std::cout << "Error in Wait Function! Error Code: " << pErrorCode << std::endl;
    }
    
//    //print position to console
//    void printPosition()
//    {
//        unsigned int PositionIsError;
//        int PositionIs;
//        auto GetPositionIs = VCS_GetPositionIs(KeyHandle, 1, &PositionIs, &PositionIsError);
//        std::cout << GetPositionIs << " " << PositionIsError <<" Position: " << PositionIs << std::endl;
//    }

};
    
}

#endif /* MotionMode_hpp */
