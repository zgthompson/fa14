#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lex.h"
#include "state_table.h"

char* token_names[] = {"ERROR", "IDENT", "INTTYPE", "DBLTYPE", "NUMLIT", "PLUS", "MINUS", "MULT",
        "DIV", "MOD", "EXP", "NOT", "AND", "OR", "XOR", "ASSIGN", "LT", "GT", "SHIFTL",
        "SHIFTR", "PLUSPLUS", "PLUSEQ", "MINUSMINUS", "MINUSEQ", "MULTEQ",
        "DIVEQ", "MODEQ", "EXPEQ", "NOTEQ", "LOGAND", "ANDEQ", "LOGOR", "OREQ",
        "XOREQ", "EQUALTO", "SHIFTLEQ", "LTE", "SHIFTREQ", "GTE", "TILDA", "RPAREN",
        "LPAREN", "SEMI", "QUEST", "COLON", "COMMA", "EOFT"};

const int START_STATE = 0;
size_t BUFFER_SIZE = 1024;
int CHAR_MAP[256];


void init_lex(lex_state *ls, FILE *input, FILE *output, FILE *debug) {


    ls->input = input;
    ls->output = output;
    ls->debug = debug;

    ls->state = START_STATE;

    ls->line_count = 0;
    ls->pos = 0;
    ls->error_count = 0;

    ls->line = malloc(BUFFER_SIZE);
    ls->length = 0;

    ls->lexeme = malloc(BUFFER_SIZE);
    ls->lexeme_pos = 0;

    init_char_map();
}


void end_lex(lex_state *ls) {
    free(ls->lexeme);
    fprintf(ls->output, "%d lexical errors found in input file\n", ls->error_count);

}


enum token_type get_token(lex_state *ls) {

    ls->lexeme_pos = 0;

    if (ls->pos >= ls->length) {
        bool success = load_next_line(ls);
        if (!success) return EOFT;
    }

    while (true) {

        switch (ls->state) {
            case ID_TOKENIZER:
                return id_token(ls);
            case NUMERIC_TOKENIZER:
                return numeric_token(ls);
            case ERROR_NON_CONSUMING:
                return error_token_non_consuming(ls);
            case ERROR_CONSUMING:
                return error_token_consuming(ls);
            case TOKENIZER_NON_CONSUMING:
                return token_non_consuming(ls);
            case TOKENIZER_CONSUMING:
                return token_consuming(ls);
            default:
                break;
        }

        if (ls->state == WHITESPACE_CONSUMING) (ls->pos)++;
        else if (ls->state != START_STATE) (ls->lexeme)[(ls->lexeme_pos)++] = (ls->line)[(ls->pos)++];

        if (ls->pos >= ls->length) {
            bool success = load_next_line(ls);
            if (!success) return EOFT;
        }

        ls->state = STATE_TABLE[ls->state][ CHAR_MAP[ (int) (ls->line)[ls->pos]] ];

    }

}


char *get_lexeme(lex_state *ls) {
    return ls->lexeme;
}


bool load_next_line(lex_state *ls) {

    ls->pos = 0;
    ls->length = getline(&(ls->line), &BUFFER_SIZE, ls->input); 
    bool success = ls->length != -1;


    if (success) {
        fprintf(ls->output, "%4d: %s", ++(ls->line_count), ls->line);
    }

    return success;
}


enum token_type id_token(lex_state *ls) {

    tokenize(ls);

    enum token_type t;

    if (strcmp(ls->lexeme, "int") == 0) t = INTTYPE;
    else if (strcmp(ls->lexeme, "double") == 0) t = DBLTYPE;
    else t = IDENT;

    fprintf(ls->debug, "\t%-12s%s\n", token_names[t], get_lexeme(ls));
    return t;

}


enum token_type numeric_token(lex_state *ls) {

    tokenize(ls);

    enum token_type t;
    t = NUMLIT;
    fprintf(ls->debug, "\t%-12s%s\n", token_names[t], get_lexeme(ls));
    return t;

}


enum token_type error_token_non_consuming(lex_state *ls) {

    tokenize(ls);

    fprintf(ls->output, "Error at %d,%d: Invalid character found: %s\n", ls->line_count, ls->pos, ls->lexeme);

    ls->error_count++;

    enum token_type t;
    t = ERROR;
    fprintf(ls->debug, "\t%-12s%s\n", token_names[t], get_lexeme(ls));
    return t;
}


enum token_type error_token_consuming(lex_state *ls) {

    ls->lexeme[(ls->lexeme_pos)++] = ls->line[ls->pos++];
    tokenize(ls);

    fprintf(ls->output, "Error at %d,%d: Invalid character found: %s\n", ls->line_count, ls->pos, ls->lexeme);

    ls->error_count++;

    enum token_type t;
    t = ERROR;
    fprintf(ls->debug, "\t%-12s%s\n", token_names[t], get_lexeme(ls));
    return t;
}


enum token_type token_non_consuming(lex_state *ls) {

    tokenize(ls);

    enum token_type t;

    if (strcmp(ls->lexeme, "+") == 0) t = PLUS;
    else if (strcmp(ls->lexeme, "-") == 0) t = MINUS;
    else if (strcmp(ls->lexeme, "/") == 0) t = DIV;
    else if (strcmp(ls->lexeme, "*") == 0) t = MULT;
    else if (strcmp(ls->lexeme, "%") == 0) t = MOD;
    else if (strcmp(ls->lexeme, "=") == 0) t = ASSIGN;
    else if (strcmp(ls->lexeme, ">") == 0) t = GT;
    else if (strcmp(ls->lexeme, "<") == 0) t = LT;
    else if (strcmp(ls->lexeme, "!") == 0) t = NOT;
    else if (strcmp(ls->lexeme, "&") == 0) t = AND;
    else if (strcmp(ls->lexeme, "|") == 0) t = OR;
    else if (strcmp(ls->lexeme, "^") == 0) t = XOR;
    else if (strcmp(ls->lexeme, "<<") == 0) t = SHIFTL;
    else if (strcmp(ls->lexeme, ">>") == 0) t = SHIFTR;
    else {
        fprintf(ls->debug, "Unknown token in token_non_consuming(): |%s|\n", ls->lexeme);
        t = ERROR;
    }

    fprintf(ls->debug, "\t%-12s%s\n", token_names[t], get_lexeme(ls));
    return t;

}


enum token_type token_consuming(lex_state *ls) {

    ls->lexeme[(ls->lexeme_pos)++] = ls->line[(ls->pos)++];
    tokenize(ls);

    enum token_type t;

    if (strcmp(ls->lexeme, "--") == 0) t = MINUSMINUS;
    else if (strcmp(ls->lexeme, "++") == 0) t = PLUSPLUS;
    else if (strcmp(ls->lexeme, "*=") == 0) t = MULTEQ;
    else if (strcmp(ls->lexeme, "/=") == 0) t = DIVEQ;
    else if (strcmp(ls->lexeme, "%=") == 0) t = MODEQ;
    else if (strcmp(ls->lexeme, "+=") == 0) t = PLUSEQ;
    else if (strcmp(ls->lexeme, "-=") == 0) t = MINUSEQ;
    else if (strcmp(ls->lexeme, "<<=") == 0) t = SHIFTLEQ;
    else if (strcmp(ls->lexeme, ">>=") == 0) t = SHIFTREQ;
    else if (strcmp(ls->lexeme, "&=") == 0) t = ANDEQ;
    else if (strcmp(ls->lexeme, "^=") == 0) t = XOREQ;
    else if (strcmp(ls->lexeme, "|=") == 0) t = OREQ;
    else if (strcmp(ls->lexeme, "==") == 0) t = EQUALTO;
    else if (strcmp(ls->lexeme, "!=") == 0) t = NOTEQ;
    else if (strcmp(ls->lexeme, ">=") == 0) t = GTE;
    else if (strcmp(ls->lexeme, "<=") == 0) t = LTE;
    else if (strcmp(ls->lexeme, "&&") == 0) t = LOGAND;
    else if (strcmp(ls->lexeme, "||") == 0) t = LOGOR;
    else if (strcmp(ls->lexeme, "~") == 0) t = TILDA;
    else if (strcmp(ls->lexeme, "(") == 0) t = LPAREN;
    else if (strcmp(ls->lexeme, ")") == 0) t = RPAREN;
    else if (strcmp(ls->lexeme, ";") == 0) t = SEMI;
    else if (strcmp(ls->lexeme, "?") == 0) t = QUEST;
    else if (strcmp(ls->lexeme, ":") == 0) t = COLON;
    else if (strcmp(ls->lexeme, ",") == 0) t = COMMA;
    else {
        fprintf(ls->debug, "Unknown token in token_consuming(): |%s|\n", ls->lexeme);
        t = ERROR;
    }

    fprintf(ls->debug, "\t%-12s%s\n", token_names[t], get_lexeme(ls));
    return t;
}


void tokenize(lex_state *ls) {
    ls->lexeme[ls->lexeme_pos] = '\0';
    ls->state = START_STATE;
}


void init_char_map() {

    int i = 0;
    while (i < 256) CHAR_MAP[i++] = CHAR_INVALID;

    int ch = 'a';
    while (ch <= 'z') {
        CHAR_MAP[ch++] = CHAR_ALPHA;
    }
    ch = 'A';
    while (ch <= 'Z') {
        CHAR_MAP[ch++] = CHAR_ALPHA;
    }
    ch = '0';
    while (ch <= '9') {
        CHAR_MAP[ch++] = CHAR_DIGIT;
    }

    CHAR_MAP['_'] = CHAR_UNDERSCORE;
    CHAR_MAP['+'] = CHAR_PLUS;
    CHAR_MAP['-'] = CHAR_MINUS;
    CHAR_MAP['/'] = CHAR_FORWARD_SLASH;
    CHAR_MAP['*'] = CHAR_STAR;
    CHAR_MAP['%'] = CHAR_PERCENT;
    CHAR_MAP['='] = CHAR_EQUAL;
    CHAR_MAP['<'] = CHAR_LESS;
    CHAR_MAP['>'] = CHAR_GREATER;
    CHAR_MAP['&'] = CHAR_AMP;
    CHAR_MAP['^'] = CHAR_UP;
    CHAR_MAP['|'] = CHAR_BAR;
    CHAR_MAP['!'] = CHAR_BANG;
    CHAR_MAP['~'] = CHAR_TILDA;
    CHAR_MAP['('] = CHAR_LEFT_PAREN;
    CHAR_MAP[')'] = CHAR_RIGHT_PAREN;
    CHAR_MAP[';'] = CHAR_SEMI;
    CHAR_MAP['?'] = CHAR_QUESTION;
    CHAR_MAP[':'] = CHAR_COLON;
    CHAR_MAP[','] = CHAR_COMMA;
    CHAR_MAP['.'] = CHAR_PERIOD;
    CHAR_MAP[' '] = CHAR_WHITESPACE;
    CHAR_MAP['\n'] = CHAR_WHITESPACE;
    CHAR_MAP['\t'] = CHAR_WHITESPACE;
}


