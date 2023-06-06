#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "Token.h"
#include "SemanticError.h"
#include <list>
#include <stack>
#include <deque>
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
    deque<string>* part_instru;
    Simbolo* simbolo_atual;
    Simbolo* simbolo_chamado;
    string codigo_assembly;
    string tipo_atual;
    string retorno_atual;
    string lescopo_aberto;
    string vetor_chamado;

    int param_count;
    int pseudo_escopo_count;
    int temp_count;
    int exp_vet_count;

    bool atribuindo;
    bool atribuindo_vet;
    bool prim_elem_exp;
    bool vetorando;
};

#endif