#include "lib/Semantico.h"
#include "lib/Constants.h"

#include <iostream>

void Semantico::executeAction(int action, const Token *token)
{
    std::cout << "Acao: " << action << ", Token: "  << token->getId() 
              << ", Lexema: " << token->getLexeme() << std::endl;
}

