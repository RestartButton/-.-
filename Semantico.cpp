#include "Semantico.h"
#include "Constants.h"
#include <iostream>

using namespace std;

Semantico::Semantico() {
    this->tabela_simbolos = new list<Simbolo>;
    this->lista_escopos = new stack<string>;
    this->simbolo_atual = NULL;
    this->simbolo_chamado = NULL;
    this->tipo_atual = "";
    this->retorno_atual = "";
    this->lista_escopos->push("global");
    this->param_count = 0;
}

void Semantico::executeAction(int action, const Token *token)
{
    cout << "Acao: " << action << ", Token: "  << token->getId() 
         << ", Lexema: " << token->getLexeme() << std::endl;

    switch(action) {
        case 1: //declara variavel
        {   
            
            stack<string> temp = *(this->lista_escopos);
            while(!temp.empty()) {
                if(isDec(*(this->tabela_simbolos), token->getLexeme(), temp.top())){
                    declaraSimbolo( *(this->tabela_simbolos), token->getLexeme(), this->tipo_atual, temp.top(), false, false, false, false, false, false, false, 0);
                    this->simbolo_atual = getSimbolo( *(this->tabela_simbolos), token->getLexeme(), temp.top() );
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
                    this->simbolo_atual = getSimbolo( *(this->tabela_simbolos), token->getLexeme(), temp.top() );
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
            this->simbolo_atual->inic = true;
            this->param_count = 0;
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
        case 6: //define o tipo do retorno da funcao
        {
            this->retorno_atual = token->getLexeme();
            break;
        }
        case 7: //declara param
        {
            if(!isParam(*(this->tabela_simbolos), token->getLexeme(), this->lescopo_aberto)){
                declaraSimbolo( *(this->tabela_simbolos), token->getLexeme(), this->tipo_atual, this->lescopo_aberto, false, false, false, false, true, false, false, ++this->param_count);
                break;
            }
            break;
        }
        case 8: //declara vet
        {
            stack<string> temp = *(this->lista_escopos);
            while(!temp.empty()) {
                if(isDec(*(this->tabela_simbolos), token->getLexeme(), temp.top())){
                    declaraSimbolo( *(this->tabela_simbolos), token->getLexeme(), this->tipo_atual, temp.top(), false, false, false, true, false, false, false, 0);
                    this->simbolo_atual = getSimbolo( *(this->tabela_simbolos), token->getLexeme(), temp.top() );
                    break;
                }
                temp.pop();
            }
            break;
        }
        case 9: //inicializa vet //cpah outros???
        {
            this->simbolo_atual->inic = true;
            break;
        }
        case 10: //empilha variavel
        {
            stack<string> temp = *(this->lista_escopos);
            while(!temp.empty()) {
                if(isDec(*(this->tabela_simbolos), token->getLexeme(), temp.top())){
                    this->simbolo_chamado = getSimbolo( *(this->tabela_simbolos), token->getLexeme(), temp.top() );
                    break;
                }
            }
        }
        case 99: //limpa comando
        {
            this->simbolo_atual = NULL;
            this->simbolo_chamado = NULL;
            this->tipo_atual = "";
            this->lescopo_aberto = "";
            this->param_count = 0;
            break;
        }
    }
}

