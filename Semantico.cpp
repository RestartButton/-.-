#include "Semantico.h"
#include "Constants.h"
#include <iostream>

using namespace std;

Semantico::Semantico() {
    this->tabela_simbolos = new list<Simbolo>;
    this->lista_warnings = new stack<string>;

    this->lista_escopos = new stack<string>;
    this->part_instru = new deque<string>;

    this->simbolo_atual = NULL;
    this->simbolo_chamado = NULL;

    this->codigo_assembly = ".data\n\tINDICE: 0\n\tTEMP1: 0\n\tTEMP2: 0\n\tTEMP3: 0\n\tTEMP4: 0\n\tTEMP5: 0\n\tTEMP6: 0\n\tTEMP7: 0\n\tTEMP8: 0\n.text\n";
    this->tipo_atual = "";
    this->retorno_atual = "";
    this->lescopo_aberto = "";
    this->vetor_chamado = "";
    this->indice_resultado = "";

    this->param_count = 0;
    this->pseudo_escopo_count = 0;
    this->temp_count = 1;
    this->prim_elem_exp = 0;

    this->atribuindo = false;
    this->atribuindo_vet = false;
    this->vetorando = false;
    
    this->lista_escopos->push("global");

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
                string instruction = "\t" + token->getLexeme() + ": 0";
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
            this->indice_resultado = to_string(this->temp_count);
            this->atribuindo = true;
            this->simbolo_atual->inic = true;
            this->prim_elem_exp++;
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
                            this->prim_elem_exp--;
                            string instruction = "\tLD " + token->getLexeme();
                            geraCodigo(instruction, ".text");
                        } else {
                            string instruction = this->part_instru->front() + token->getLexeme();
                            this->part_instru->pop_front();
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
                    this->prim_elem_exp--;
                    string instruction = "\tLDI " + token->getLexeme();
                    geraCodigo(instruction, ".text");
                } else {
                    string instruction = this->part_instru->back() + "I " + token->getLexeme();
                    this->part_instru->pop_back();
                    geraCodigo(instruction, ".text");
                }
                // string instruction = "\tSTO TEMP" + to_string(this->temp_count;
                // geraCodigo(instruction, ".text");
            } else if(this->atribuindo_vet){
                this->prim_elem_exp--;
                string instruction = "\tLDI " + token->getLexeme() + "\n\tSTO TEMP" + std::to_string(this->temp_count);
                geraCodigo(instruction, ".text");
                this->temp_count++;
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
                this->part_instru->push_back("\tADD");
            }
            break;

        }
        case 18: //subtracao
        {
            if(this->atribuindo || this->vetorando) {
                this->part_instru->push_back("\tSUB");
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
                    this->prim_elem_exp++;
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
            string instruction = "\tLDV " + this->vetor_chamado + "\n\tSTO TEMP" + std::to_string(this->temp_count);
            this->temp_count++;
            this->vetorando = false;
            geraCodigo(instruction, ".text");
            break;
        }
        case 22: // declara as posições do vetor
        {
            string instruction = "\t" + this->simbolo_atual->nome + ": 0";

            for(int i = 1; i < atoi(token->getLexeme().c_str()); i++){
                instruction += ", 0";
            }
            geraCodigo(instruction, ".data");

            break;
        }
        case 23: //leia
        {
            string instruction = "\tLD $in_port\n\tSTO " + token->getLexeme();
            geraCodigo(instruction,".text");
            break;
        }
        case 24: //inicializa vet 
        {
            geraCodigo("\tSTO INDICE", ".text");
            stack<string> temp = *(this->lista_escopos);
            this->simbolo_atual = getSimbolo( *(this->tabela_simbolos), this->vetor_chamado, temp.top() );
            this->atribuindo_vet = true;
            this->simbolo_atual->inic = true;
            this->prim_elem_exp++;
            this->indice_resultado = to_string(this->temp_count);
            this->vetorando = false;
            break;
        }
        case 25: //escreva literal
        {
            string instruction = "\tLDI " + token->getLexeme() + "\n\tSTO $out_port";
            geraCodigo(instruction,".text");
            break;
        }
        case 26: //escreva variavel
        {
            string instruction = "\tLD " + token->getLexeme() + "\n\tSTO $out_port";
            geraCodigo(instruction,".text");
            break;
        }
        case 99: //limpa comando
        {
            if(this->atribuindo) {
                string instruction = "";
                
                // if(this->temp_count > 0){
                //     int i = 2;
                //     instruction += "\tLD TEMP" + std::to_string(i) + "\n";
                //     i++;
                //     while(i <= this->temp_count) {
                //         instruction += "\tADD TEMP" + std::to_string(i) + "\n";
                //         i++;
                //     }
                // }
                
                instruction += "\tSTO " + this->simbolo_atual->nome;
                geraCodigo(instruction, ".text");
            } else if(this->atribuindo_vet) {
                string instruction = "";

                if(this->temp_count > 0){
                    int i = 1;
                    instruction += "\tLD TEMP" + std::to_string(i) + "\n";
                    i++;
                    //if(this->temp_count >2) {
                        while(i <= this->temp_count) {
                            instruction += this->part_instru->front() + " TEMP" + std::to_string(i) + "\n";
                            this->part_instru->pop_front();
                            i++;
                        }
                        instruction += "\tSTO TEMP" + this->indice_resultado + "\n";
                    //}
                    instruction += "\tLD INDICE\n";
                    instruction += "\tSTO $indr\n";
                    instruction += "\tLD TEMP" + this->indice_resultado + "\n";
                }
                
                instruction += "\tSTOV " + this->simbolo_atual->nome;
                geraCodigo(instruction, ".text");
            }


            this->simbolo_atual = NULL;
            this->simbolo_chamado = NULL;
            this->tipo_atual = "";
            this->lescopo_aberto = "";
            this->param_count = 0;


            this->atribuindo = false;
            this->prim_elem_exp = 0;

            this->vetor_chamado = "";
            this->vetorando = false;
            this->temp_count = 1;
            this->indice_resultado = "";

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

