/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <set>
#include <iterator>
#include "lexer.h"
#include "LexicalAnalyzer2.hpp"

struct REG_node {
    struct REG_node * first_neighbor;
    char first_label;
    struct REG_node * second_neighbor;
    char second_label;
};

struct REG {
    struct REG_node * starting;
    struct REG_node * accepting;
};



class Parser {
  private:
    LexicalAnalyzer lexer;
    //LexicalAnalyzer2 regLex;

    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek();


    void parse_input(LexicalAnalyzer2*regLex);
    void parse_char_list();
    void parse_tokens_section(LexicalAnalyzer2*regLex);
    void parse_token_list(LexicalAnalyzer2*regLex);
    void parse_token(LexicalAnalyzer2*regLex);
    REG* parse_expr();
    std::set<REG_node*> matchOneChar(std::set<REG_node*> S, char c);
    
  public:
    void ParseProgram(LexicalAnalyzer2*regLex);
    std::string match(REG* r, std::string s, int p);
};

#endif

