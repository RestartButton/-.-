#include "Semantico.h"
#include "Constants.h"
#include <iostream>

Semantico::Semantico() {
    this->tabela_simbolos = new list<Simbolo>;
    this->lista_escopos = new stack<string>;
    this->simbolo_atual = NULL;
    this->simbolo_chamado = NULL;
    this->tipo_atual = "";
    this->retorno_atual = "";
    this->lista_escopos->push("global");
}

void Semantico::executeAction(int action, const Token *token)
{
    std::cout << "Acao: " << action << ", Token: "  << token->getId() 
              << ", Lexema: " << token->getLexeme() << std::endl;

    switch(action) {
        case 1: //declara variavel
        {   
            stack<string> temp = *(this->lista_escopos);
            while(!temp.empty()) {
                if(isDec(*(this->tabela_simbolos), token->getLexeme(), temp.top())){
                    declaraSimbolo( *(this->tabela_simbolos), token->getLexeme(), this->tipo_atual, temp.top(), false, false, false, false, false, false, false, 0);
                    break;
                }
                temp.pop();
            }
            break;
        }
        case 2: //declara funcao
        {
            stack<string> temp = *(this->lista_escopos);
            while(!temp.empty()) {
                if(isDec(*(this->tabela_simbolos), token->getLexeme(), temp.top())){
                    declaraSimbolo( *(this->tabela_simbolos), token->getLexeme(), this->retorno_atual, temp.top(), true, false, false, false, false, false, false, 0);
                    this->lescopo_aberto = token->getLexeme();
                    break;
                }
                temp.pop();
            }
            break;
        }
        case 3: //define o tipo a ser trabalhado
        {
            this->tipo_atual = token->getLexeme();
            break;
        }
        case 4: //abre escopo
        {
            this->lista_escopos->push(this->lescopo_aberto);
            this->lescopo_aberto = "";
            break;
        }
        case 5: //fecha escopo
        {
            this->lista_escopos->pop();
            this->simbolo_atual = NULL;
            this->simbolo_chamado = NULL;
            this->tipo_atual = "";
            this->retorno_atual = "";
            break;
        }
        case 6: //define o retorno da funcao
        {
            this->retorno_atual = token->getLexeme();
            break;
        }
        case 99: //limpa comando
        {
            this->simbolo_atual = NULL;
            this->simbolo_chamado = NULL;
            this->tipo_atual = "";
            this->lescopo_aberto = "";
            break;
        }
    }
}

