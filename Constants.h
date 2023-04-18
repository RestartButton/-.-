#ifndef CONSTANTS_H
#define CONSTANTS_H

enum TokenId 
{
    EPSILON  = 0,
    DOLLAR   = 1,
    t_comentario = 2,
    t_comentario_multiplo = 3,
    t_if = 4,
    t_else = 5,
    t_while = 6,
    t_do = 7,
    t_return = 8,
    t_for = 9,
    t_soma = 10,
    t_mult = 11,
    t_sub = 12,
    t_div = 13,
    t_atrib = 14,
    t_mod = 15,
    t_maior_q = 16,
    t_menor_q = 17,
    t_maior_ig = 18,
    t_menor_ig = 19,
    t_igual_a = 20,
    t_difere = 21,
    t_and_log = 22,
    t_or_log = 23,
    t_not = 24,
    t_menos = 25,
    t_shfl = 26,
    t_shfr = 27,
    t_bwand = 28,
    t_bwor = 29,
    t_bwinv = 30,
    t_xor = 31,
    t_pontpont = 32,
    t_endl = 33,
    t_pont = 34,
    t_virg = 35,
    t_abre_par = 36,
    t_fecha_par = 37,
    t_abre_col = 38,
    t_fecha_col = 39,
    t_abre_cha = 40,
    t_fecha_cha = 41,
    t_void = 42,
    t_long = 43,
    t_short = 44,
    t_unsigned = 45,
    t_const = 46,
    t_int = 47,
    t_char = 48,
    t_string = 49,
    t_double = 50,
    t_float = 51,
    t_boolean = 52,
    t_true = 53,
    t_false = 54,
    t_lit_int = 55,
    t_lit_char = 56,
    t_lit_str = 57,
    t_lit_double = 58,
    t_lit_float = 59,
    t_lit_bin = 60,
    t_lit_hex = 61,
    t_func_id = 62,
    t_var_id = 63
};

const int STATES_COUNT = 88;

extern int SCANNER_TABLE[STATES_COUNT][256];

extern int TOKEN_STATE[STATES_COUNT];

extern const char *SCANNER_ERROR[STATES_COUNT];

const int FIRST_SEMANTIC_ACTION = 104;

const int SHIFT  = 0;
const int REDUCE = 1;
const int ACTION = 2;
const int ACCEPT = 3;
const int GO_TO  = 4;
const int ERROR  = 5;

extern const int PARSER_TABLE[172][104][2];

extern const int PRODUCTIONS[101][2];

extern const char *PARSER_ERROR[172];

#endif
