//
//  CommandLineParser.hpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#ifndef CommandLineParser_hpp
#define CommandLineParser_hpp

#include <iostream>
#include <fstream>
#include <assert.h>
#include "InputParser.hpp"

namespace HeartrateControllerSpace {
    
class CommandLineParser : public InputParser {
public:
    
    CommandLineParser(){}
    
    virtual ~CommandLineParser(){}
    
    void readCommandLine(int argc, char** argv)
    {
        std::istringstream issMode (argv[1]);
        if (argc < 2)
        {
            std::cout << "gui\n";
            issMode.str("-gui");
        }
        else std::cout << "gudi\n";


        std::string mode;
        if (issMode >> mode) m_tasks.emplace(mode, 1.0);
        
        for (int i(2); i < argc; ++i)
        {
            std::istringstream issType (argv[i]);
            std::string type;
            if (issType >> type)
            {
                std::istringstream issVal (argv[++i]);
                double val(0);
                if (issVal >> val)
                {
                    std::cout << type << ":" << val << std::endl;
                    m_tasks.emplace(type, val);
                }
                
            }
            
        }
    }

    
protected:
    

};
    

}


#endif /* CommandLineParser_hpp */
