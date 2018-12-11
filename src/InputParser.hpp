//
//  InputParser.hpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#ifndef InputParser_hpp
#define InputParser_hpp

#include <iostream>
#include <fstream>
#include <assert.h>


namespace HeartrateControllerSpace {
    
class InputParser {
public:
    
    InputParser(){}

    virtual ~InputParser(){}
    
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
    
    double& operator[](const std::string& type)
    {
        return m_tasks.find(type)->second;
    }
    
    const std::map<std::string, double>& tasks()
    {
        return m_tasks;
    }

    void printAll()
    {
        std::cout << "InputParser::printAll()" << std::endl;
        for (auto& task : m_tasks)
        {
            std::cout << "  [" << task.first << ':' << task.second << "]" << std::endl;
        }
    }
        
protected:
    
    std::map<std::string, double> m_tasks;


};
    

}


#endif /* InputParser_hpp */
