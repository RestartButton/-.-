#include "Semantico.h"
#include "Constants.h"
#include <iostream>

using namespace std;

Semantico::Semantico() {
    this->tabela_simbolos = new list<Simbolo>;
    this->lista_escopos = new stack<string>;
    this->lista_warnings = new stack<string>;
    this->codigo_assembly = ".data\n.text\n";
    this->simbolo_atual = NULL;
    this->simbolo_chamado = NULL;
    this->tipo_atual = "";
    this->retorno_atual = "";
    this->lista_escopos->push("global");
    this->param_count = 0;
    this->pseudo_escopo_count = 0;
}

void Semantico::executeAction(int action, const Token *token) throw (SemanticError)
{
    stack<string> temp = *(this->lista_escopos);
    cout << "Acao: " << action << ", Token: "  << token->getId() 
         << ", Lexema: " << token->getLexeme() << std::endl;

    

    switch(action) {
        case 1: //declara variavel
        {   
            bool isDecla = false;
            stack<string> temp = *(this->lista_escopos);
            while(!temp.empty()) {
                if(isDec(*(this->tabela_simbolos), token->getLexeme(), temp.top())){
                    isDecla = true;
                    break;
                }
                temp.pop();
            }
            if(isDecla){
                throw SemanticError("Variavel " + token->getLexeme() +  " ja declarada neste escopo!", token->getPosition());
            } else {
                temp = *(this->lista_escopos);
                declaraSimbolo( *(this->tabela_simbolos), token->getLexeme(), this->tipo_atual, temp.top(), false, false, false, false, false, false, false, 0);
                this->simbolo_atual = getSimbolo( *(this->tabela_simbolos), token->getLexeme(), temp.top() );
            }

            break;
        }
        case 2: //declara funcao
        {
            bool isDecla = false;
            stack<string> temp = *(this->lista_escopos);
            while(!temp.empty()) {
                if(isDec(*(this->tabela_simbolos), token->getLexeme(), temp.top())){
                    isDecla = true;
                    break;
                }
                temp.pop();
            }
            if(isDecla){
                throw SemanticError("Funcao " + token->getLexeme() +  " ja declarada neste escopo!", token->getPosition());
            } else {
                temp = *(this->lista_escopos);
                declaraSimbolo( *(this->tabela_simbolos), token->getLexeme(), this->retorno_atual, temp.top(), true, false, false, false, false, false, false, 0);
                this->simbolo_atual = getSimbolo( *(this->tabela_simbolos), token->getLexeme(), temp.top() );
                this->lescopo_aberto = token->getLexeme();
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
        case 5: //fecha escopo (sem jump)
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
            bool isDecla = false;
            if(isParam(*(this->tabela_simbolos), token->getLexeme(), this->lescopo_aberto)){
                isDecla = true;
                break;
            }
            if(isDecla) {
                throw SemanticError("Variavel " + token->getLexeme() +  " ja declarada neste escopo!", token->getPosition());
            } else {
                declaraSimbolo( *(this->tabela_simbolos), token->getLexeme(), this->tipo_atual, this->lescopo_aberto, false, false, false, false, true, false, false, ++this->param_count);
            }
            break;
        }
        case 8: //declara vet
        {
            bool isDecla = false;
            stack<string> temp = *(this->lista_escopos);
            while(!temp.empty()) {
                if(isDec(*(this->tabela_simbolos), token->getLexeme(), temp.top())){
                    isDecla = true;
                    break;
                }
                temp.pop();
            }
            if(isDecla){
                throw SemanticError("Variavel " + token->getLexeme() +  " ja declarada neste escopo!", token->getPosition());
            } else {
                temp = *(this->lista_escopos);
                declaraSimbolo( *(this->tabela_simbolos), token->getLexeme(), this->tipo_atual, temp.top(), false, false, false, true, false, false, false, 0);
                this->simbolo_atual = getSimbolo( *(this->tabela_simbolos), token->getLexeme(), temp.top() );
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
            bool isDecla = false;
            bool isInici = false;
            stack<string> temp = *(this->lista_escopos);
            while(!temp.empty()) {
                if(isDec(*(this->tabela_simbolos), token->getLexeme(), temp.top())){
                    this->simbolo_chamado = NULL;
                    this->simbolo_chamado = getSimbolo( *(this->tabela_simbolos), token->getLexeme(), temp.top() );
                    this->simbolo_chamado->usada = true;
                    isDecla = true;
                    if(isInit(*(this->tabela_simbolos), token->getLexeme(), temp.top())) {
                        isInici = true;
                    }
                    break;
                }
                temp.pop();
            }
            if(!isDecla){
                throw SemanticError(("Variavel " + token->getLexeme() +  " nao declarada neste escopo!"), token->getPosition());
            }
            if(!isInici){
                this->lista_warnings->push("Variavel " + token->getLexeme() + " usada sem ser inicializada!");
            }
            break;
        }
        case 11: //declara pseudo escopo
        {
            stack<string> temp = *(this->lista_escopos);
            string pseudo_nome = "pseudo" + std::to_string(this->pseudo_escopo_count++);
            declaraSimbolo( *(this->tabela_simbolos), pseudo_nome, this->retorno_atual, temp.top(), false, false, false, false, false, true, true, 0);
            this->simbolo_atual = getSimbolo( *(this->tabela_simbolos), pseudo_nome, temp.top() );
            this->lescopo_aberto = pseudo_nome;
            break;
        }
        case 12: //usa func
        {
            bool isDecla = false;
            bool isInici = false;
            stack<string> temp = *(this->lista_escopos);
            while(!temp.empty()) {
                if(isDec(*(this->tabela_simbolos), token->getLexeme(), temp.top())){
                    getSimbolo( *(this->tabela_simbolos), token->getLexeme(), temp.top() )->usada = true;
                    isDecla = true;
                    if(isInit(*(this->tabela_simbolos), token->getLexeme(), temp.top())) {
                        isInici = true;
                    }
                    break;
                }
                temp.pop();
            }
            if(!isDecla){
                throw SemanticError(("Funcao " + token->getLexeme() +  " nao declarada neste escopo!"), token->getPosition());
            }
            if(!isInici){
                this->lista_warnings->push("Funcao " + token->getLexeme() + " usada sem ser inicializada!");
            }
            break;
        }
        case 13:
        {
            stack<string> temp = *(this->lista_escopos);
            this->simbolo_atual = getSimbolo( *(this->tabela_simbolos), token->getLexeme(), temp.top() );
            this->simbolo_atual->inic = true;
            break;
        }
        case 14: //fecha escopo (com jump)
        {
            this->lista_escopos->pop();
            this->simbolo_atual = NULL;
            this->simbolo_chamado = NULL;
            this->tipo_atual = "";
            this->retorno_atual = "";
            this->geraCodigo("R" + to_string(this->pseudo_escopo_count) + ":", ".text");
            break;
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

void Semantico::geraCodigo(string instrucao, string onde) {
    if (onde == ".data") {
        size_t posicao = this->codigo_assembly.find(".text");
        if (posicao != std::string::npos) {
            this->codigo_assembly.insert(posicao, instrucao + "\n");
        }
    } else {
        this->codigo_assembly += instrucao + "\n";
    }
}

string Semantico::getAssembly() {
    string resultado = this->codigo_assembly;

    if (!resultado.empty()) {
        while (resultado.back() == '\n') {
            resultado.pop_back();
            if (resultado.empty()) {
                break;
            }
        }
    }
    return resultado;
}

