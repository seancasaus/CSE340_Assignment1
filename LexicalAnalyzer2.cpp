//
//  LexicalAnalyzer2.cpp
//  CSE340_Project1
//
//  Created by Sean Casaus on 2/3/18.
//  Copyright © 2018 sjcasaus. All rights reserved.
//
#include <iostream>
#include <cstdlib>
#include <set>
#include <iterator>
#include "parser.h"
#include "LexicalAnalyzer2.hpp"

using namespace std;

LexicalAnalyzer2::LexicalAnalyzer2() {
    stringPosition = 0;
}

Token2 LexicalAnalyzer2::getToken() {
    Parser parser;
    REG_builder * current = new REG_builder;
    std::string currentLongest = "";
    std::string cleanedString = clean(inputString);
    inputString = cleanedString;

    while (stringPosition < inputString.length() && inputString[stringPosition] != ' ') {
        for (auto i = REG_list.begin(); i != REG_list.end(); ++i) {
            std::string temp = parser.match((*i)->node, inputString, stringPosition);
            if (temp.length() > currentLongest.length()) {
                current->REG_name = (*i)->REG_name;
                current->node = (*i)->node;
                currentLongest = temp;
            }
        }
        stringPosition += currentLongest.length();
    }
    Token2 * token = new Token2;
    token->lexeme = currentLongest;
    token->tokenName = current->REG_name;
    return *token;
}

std::string LexicalAnalyzer2::clean(std::string input) {
    int length = (int)input.length();
    if (input[0] == ' ') {
        input.erase(0,1);
    }
    if (input[length-2] == ' ') {
        input.erase(length-2, 1);
    }
    return input;
}


