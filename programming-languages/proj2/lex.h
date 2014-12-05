#include <stdio.h>
#include <stdbool.h>

typedef struct {
    FILE *input;
    FILE *output;
    FILE *debug;
    char *line;
    int length;
    int pos;
    int line_count;
    int state;
    char *lexeme;
    int lexeme_pos;
    int error_count;
} lex_state;


enum token_type {ERROR, IDENT, INTTYPE, DBLTYPE, NUMLIT, PLUS, MINUS, MULT,
        DIV, MOD, EXP, NOT, AND, OR, XOR, ASSIGN, LT, GT, SHIFTL,
        SHIFTR, PLUSPLUS, PLUSEQ, MINUSMINUS, MINUSEQ, MULTEQ,
        DIVEQ, MODEQ, EXPEQ, NOTEQ, LOGAND, ANDEQ, LOGOR, OREQ,
        XOREQ, EQUALTO, SHIFTLEQ, LTE, SHIFTREQ, GTE, TILDA, RPAREN,
        LPAREN, SEMI, QUEST, COLON, COMMA, EOFT};


enum token_states {ID_TOKENIZER = 2, NUMERIC_TOKENIZER = 6,
    ERROR_NON_CONSUMING = 7, ERROR_CONSUMING = 22, TOKENIZER_NON_CONSUMING = 9,
    TOKENIZER_CONSUMING = 10, WHITESPACE_CONSUMING = 23 };


enum char_types { CHAR_ALPHA, CHAR_DIGIT, CHAR_UNDERSCORE, CHAR_PLUS, CHAR_MINUS,
    CHAR_FORWARD_SLASH, CHAR_STAR, CHAR_PERCENT, CHAR_EQUAL, CHAR_LESS, CHAR_GREATER,
    CHAR_AMP, CHAR_UP, CHAR_BAR, CHAR_BANG, CHAR_TILDA, CHAR_LEFT_PAREN, CHAR_RIGHT_PAREN,
    CHAR_SEMI, CHAR_QUESTION, CHAR_COLON, CHAR_COMMA, CHAR_PERIOD, CHAR_INVALID, CHAR_WHITESPACE };


extern char * token_names [];
extern const int START_STATE;
extern size_t BUFFER_SIZE;
extern int CHAR_MAP[256];


void init_lex (lex_state *ls, FILE *input, FILE *output, FILE *debug);
enum token_type get_token (lex_state *ls);
char * get_lexeme (lex_state *ls);
void error_message (char * msg);
void end_lex (lex_state *ls);
bool load_next_line(lex_state *ls);
enum token_type id_token(lex_state *ls);
enum token_type numeric_token(lex_state *ls);
enum token_type error_token_non_consuming(lex_state *ls);
enum token_type error_token_consuming(lex_state *ls);
enum token_type token_non_consuming(lex_state *ls);
enum token_type token_consuming(lex_state *ls);
void tokenize(lex_state *ls);
void init_char_map();




