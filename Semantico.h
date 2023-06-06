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
    void executeAction(int action, const Token *token) throw (SemanticError);
    list<Simbolo>* tabela_simbolos;
    stack<string>* lista_warnings;
    string getAssembly();
private:
    void geraCodigo(string instrucao, string onde);
    stack<string>* lista_escopos;
    Simbolo* simbolo_atual;
    Simbolo* simbolo_chamado;
    string codigo_assembly;
    string tipo_atual;
    string retorno_atual;
    string lescopo_aberto;
    string part_instru;
    string vetor_chamado;

    int param_count;
    int pseudo_escopo_count;
    int temp_count;

    bool atribuindo;
    bool atribuindo_vet;
    bool prim_elem_exp;
    bool vetorando;
};

#endif