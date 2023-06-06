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
    this->atribuindo = false;
    this->prim_elem_exp = true;
    this->part_instru = "";
    this->vetor_chamado = "";
    this->vetorando = false;
}

void Semantico::executeAction(int action, const Token *token) throw (SemanticError)
{
    stack<string> temp = *(this->lista_escopos);
    cout << "Acao: " << action << ", Token: "  << token->getId() 
         << ", Lexema: " << token->getLexeme() << endl;

    

    switch(action) {
        case 1: //declara variavel
        {   
            bool isDecla = false;

            if(isDec(*(this->tabela_simbolos), token->getLexeme(), this->lista_escopos->top())){
                isDecla = true;
                break;
            }

            if(isDecla){
                throw SemanticError("Variavel " + token->getLexeme() +  " ja declarada neste escopo!", token->getPosition());
            } else {
                temp = *(this->lista_escopos);
                declaraSimbolo( *(this->tabela_simbolos), token->getLexeme(), this->tipo_atual, temp.top(), false, false, false, false, false, false, false, 0);
                this->simbolo_atual = getSimbolo( *(this->tabela_simbolos), token->getLexeme(), temp.top() );
                string instruction = token->getLexeme() + ": 0";
                geraCodigo(instruction,".data");
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

            if(isDec(*(this->tabela_simbolos), token->getLexeme(), this->lista_escopos->top())){
                isDecla = true;
                break;
            }

            if(isDecla){
                throw SemanticError("Vetor " + token->getLexeme() +  " ja declarada neste escopo!", token->getPosition());
            } else {
                temp = *(this->lista_escopos);
                declaraSimbolo( *(this->tabela_simbolos), token->getLexeme(), this->tipo_atual, temp.top(), false, false, false, true, false, false, false, 0);
                this->simbolo_atual = getSimbolo( *(this->tabela_simbolos), token->getLexeme(), temp.top() );
            }
            break;
        }
        case 9: //inicializa vet //cpah outros???
        {
            this->atribuindo = true;
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

                    if(this->atribuindo || this->vetorando){
                        if(this->prim_elem_exp){
                            string instruction = "\tLD " + token->getLexeme();
                            geraCodigo(instruction, ".text");
                        } else {
                            string instruction = this->part_instru + token->getLexeme();
                            geraCodigo(instruction, ".text");
                        }
                    }

                    break;
                }
                temp.pop();
            }
            if(!isInici){
                this->lista_warnings->push("Variavel " + token->getLexeme() + " usada sem ser inicializada!");
            }
            if(!isDecla){
                throw SemanticError(("Variavel " + token->getLexeme() +  " nao declarada neste escopo!"), token->getPosition());
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
            if(!isInici){
                this->lista_warnings->push("Funcao " + token->getLexeme() + " usada sem ser inicializada!");
            }
            if(!isDecla){
                throw SemanticError(("Funcao " + token->getLexeme() +  " nao declarada neste escopo!"), token->getPosition());
            }
            break;
        }
        case 13: //atribuição
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
        case 15: //literal inteiro
        {
            if(this->atribuindo || this->vetorando){
                if(this->prim_elem_exp){
                    this->prim_elem_exp = false;
                    string instruction = "\tLDI " + token->getLexeme();
                    geraCodigo(instruction, ".text");
                } else {
                    string instruction = this->part_instru + "I " + token->getLexeme();
                    geraCodigo(instruction, ".text");
                }
            }
            break;
        }
        case 16: //literal binario
        {
            break;
        }
        case 17: //soma
        {
            if(this->atribuindo || this->vetorando) {
                this->part_instru = "\tADD";
            }
            break;

        }
        case 18: //subtracao
        {
            if(this->atribuindo || this->vetorando) {
                this->part_instru = "\tSUB";
            }
            break;
        }
        case 19: //chamada vetor
        {
            bool isDecla = false;
            bool isInici = false;
            stack<string> temp = *(this->lista_escopos);
            while(!temp.empty()) {
                if(isDec(*(this->tabela_simbolos), token->getLexeme(), temp.top())){
                    this->vetor_chamado = token->getLexeme(); //THE DO!
                    isDecla = true;
                    if(isInit(*(this->tabela_simbolos), token->getLexeme(), temp.top())) {
                        isInici = true;
                    }
                    break;
                }
                temp.pop();
            }
            if(!isInici){
                this->lista_warnings->push("Vetor " + token->getLexeme() + " usada sem ser inicializada!");
            }
            if(!isDecla){
                throw SemanticError(("Vetor " + token->getLexeme() +  " nao declarada neste escopo!"), token->getPosition());
            }
            break;
        }
        case 20: //iniciando indice
        {
            this->vetorando = true;
            break;
        }
        case 21: //carregando vetor
        {
            geraCodigo("\tSTO $indr",".text");
            string instruction = "\tLDV " + this->vetor_chamado;
            geraCodigo(instruction, ".text");
            break;
        }
        case 22: //salva vetor
        {
                
            if(this->atribuindo_vet){
                geraCodigo("\tLD TEMP2\n\tSTO $indr",".text");
            } else {
                geraCodigo("\tLD TEMP1\n\tSTO $indr",".text");
            }

            break;
        }
        case 23: //atribuição vetor
        {

        }
        case 99: //limpa comando
        {
            if(this->atribuindo) {
                string instruction = "\tSTO " + this->simbolo_atual->nome;
                geraCodigo(instruction, ".text");
            }


            this->simbolo_atual = NULL;
            this->simbolo_chamado = NULL;
            this->tipo_atual = "";
            this->lescopo_aberto = "";
            this->param_count = 0;


            this->atribuindo = false;
            this->prim_elem_exp = true;
            this->part_instru = "";

            this->vetor_chamado = "";
            this->vetorando = false;

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

