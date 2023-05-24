#include "Tabela.h"


using namespace std;

void declaraSimbolo( list<Simbolo> &tabela, string nome, string tipo, string escopo, bool func, bool ref, bool matrix, bool vet, bool param, bool usada, bool inic, int pos ) {
    Simbolo novo;
    novo.nome = nome;
    novo.tipo = tipo;
    novo.escopo = escopo;
    novo.func = func;
    novo.ref = ref;
    novo.matrix = matrix;
    novo.vet = vet;
    novo.param = param;
    novo.usada = usada;
    novo.inic = inic;
    novo.pos = pos;

    tabela.push_back(novo);
}

bool isInit( list<Simbolo> tabela, string var, string escopo ) {
    for( list<Simbolo>::iterator it = tabela.begin(); it != tabela.end(); it++) {
        if(it->nome == var && it->escopo == escopo) {
            return it->inic;
        }
    }
    return false;
}

bool isUsad( list<Simbolo> tabela, string var, string escopo ) {
    for( list<Simbolo>::iterator it = tabela.begin(); it != tabela.end(); it++) {
        if(it->nome == var && it->escopo == escopo) {
            return it->usada;
        }
    }
    return false;
}

bool isFunc( list<Simbolo> tabela, string fun, string escopo ) {
    for( list<Simbolo>::iterator it = tabela.begin(); it != tabela.end(); it++) {
        if(it->nome == fun && it->escopo == escopo) {
            return it->func;
        }
    }
    return false;
}

bool isParam( list<Simbolo> tabela, string par, string escopo ) {
    for( list<Simbolo>::iterator it = tabela.begin(); it != tabela.end(); it++) {
        if(it->nome == par && it->escopo == escopo) {
            return it->param;
        }
    }
    return false;
}

bool isVet( list<Simbolo> tabela, string vet, string escopo ) {
    for( list<Simbolo>::iterator it = tabela.begin(); it != tabela.end(); it++) {
        if(it->nome == vet && it->escopo == escopo) {
            return it->vet;
        }
    }
    return false;
}

bool isMatrix( list<Simbolo> tabela, string var, string escopo ) {
    for( list<Simbolo>::iterator it = tabela.begin(); it != tabela.end(); it++) {
        if(it->nome == var && it->escopo == escopo) {
            return it->matrix;
        }
    }
    return false;
}

bool isDec( list<Simbolo> tabela, string nom, string escopo ) {
    for( list<Simbolo>::iterator it = tabela.begin(); it != tabela.end(); it++) {
        if(it->nome == nom && it->escopo == escopo) {
            return true;
        }
    }
    return false;
}


string getTipo( list<Simbolo> tabela, string var, string escopo ) {
    for( list<Simbolo>::iterator it = tabela.begin(); it != tabela.end(); it++) {
        if(it->nome == var && it->escopo == escopo) {
            return it->tipo;
        }
    }
    return "";
}


int getParamPos( list<Simbolo> tabela, string var, string escopo ) {
    for( list<Simbolo>::iterator it = tabela.begin(); it != tabela.end(); it++) {
        if(it->nome == var && it->escopo == escopo && it->param) {
            return it->pos;
        }
    }
    return -1;
}

Simbolo* getSimbolo( list<Simbolo> &tabela, string var, string escopo ) {
    for( list<Simbolo>::iterator it = tabela.begin(); it != tabela.end(); it++) {
        if(it->nome == var && it->escopo == escopo) {
            return &*it;
        }
    }
    return NULL;
}
