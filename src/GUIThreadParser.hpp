//
//  GUIThreadParser.hpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#ifndef GUIThreadParser_hpp
#define GUIThreadParser_hpp

#include <iostream>
#include <fstream>
#include <assert.h>
#include "InputParser.hpp"

namespace HeartrateControllerSpace {

class GUIThreadParser : public InputParser {
public:
    
    GUIThreadParser() : m_interface (std::vector<int> (10, 0)), m_waitingForInput (true), m_taskSubmitted (false)
    {
        std::cout << "GUIThreadParser created" << std::endl;
    }
    
    virtual ~GUIThreadParser(){}
    
    void operator()()
    {
        std::cout << "gtp operator()" << std::endl;
    }
    
    void readGUIActions()
    {
        std::cout << "\nThe first word to type in is the action to perform \n or a parameter to change (for help type h or help).\n";
        std::string line;

        while (m_waitingForInput)
        {
            std::cout << ">>> ";
            std::getline(std::cin, line);
            std::istringstream iss(line);
            std::string task, value;
            iss >> task >> value;

            
            if (!task.compare("move"))
            {
                m_interface[0] = 1;
                m_interface[1] = std::stoi(value);
                
                m_tasks["-pd"] = std::stoi(value);
                m_taskSubmitted = true;
            }
            
            if (!task.compare("ipm"))
            {
                m_interface[2] = 1;
                m_interface[3] = std::stoi(value);
                
                m_tasks["-ia"] = std::stoi(value);
                m_taskSubmitted = true;
            }

            if(!task.compare("exit"))
            {
                m_waitingForInput = false;
                
            }
        }
    }
    
    const std::vector<int>& interface()
    {
        return m_interface;
    }
    
    const int& interface(const int& i)
    {
        return m_interface[i];
    }
    
    const bool& waitingForInput()
    {
        return m_waitingForInput;
    }

    const bool& taskSubmitted()
    {
        return m_taskSubmitted;
    }

    void taskReceived()
    {
        m_taskSubmitted = false;
    }
    
protected:
    
    bool m_waitingForInput;
    bool m_taskSubmitted;
    std::vector<int> m_interface;
    
};
    

}


#endif /* GUIThreadParser_hpp */
