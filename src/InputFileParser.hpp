//
//  InputFileParser.hpp
//  HeartrateController
//
//  Created by Thomas Kummer on 20.11.18.
//  Copyright © 2018 Thomas Kummer. All rights reserved.
//

#ifndef InputFileParser_hpp
#define InputFileParser_hpp

#include <iostream>
#include <fstream>
#include <assert.h>


// namespace HADController {
    
class InputFileParser {
public:
    
    InputFileParser(const std::string& filename) :
        m_filename(filename)
    {}
    
    virtual ~InputFileParser(){}
    

    const std::string operator()(const std::string& variable) const
    {
        std::ifstream file (m_filename.c_str());
        assert(file.is_open());
        
        std::string line;
        while (std::getline(file, line))
        {
            cutComment(line);
            if (line.empty()) continue;

            std::istringstream iss(line);
            std::string a, b, c;

            if (!(iss >> a >> b >> c)) continue;
            
            if (a == variable) return c;
        }
        
        return "Variable not found.";
    }
    
    const double operator[](const std::string& variable) const
    {
        std::ifstream file (m_filename.c_str());
        assert(file.is_open());
        
        std::string line;
        while (std::getline(file, line))
        {
            cutComment(line);
            if (line.empty()) continue;

            std::istringstream iss(line);
            std::string a, b, c;
            
            if (!(iss >> a >> b >> c)) continue;
            
            if (a == variable) return std::stod(c);
        }
        
        return 0;
    }

        
protected:

    void cutComment(std::string& str) const
    {
        std::size_t posComment = str.find("//");
        str = str.substr(0, posComment);
    }
    
    const std::string m_filename;
    
};
    

// }


#endif /* InputFileParser_hpp */
