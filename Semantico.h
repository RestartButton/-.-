#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "Token.h"
#include "SemanticError.h"
#include <list>
#include <stack>
#include "Tabela.h"

class Semantico
{
public:
    Semantico();
    void executeAction(int action, const Token *token);
private:
    stack<string>* lista_escopos;
    list<Simbolo>* tabela_simbolos;
    Simbolo* simbolo_atual;
    Simbolo* simbolo_chamado;
    string tipo_atual;
    string retorno_atual;
    string lescopo_aberto;
};

#endif
