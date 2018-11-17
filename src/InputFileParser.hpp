//
//  InputFileParser.hpp
//  FiniteElementAnalysis
//
//  Created by Thomas Kummer on 04.05.17.
//  Copyright © 2017 Thomas Kummer. All rights reserved.
//

#ifndef InputFileParser_hpp
#define InputFileParser_hpp

// #include "BCHandler.hpp"


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

//    void operator()(BCHandler& bcHandler) const
//    {
//        std::ifstream file (m_filename.c_str());
//        assert(file.is_open());
//
//        std::string line;
//        while (std::getline(file, line))
//        {
//            std::istringstream iss(line);
//            std::string a, b, c, d, e;
//
//            if (!(iss >> a >> b >> c >> d >> e)) continue;
//
//            if (a == "BC")
//            {
//                if (b == "Essential" && c == "Full") bcHandler.addBC(Essential, Full, std::stoi(d), std::stod(e));
//                if (b == "Essential" && c == "Component") bcHandler.addBC(Essential, Component, std::stoi(d), std::stod(e));
//
//                if (b == "Natural" && c == "Full") bcHandler.addBC(Natural, Full, std::stoi(d), std::stod(e));
//                if (b == "Natural" && c == "Component") bcHandler.addBC(Natural, Component, std::stoi(d), std::stod(e));
//                if (b == "Natural" && c == "Normal") bcHandler.addBC(Natural, Normal, std::stoi(d), std::stod(e));
//            }
//        }
//    }
    
        
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
