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
    t_shfl = 25,
    t_shfr = 26,
    t_bwand = 27,
    t_bwor = 28,
    t_bwinv = 29,
    t_xor = 30,
    t_pontpont = 31,
    t_endl = 32,
    t_pont = 33,
    t_virg = 34,
    t_abre_par = 35,
    t_fecha_par = 36,
    t_abre_col = 37,
    t_fecha_col = 38,
    t_abre_cha = 39,
    t_fecha_cha = 40,
    t_void = 41,
    t_long = 42,
    t_short = 43,
    t_unsigned = 44,
    t_const = 45,
    t_int = 46,
    t_char = 47,
    t_string = 48,
    t_double = 49,
    t_float = 50,
    t_boolean = 51,
    t_true = 52,
    t_false = 53,
    t_lit_int = 54,
    t_lit_char = 55,
    t_lit_str = 56,
    t_lit_double = 57,
    t_lit_float = 58,
    t_lit_bin = 59,
    t_lit_hex = 60,
    t_func_id = 61,
    t_var_id = 62
};

const int STATES_COUNT = 88;

extern int SCANNER_TABLE[STATES_COUNT][256];

extern int TOKEN_STATE[STATES_COUNT];

extern const char *SCANNER_ERROR[STATES_COUNT];

const int FIRST_SEMANTIC_ACTION = 102;

const int SHIFT  = 0;
const int REDUCE = 1;
const int ACTION = 2;
const int ACCEPT = 3;
const int GO_TO  = 4;
const int ERROR  = 5;

extern const int PARSER_TABLE[170][102][2];

extern const int PRODUCTIONS[99][2];

extern const char *PARSER_ERROR[170];

#endif
