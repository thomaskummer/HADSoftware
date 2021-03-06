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

# define interfaceSize 18
    
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
        readGUIActions();
    }
        
    void requestInput()
    {
        std::cout << ">>> ";
    }

    void requestInput(std::string& line)
    {
        std::cout << ">>> ";
        std::getline(std::cin, line);
    }
    
    void readGUIActions()
    {
        std::cout << "\nType help to see available commands.\n";
        std::string line;

        while (m_waitingForInput)
        {
            requestInput(line);
            std::istringstream iss(line);
            std::string task, value1, value2, value3, value4;
            iss >> task; // >> value1 >> value2 >> value3 >> value4;

            if (!task.compare("move") || !task.compare("mv"))
            {
                iss >> value1;
                m_interface[0] = 1;
                m_interface[1] = std::stoi(value1);
                
                m_tasks["-pd"] = std::stoi(value1);
                m_keepRunning = true;
                m_taskSubmitted = true;
            }
            
            if (!task.compare("move-to") || !task.compare("mvto"))
            {
                iss >> value1;
                m_interface[14] = 1;
                m_interface[15] = std::stoi(value1);
                
                m_tasks["-pd"] = std::stoi(value1);
                m_keepRunning = true;
                m_taskSubmitted = true;
            }
            
            if (!task.compare("ppm") || !task.compare("position-profile-mode"))
            {
                iss >> value1; iss >> value2;
        
                m_interface[16] = 1;
                m_interface[17] = (value1.empty() ? 10 : std::stoi(value1));
                
                m_tasks["-ppa"] = (value1.empty() ? 10 : std::stoi(value1));
                m_tasks["-ppf"] = (value2.empty() ? 30 : std::stoi(value2));

                m_keepRunning = true;
                m_taskSubmitted = true;
            }
        
            if (!task.compare("ipm"))
            {
                iss >> value1; iss >> value2; iss >> value3; iss >> value4;
                m_interface[2] = 1;
                m_interface[3] = (value1.empty() ? -10 : std::stoi(value1));
                
                m_tasks["-ia"] = (value1.empty() ? -10 : std::stoi(value1));
                m_tasks["-ip"] = (value2.empty() ? 60000. / 72. : 60000. / std::stod(value2));
                m_tasks["-if"] = (value3.empty() ? 1 : std::stoi(value3));
                m_tasks["-ias"] = (value4.empty() ? 0.35 : std::stod(value4));

                m_keepRunning = true;
                m_taskSubmitted = true;
            }
            
            if (!task.compare("cd"))
            {
                iss >> value1; iss >> value2;

                if (value1 == "bpm") {
                    m_tasks["-ip"] = (value2.empty() ? 60000. / 72. : 60000. / std::stod(value2));
                }
                else if (value1 == "amp" || value1 == "amplitude") {
                    m_tasks["-ia"] = (value2.empty() ? -10 : std::stoi(value2));
                }
                else if (value1 == "syf" || value1 == "systolic-fraction") {
                    m_tasks["-ias"] = (value2.empty() ? 0.3 : std::stod(value2));
                }
                else if (value1 == "ppa" || value1 == "pp-a") {
                    m_tasks["-ppa"] = (value2.empty() ? 10 : std::stoi(value2));
                }
                else if (value1 == "ppf" || value1 == "pp-f") {
                    m_tasks["-ppf"] = (value2.empty() ? 60 : std::stoi(value2));
                }
                else std::cout << "Provide valid argument to cd" << std::endl;
            }

            if (!task.compare("cp") || !task.compare("cont-plus-push") || !task.compare("home-plus"))
            {
                iss >> value1;
                m_interface[8] = 1;
                m_interface[9] = (value1.empty() ? -2 : -std::stoi(value1));
                
                m_tasks["-vs"] = (value1.empty() ? -2 : -std::stoi(value1));
                m_keepRunning = true;
                m_taskSubmitted = true;
            }

            if (!task.compare("cm") || !task.compare("cont-minus-push") || !task.compare("home-minus"))
            {
                iss >> value1;
                m_interface[12] = 1;
                m_interface[13] = (value1.empty() ? 2 : std::stoi(value1));
                
                m_tasks["-vs"] = (value1.empty() ? 2 : std::stoi(value1));
                m_keepRunning = true;
                m_taskSubmitted = true;
            }
            
            if (!task.compare("cr") || !task.compare("cont-plus-release") || !task.compare("cont-minus-release") || !task.compare("home-stop"))
            {
                iss >> value1;
                m_interface[10] = 1;
                m_interface[11] = (value1.empty() ? 0 : -std::stoi(value1));
                
                m_tasks["-vs"] = (value1.empty() ? 0 : -std::stoi(value1));
                m_keepRunning = false;
                m_taskSubmitted = true;
            }
            
            if (!task.compare("help"))
            {
                m_interface[4] = 1;
                m_taskSubmitted = true;
                sleep(1);
            }
            
            if (!task.compare("reset"))
            {
                m_interface[6] = 1;
                m_taskSubmitted = true;
            }

            if (!task.compare("stop"))
            {
                m_keepRunning = false;
            }
            
            if (!task.compare("exit"))
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
    
    bool& keepRunning()
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
