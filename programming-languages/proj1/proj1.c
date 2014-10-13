#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lex.h"
#include "state_table.h"

bool load_next_line();
enum token_type id_token();
enum token_type numeric_token();
enum token_type error_token_non_consuming();
enum token_type error_token_consuming();
enum token_type token_non_consuming();
enum token_type token_consuming();
void tokenize();
void init_char_map();



enum token_states {ID_TOKENIZER = 2, NUMERIC_TOKENIZER = 6,
    ERROR_NON_CONSUMING = 7, ERROR_CONSUMING = 22, TOKENIZER_NON_CONSUMING = 9,
    TOKENIZER_CONSUMING = 10, WHITESPACE_CONSUMING = 23 };


char* token_names[] = {"ERROR", "IDENT", "INTTYPE", "DBLTYPE", "NUMLIT", "PLUS", "MINUS", "MULT",
        "DIV", "MOD", "EXP", "NOT", "AND", "OR", "XOR", "ASSIGN", "LT", "GT", "SHIFTL",
        "SHIFTR", "PLUSPLUS", "PLUSEQ", "MINUSMINUS", "MINUSEQ", "MULTEQ",
        "DIVEQ", "MODEQ", "EXPEQ", "NOTEQ", "LOGAND", "ANDEQ", "LOGOR", "OREQ",
        "XOREQ", "EQUALTO", "SHIFTLEQ", "LTE", "SHIFTREQ", "GTE", "TILDA", "RPAREN",
        "LPAREN", "SEMI", "QUEST", "COLON", "COMMA", "EOFT"};


enum char_types { CHAR_ALPHA, CHAR_DIGIT, CHAR_UNDERSCORE, CHAR_PLUS, CHAR_MINUS,
    CHAR_FORWARD_SLASH, CHAR_STAR, CHAR_PERCENT, CHAR_EQUAL, CHAR_LESS, CHAR_GREATER,
    CHAR_AMP, CHAR_UP, CHAR_BAR, CHAR_BANG, CHAR_TILDA, CHAR_LEFT_PAREN, CHAR_RIGHT_PAREN,
    CHAR_SEMI, CHAR_QUESTION, CHAR_COLON, CHAR_COMMA, CHAR_PERIOD, CHAR_INVALID, CHAR_WHITESPACE };


const int START_STATE = 0;
size_t BUFFER_SIZE = 1024;

int CHAR_MAP[256];

FILE *fp;
char *line;
int length;
int pos;
int line_count;
int state;
char *lexeme;
int lexeme_pos;
int error_count;



void init_lex(char *filename) {

    printf("Input file: %s\n", filename);

    fp = fopen(filename, "r");

    state = START_STATE;

    line_count = 0;
    pos = 0;
    error_count = 0;

    lexeme = malloc(BUFFER_SIZE);
    lexeme_pos = 0;

    init_char_map();
}


void end_lex() {
    fclose(fp);
    free(lexeme);
    printf("%d errors found in input file\n", error_count);

}


enum token_type get_token() {

    lexeme_pos = 0;

    if (pos >= length) {
        bool success = load_next_line();
        if (!success) return EOFT;
    }

    while (true) {

        switch (state) {
            case ID_TOKENIZER:
                return id_token();
            case NUMERIC_TOKENIZER:
                return numeric_token();
            case ERROR_NON_CONSUMING:
                return error_token_non_consuming();
            case ERROR_CONSUMING:
                return error_token_consuming();
            case TOKENIZER_NON_CONSUMING:
                return token_non_consuming();
            case TOKENIZER_CONSUMING:
                return token_consuming();
            default:
                break;
        }

        if (state == WHITESPACE_CONSUMING) pos++;
        else if (state != START_STATE) lexeme[lexeme_pos++] = line[pos++];

        if (pos >= length) {
            bool success = load_next_line();
            if (!success) return EOFT;
        }

        state = STATE_TABLE[state][ CHAR_MAP[ (int) line[pos]] ];

    }

}


char *get_lexeme() {
    return lexeme;
}


bool load_next_line() {

    pos = 0;
    length = getline(&line, &BUFFER_SIZE, fp); 
    bool success = length != -1;

    if (success) {
        printf("%4d: %s", ++line_count, line);
    }

    return success;
}


enum token_type id_token() {

    tokenize();

    if (strcmp(lexeme, "int") == 0) return INTTYPE;
    else if (strcmp(lexeme, "double") == 0) return DBLTYPE;
    else return IDENT;

}


enum token_type numeric_token() {

    tokenize();

    return NUMLIT;

}


enum token_type error_token_non_consuming() {

    tokenize();

    printf("Error at %d,%d: Invalid character found: %s\n", line_count, pos, lexeme);

    error_count++;

    return ERROR;
}


enum token_type error_token_consuming() {

    lexeme[lexeme_pos++] = line[pos++];
    tokenize();

    printf("Error at %d,%d: Invalid character found: %s\n", line_count, pos, lexeme);

    error_count++;

    return ERROR;
}


enum token_type token_non_consuming() {

    tokenize();

    if (strcmp(lexeme, "+") == 0) return PLUS;
    else if (strcmp(lexeme, "-") == 0) return MINUS;
    else if (strcmp(lexeme, "/") == 0) return DIV;
    else if (strcmp(lexeme, "*") == 0) return MULT;
    else if (strcmp(lexeme, "%") == 0) return MOD;
    else if (strcmp(lexeme, "=") == 0) return ASSIGN;
    else if (strcmp(lexeme, ">") == 0) return GT;
    else if (strcmp(lexeme, "<") == 0) return LT;
    else if (strcmp(lexeme, "!") == 0) return NOT;
    else if (strcmp(lexeme, "&") == 0) return AND;
    else if (strcmp(lexeme, "|") == 0) return OR;
    else if (strcmp(lexeme, "^") == 0) return XOR;
    else if (strcmp(lexeme, "<<") == 0) return SHIFTL;
    else if (strcmp(lexeme, ">>") == 0) return SHIFTR;
    else {
        printf("Unknown token in token_non_consuming(): |%s|\n", lexeme);
        return ERROR;
    }

}


enum token_type token_consuming() {

    lexeme[lexeme_pos++] = line[pos++];
    tokenize();

    if (strcmp(lexeme, "--") == 0) return MINUSMINUS;
    else if (strcmp(lexeme, "++") == 0) return PLUSPLUS;
    else if (strcmp(lexeme, "*=") == 0) return MULTEQ;
    else if (strcmp(lexeme, "/=") == 0) return DIVEQ;
    else if (strcmp(lexeme, "%=") == 0) return MODEQ;
    else if (strcmp(lexeme, "+=") == 0) return PLUSEQ;
    else if (strcmp(lexeme, "-=") == 0) return MINUSEQ;
    else if (strcmp(lexeme, "<<=") == 0) return SHIFTLEQ;
    else if (strcmp(lexeme, ">>=") == 0) return SHIFTREQ;
    else if (strcmp(lexeme, "&=") == 0) return ANDEQ;
    else if (strcmp(lexeme, "^=") == 0) return XOREQ;
    else if (strcmp(lexeme, "|=") == 0) return OREQ;
    else if (strcmp(lexeme, "==") == 0) return EQUALTO;
    else if (strcmp(lexeme, "!=") == 0) return NOTEQ;
    else if (strcmp(lexeme, ">=") == 0) return GTE;
    else if (strcmp(lexeme, "<=") == 0) return LTE;
    else if (strcmp(lexeme, "&&") == 0) return LOGAND;
    else if (strcmp(lexeme, "||") == 0) return LOGOR;
    else if (strcmp(lexeme, "~") == 0) return TILDA;
    else if (strcmp(lexeme, "(") == 0) return LPAREN;
    else if (strcmp(lexeme, ")") == 0) return RPAREN;
    else if (strcmp(lexeme, ";") == 0) return SEMI;
    else if (strcmp(lexeme, "?") == 0) return QUEST;
    else if (strcmp(lexeme, ":") == 0) return COLON;
    else if (strcmp(lexeme, ",") == 0) return COMMA;
    else {
        printf("Unknown token in token_consuming(): |%s|\n", lexeme);
        return ERROR;
    }
}


void tokenize() {
    lexeme[lexeme_pos] = '\0';
    state = START_STATE;
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


int main (int argc, char * argv[])
{
    enum token_type t;
    if (argc < 2)
    {
        printf ("format: proj1 <filename>\n");
        exit (1);
    }
    init_lex (argv[1]);
    while ((t = get_token()) != EOFT) {
        printf ("\t%-12s%s\n", token_names[t], get_lexeme());
    }
    end_lex();
    return 0;
}
