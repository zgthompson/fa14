#include "lex.h"

typedef struct {
    lex_state *ls;
    enum token_type cur_token;
    int error_count;
    FILE *input;
    FILE *output;
    FILE *debug;
} syn_state;

void init_syn(syn_state *ss, FILE *input, FILE *output, FILE *debug);
void end_syn(syn_state *ss);
int parse_syn(syn_state *ss);
void next_token(syn_state *ss);
void program(syn_state *ss);
void stmt(syn_state *ss);
void decl(syn_state *ss);
void more_stmts(syn_state *ss);
void term(syn_state *ss);
void stmt_tail(syn_state *ss);
void ntype(syn_state *ss);
void decl_tail(syn_state *ss);
void more_decls(syn_state *ss);
void pre(syn_state *ss);
void var(syn_state *ss);
void post(syn_state *ss);
void binop(syn_state *ss);
void uoppre(syn_state *ss);
void print_error(syn_state *ss, char *error_message);
