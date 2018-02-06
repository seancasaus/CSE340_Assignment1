//
//  LexicalAnalyzer2.hpp
//  CSE340_Project1
//
//  Created by Sean Casaus on 2/3/18.
//  Copyright © 2018 sjcasaus. All rights reserved.
//

#ifndef LexicalAnalyzer2_hpp
#define LexicalAnalyzer2_hpp

#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <set>
#include <vector>
#include <iterator>
#include "parser.h"

struct Token2 {
    std::string lexeme;
    std::string tokenName;
};

struct REG_builder {
    struct REG * node;
    std::string REG_name;
};

class LexicalAnalyzer2 {
    public:
        LexicalAnalyzer2();
        Token2 getToken();
        std::vector<REG_builder*> REG_list;
        std::vector<Token2*> finalList;
        std::string inputString;
        std::string test;
        int stringPosition;
    
    private:
        std::string clean(std::string input);

    
};

#endif /* LexicalAnalyzer2_hpp */
