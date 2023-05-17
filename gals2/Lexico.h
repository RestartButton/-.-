#ifndef LEXICO_H
#define LEXICO_H

#include "Token.h"
#include "LexicalError.h"

#include <string>
#include <iostream>

class Lexico
{
public:
    Lexico(const std::string &input) { setInput(input); }
    Lexico() : position(0), input("") { }

    void setInput(const std::string &input);
    void setPosition(unsigned pos) { position = pos; }
    Token *nextToken();

private:
    unsigned position;
    std::string input;

    int nextState(unsigned char c, int state) const;
    TokenId tokenForState(int state) const;

    bool hasInput() const { return position < input.size(); }
    char nextChar() { return hasInput() ? input[position++] : (char) -1; }
};

#endif
