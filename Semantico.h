#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "Token.h"
#include "SemanticError.h"
#include <list>
#include "Tabela.h"

class Semantico
{
public:
    void executeAction(int action, const Token *token);
private:
    list<Simbolo> tabela_simbolos;
};

#endif
