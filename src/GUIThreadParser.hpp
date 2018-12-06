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

# define interfaceSize 10
    
class GUIThreadParser : public InputParser {
public:
    
    GUIThreadParser() :
        m_interface (std::vector<int> (interfaceSize, 0)),
        m_waitingForInput (true),
        m_taskSubmitted (false),
        m_keepRunning (false)
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
        std::cout << "\nType help to see available commands.\n";
        std::string line;

        while (m_waitingForInput)
        {
            std::cout << ">>> ";
            std::getline(std::cin, line);
            std::istringstream iss(line);
            std::string task, value1, value2, value3;
            iss >> task; // >> value1 >> value2 >> value3;
            
            if (!task.compare("move"))
            {
                iss >> value1;
                m_interface[0] = 1;
                m_interface[1] = std::stoi(value1);
                
                m_tasks["-pd"] = std::stoi(value1);
                m_taskSubmitted = true;
            }
            
            if (!task.compare("ipm"))
            {
                iss >> value1; iss >> value2; iss >> value3;
                m_interface[2] = 1;
                m_interface[3] = (value1.empty() ? -10 : std::stoi(value1));
                
                m_tasks["-ia"] = (value1.empty() ? -10 : std::stoi(value1));
                m_tasks["-ip"] = (value2.empty() ? 1000 : std::stoi(value2));
                m_tasks["-if"] = (value3.empty() ? 1 : std::stoi(value3));
                m_keepRunning = true;
                m_taskSubmitted = true;
            }

            if(!task.compare("help"))
            {
                m_interface[4] = 1;
                m_taskSubmitted = true;
                sleep(1);
            }

            if(!task.compare("reset"))
            {
                m_interface[6] = 1;
                m_taskSubmitted = true;
            }

            if(!task.compare("stop"))
            {
                m_keepRunning = false;
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
    
    int& interface(const int& i)
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
    
    const bool& keepRunning()
    {
        return m_keepRunning;
    }
    
    
protected:
    
    bool m_waitingForInput;
    bool m_taskSubmitted;
    bool m_keepRunning;
    std::vector<int> m_interface;
    
};
    

}


#endif /* GUIThreadParser_hpp */