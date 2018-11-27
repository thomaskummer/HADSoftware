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


// namespace HADController {
    
class CommandLineParser {
public:
    
    CommandLineParser(int argc, char** argv)
    {
        for (int i(1); i < argc; ++i)
        {
            std::istringstream issType (argv[i]);
            std::string type;
            if (issType >> type)
            {
                std::istringstream issVal (argv[++i]);
                std::string val;
                if (issVal >> val)
                {
                    std::cout << type << ":" << val << std::endl;
                    m_tasks.emplace(type, val);
                }
            }
            
        }
    }
    
    virtual ~CommandLineParser(){}
    
    const double operator()(const std::string type) const
    {
        return m_tasks.find(type)->second;
    }
    
    void printAll()
    {
        std::cout << "CommandLineParser::printAll()" << std::endl;
        for (auto& task : m_tasks)
        {
            std::cout << "  [" << task.first << ':' << task.second << ']' << std::endl;
        }
    }
        
protected:
    
    std::map<std::string, double> m_tasks;

};
    

// }


#endif /* CommandLineParser_hpp */
