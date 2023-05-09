#include <iostream>
#include <string.h>
#include <list>

using namespace std;

struct Simbolo {
    string nome;
    string tipo;
    string escopo;
    bool func;
    bool ref;
    bool matrix;
    bool vet;
    bool param;
    bool usada;
    bool inic;
    int pos;
};

void declaraSimbolo( list<Simbolo> &tabela, string nome, string tipo, string escopo, bool func, bool ref, bool matrix, bool vet, bool param, bool usada, bool inic, int pos );

bool isInit( list<Simbolo> tabela, string var, string escopo );
bool isUsad( list<Simbolo> tabela, string var, string escopo );
bool isFunc( list<Simbolo> tabela, string fun, string escopo );
bool isParam( list<Simbolo> tabela, string var, string escopo );
bool isVet( list<Simbolo> tabela, string fun, string escopo );
bool isMatrix( list<Simbolo> tabela, string var, string escopo );
bool isDec( list<Simbolo> tabela, string var, string escopo );

string getTipo();
int getParamPos();
