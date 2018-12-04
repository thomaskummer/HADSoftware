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
    
    CommandLineParser(){}
    
    CommandLineParser(int argc, char** argv)
    {
        readCommandLine(argc, argv);
    }
    
    virtual ~CommandLineParser(){}
    
    void readCommandLine(int argc, char** argv)
    {
        std::istringstream issMode (argv[1]);
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
                    //std::cout << type << ":" << val << std::endl;
                    m_tasks.emplace(type, val);
                }
                
            }
            
        }
    }
    
    const std::map<std::string, double>& map() const
    {
        return m_tasks;
    }

    const bool feature(const std::string& arg) const
    {
        return (m_tasks.count(arg) > 0);
    }
    
    const std::pair<std::string, double> operator()(const std::string& type) const
    {
        return std::pair<std::string, double>(type, m_tasks.find(type)->second);
    }
    
    const double& operator[](const std::string& type) const
    {
        return m_tasks.find(type)->second;
    }
    
    void printAll()
    {
        std::cout << "CommandLineParser::printAll()" << std::endl;
        for (auto& task : m_tasks)
        {
            std::cout << "  [" << task.first << ':' << task.second << "]" << std::endl;
        }
    }
        
protected:
    
    std::map<std::string, double> m_tasks;

};
    

// }


#endif /* CommandLineParser_hpp */
