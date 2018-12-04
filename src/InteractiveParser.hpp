//
//  InteractiveParser.hpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#ifndef InteractiveParser_hpp
#define InteractiveParser_hpp

#include <iostream>
#include <fstream>
#include <assert.h>

#define mm * -1600

namespace HeartrateControllerSpace {
    
class InteractiveParser {
public:
    
    InteractiveParser(){}
    
    virtual ~InteractiveParser(){}
    
    void run()
    {
        std::cout << "\n====================================================" << std::endl;
        std::cout << "Heartrate-controller for Maxon EPOS2 70/10" << std::endl;
        std::cout << "Institute of fluid dynamics - ETH Zurich" << std::endl;
        std::cout << "====================================================" << std::endl;
        
        std::string mode;
        std::cout << "\nType 'run' to start-up the controller or 'help' for \nmore information. \n>>> ";
        std::cin >> mode;
        if ( mode.compare("help") == 0 ) std::cout << "This is the still todo help page" << std::endl;
        
        HADController controller;
        controller.activateProfileMode();
        
        std::cout << "\nDefine the mode of motion (for help type h or help) \n>>> ";

        while ( std::cin >> mode )
        {
            if ( mode.compare("exit") == 0 || mode.compare("quit") == 0 || mode.compare("q") == 0 )
            {
                std::cout << ">>> Exiting heartrate-controller.";
                break;
            }
            
            std::cout << mode << std::endl;
            
            std::istringstream iss(mode);
            std::string a, b;
            
            if (iss >> a >> b)
            {
                std::cout << "iss: " << a << " " << b << std::endl;
                if ( a.compare("move") == 0 )
                {
                    controller.move(std::stoi(b) mm);
                    controller.printSpindlePosition();
                }
            }
            
            std::cout << "Define the mode of motion (for help type h or help) >>> ";
        }
    }
    
    
protected:
    

};
    

}


#endif /* InteractiveParser_hpp */
