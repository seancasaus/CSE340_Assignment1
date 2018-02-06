/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include <set>
#include <iterator>

#include "parser.h"

using namespace std;

void Parser::syntax_error()
{
    cout << "Syntax Error\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// this function simply checks the next token without
// consuming the input
// Written by Mohsen Zohrevandi
Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

// Parsing

void Parser::parse_input(LexicalAnalyzer2*regLex)
{
    //input -> tokens_section INPUT_TEXT

    parse_tokens_section(regLex);
    (*regLex).inputString = expect(INPUT_TEXT).lexeme;
}

/*void Parser::parse_alphabet_section()
{
    //alphabet_section -> char_list HASH

    parse_char_list();
    expect(HASH);
}
*/


void Parser::parse_tokens_section(LexicalAnalyzer2*regLex)
{
    // tokens_section -> token_list HASH


    parse_token_list(regLex);
    expect(HASH);
}

void Parser::parse_token_list(LexicalAnalyzer2*regLex)
{
    // token_list -> token
    // token_list -> token COMMA token_list

    parse_token(regLex);
    Token t = peek();
    if (t.token_type == COMMA)
    {
        // token_list -> token COMMA token_list
        expect(COMMA);
        parse_token_list(regLex);
    }
    else if (t.token_type == HASH)
    {
        // token_list -> token
    }
    else
    {
        syntax_error();
    }

}

void Parser::parse_token(LexicalAnalyzer2*regLex)
{
    // token -> ID expr

    string name = expect(ID).lexeme;
    REG* temp = parse_expr();

    REG_builder * newBuilder = new REG_builder;
    newBuilder->node = temp;
    newBuilder->REG_name = name;
    (*regLex).REG_list.push_back (newBuilder);
}

REG* Parser::parse_expr()
{
    // expr -> CHAR
    // expr -> LPAREN expr RPAREN DOT LPAREN expr RPAREN
    // expr -> LPAREN expr RPAREN OR LPAREN expr RPAREN
    // expr -> LPAREN expr RPAREN STAR
    // expr -> UNDERSCORE

    Token t = lexer.GetToken();
    if(t.token_type == CHAR){
        // expr -> CHAR
        REG_node * node1 = new REG_node;
        REG_node * node2 = new REG_node;
        node1->first_label = t.lexeme[0];
        //std::cout << node1->first_label <<endl;
        node1->second_label = '\0';
        node2->first_label = '\0';
        node2->second_label = '\0';
        node1->first_neighbor = node2;
        node1->second_neighbor = NULL;
        node2->first_neighbor = NULL;
        node2->second_neighbor = NULL;
        REG * R1 = new REG; //maybe scope conflict
        R1->starting = node1;
        R1->accepting = node2;
        //std::cout << node1->first_neighbor->first_label << endl;
        
        return R1;
    }
    else if (t.token_type == UNDERSCORE){
        // expr -> CHAR
        REG_node * node1 = new REG_node;
        REG_node * node2 = new REG_node;
        node1->first_label = '_';
        //std::cout << node1->first_label <<endl;
        node1->second_label = '\0';
        node2->first_label = '\0';
        node2->second_label = '\0';
        node1->first_neighbor = node2;
        node1->second_neighbor = NULL;
        node2->first_neighbor = NULL;
        node2->second_neighbor = NULL;
        REG * R1 = new REG; //maybe scope conflict
        R1->starting = node1;
        R1->accepting = node2;
        return R1;
    }
    else if(t.token_type == LPAREN){
        // expr -> LPAREN expr RPAREN DOT LPAREN expr RPAREN
        // expr -> LPAREN expr RPAREN OR LPAREN expr RPAREN
        // expr -> LPAREN expr RPAREN STAR
//        parse_expr();
        REG * R2 = parse_expr();
        expect(RPAREN);
        Token t2 = lexer.GetToken();
        if(t2.token_type == DOT || t2.token_type == OR){
            expect(LPAREN);
//            parse_expr();
//            expect(RPAREN);
            
            if (t2.token_type == OR) {
                REG * R1 = parse_expr();
                expect(RPAREN);
                REG * ROr = new REG;
                REG_node * node1 = new REG_node;
                REG_node * node2 = new REG_node;
                node1->first_label = '_';
                node1->second_label = '_';
                R2->accepting->first_label = '_';
                R2->accepting->second_label = '\0';
                R1->accepting->first_label = '_';
                R1->accepting->second_label = '\0';
                node1->first_neighbor = R2->starting;
                node1->second_neighbor = R1->starting;
                R2->accepting->first_neighbor = node2;
                R2->accepting->second_neighbor = NULL;
                R1->accepting->first_neighbor = node2;
                R1->accepting->second_neighbor = NULL;
                ROr->starting = node1;
                ROr->accepting = node2;
                
                return ROr;
            }
            else if (t2.token_type == DOT) {
                REG * R1 = parse_expr();
                expect(RPAREN);
                REG * RC = new REG;
                R2->accepting->first_label = '_';
                R2->accepting->second_label = '\0';
                R2->accepting->first_neighbor = R1->starting;
                R1->accepting->first_label = '\0';
                R1->accepting->second_label = '\0';
                R2->accepting->second_neighbor = NULL;
                R1->accepting->first_neighbor = NULL;
                R1->accepting->second_neighbor = NULL;
                RC->starting = R2->starting;
                RC->accepting = R1->accepting;
                
                return RC;
            }
        }
        else if(t2.token_type == STAR)
        {
            //REG * R2 = parse_expr();
            REG * RStar = new REG;
            REG_node * node3 = new REG_node;
            REG_node * node4 = new REG_node;
            node3->first_label = '_';
            node3->second_label = '_';
            node4->first_label = '\0';
            node4->second_label = '\0';
            node3->first_neighbor = R2->starting;
            node3->second_neighbor = node4;
            node4->first_neighbor = NULL;
            node4->second_neighbor = NULL;
            R2->accepting->second_neighbor = node4;

            RStar->starting = node3;
            RStar->accepting = node4;
            //std::cout << R2->accepting->second_neighbor->first_label;
            
            return RStar;
        }
        return NULL;
    }
    else
    {
        syntax_error();
        return NULL;
    }
}

std::set<REG_node*> Parser::matchOneChar(std::set<REG_node*> S, char c) {
    //1. find all nodes that can be reached from S by consuming c
    set <REG_node*, less <REG_node*> > temp;
    set <REG_node*, less <REG_node*> > :: iterator it;
    for (it = S.begin(); it != S.end(); it++) {
        if (c == '\0') {
            return temp;
        }
        if ((*it)->first_label == c)  {
            temp.insert((*it)->first_neighbor);
        }
        if ((*it)->second_label == c)  {
            temp.insert((*it)->second_neighbor);
        }
    }

    if (temp.empty() == true) {
        return temp;
    }
    //    //2. find all all nodes that can be reached from the resulting set S by consuming no input
    bool changed = true;
    while (changed) {
        changed = false;
        std::pair<std::set<REG_node*>::iterator,bool> ret;
        for (it = temp.begin(); it != temp.end(); it++) {
            //first node
            if ((*it)->first_label == '_') {
                ret = temp.insert((*it)->first_neighbor);
                //checks if neighbor was inserted
                if(ret.second == false)
                {
                    changed = false;
                }
                else {
                    changed = true;
                }
            }
            //second node
            if ((*it)->second_label == '_') {
                ret = temp.insert((*it)->second_neighbor);
                //checks if neighbor was inserted
                if (ret.second == false) {
                    changed = false;
                }
                else {
                    changed = true;
                }
            }
        }
    }
    return temp;
}

std::string Parser::match(REG* R, std::string s, int p) {
    int i = p;
    int currentMax = 0;
    set <REG_node*, less <REG_node*> > :: iterator it;
    std::string final = "";
    set <REG_node*, less <REG_node*> > start;
    start.insert(R->starting);
    set <REG_node*, less <REG_node*> > set2;
    set2 = matchOneChar(start, '_');
    bool changed = true;
    while(changed) {
        changed = false;
        set2 = matchOneChar(set2, s[i]);
        i++;
        it = set2.find(R->accepting);
        if (it != set2.end()) {
            currentMax = i;
            changed = true;
        }
        if(s[p + 1] == ' ') {
            break;
        }
        if (set2.empty()){
            break;
        }
    }

    if (currentMax == 0) {
        return "";
    }
    else {
        for (int i = p; i <= currentMax; i++) {
            final += s[i];
        }
    }
    
    return final;
}

void Parser::ParseProgram(LexicalAnalyzer2*regLex)
{
    parse_input(regLex);
    expect(END_OF_FILE);
}

int main()
{
    LexicalAnalyzer2 * regLex = new LexicalAnalyzer2();
    
    Parser parser;
    parser.ParseProgram(regLex);

    bool isEmpty = false;
    while (!isEmpty) {
        isEmpty = true;
        Token2 newToken = regLex->getToken();
        std::cout << newToken.tokenName + " , " + "\"" + newToken.lexeme + "\"" << endl;
        if (regLex->inputString[regLex->stringPosition] == ' ') {
            for (int i = regLex->stringPosition; i > 0; i--) {
                regLex->inputString.erase(i,1);
            }
            regLex->stringPosition = 0;
            isEmpty = false;
        }
        else if (regLex->stringPosition == regLex->inputString.length()) {
            return 0;
        }
    }
}
