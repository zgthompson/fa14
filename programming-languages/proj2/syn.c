#include <stdlib.h>
#include <stdio.h>
#include "syn.h"

void init_syn(syn_state *ss, FILE *input, FILE *output, FILE *debug) {
    lex_state *ls = malloc( sizeof(lex_state) );
    init_lex(ls, input, output, debug);
    ss->ls = ls;
    ss->input = input;
    ss->output = output;
    ss->debug = debug;
    ss->error_count = 0;
}

void end_syn(syn_state *ss) {
    end_lex(ss->ls);
    free(ss->ls);
    fprintf(ss->output, "%d syntactic errors found in input file\n", ss->error_count);
}

int parse_syn(syn_state *ss) {
    next_token(ss);
    program(ss);
    return ss->error_count;
}

void next_token(syn_state *ss) {
    ss->cur_token = get_token(ss->ls);
}

void program(syn_state *ss) {

    switch (ss->cur_token) {
        case DBLTYPE: case INTTYPE: 
            decl(ss);
            break;
        case IDENT: case LPAREN: case MINUS: case MINUSMINUS: case NOT: 
        case NUMLIT: case PLUS: case PLUSPLUS: case TILDA:
            stmt(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected a statement or declaration");
            break;
    }

    if (ss->cur_token == SEMI) {
        next_token(ss);
    }
    else {
        ss->error_count++;
        print_error(ss, "expected a semicolon");
    }

    switch (ss->cur_token) {
        case DBLTYPE: case INTTYPE: case IDENT: case LPAREN: case MINUS: 
        case MINUSMINUS: case NOT: case NUMLIT: case PLUS: case PLUSPLUS: 
        case TILDA: case EOFT:
            more_stmts(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected a statement or declaration");
            break;
    }

    if (ss->cur_token != EOFT) {
        ss->error_count++;
        print_error(ss, "expected end of file");
    }
}

void stmt(syn_state *ss) {

    switch (ss->cur_token) {
        case IDENT: case LPAREN: case MINUS: case MINUSMINUS: case NOT: 
        case NUMLIT: case PLUS: case PLUSPLUS: case TILDA:
            term(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected a unary operator");
            break;
    }

    switch (ss->cur_token) {
        case AND: case ANDEQ: case ASSIGN: case COLON: case COMMA: case DIV: 
        case DIVEQ: case EQUALTO: case EXP: case EXPEQ: case GT: case GTE: 
        case LOGAND: case LOGOR: case LT: case LTE: case MINUS: case MINUSEQ: 
        case MOD: case MODEQ: case MULT: case MULTEQ: case NOTEQ: case OR: 
        case OREQ: case PLUS: case PLUSEQ: case QUEST: case RPAREN: case SEMI: 
        case SHIFTL: case SHIFTLEQ: case SHIFTR: case SHIFTREQ: case XOR: 
        case XOREQ:
            stmt_tail(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected a binary operator");
            break;
    }
}

void decl(syn_state *ss) {
    switch (ss->cur_token) {
        case DBLTYPE: case INTTYPE:
            ntype(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected a type declaration");
            break;
    }

    if (ss->cur_token == IDENT) {
        next_token(ss);
    }
    else {
        ss->error_count++;
        print_error(ss, "expected an identifier");
    }

    switch (ss->cur_token) {
        case ASSIGN: case COMMA: case SEMI:
            decl_tail(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected an assignment");
            break;
    }

    switch (ss->cur_token) {
        case COMMA: case SEMI:
            more_decls(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected a comma");
            break;
    }
}

void more_stmts(syn_state *ss) {


    switch (ss->cur_token) {
        case DBLTYPE: case INTTYPE:
            decl(ss);
            break;
        case IDENT: case LPAREN: case MINUS: case MINUSMINUS: 
        case NOT: case NUMLIT: case PLUS: case PLUSPLUS: case TILDA:
            stmt(ss);
            break;
        case EOFT:
            return;
        default:
            ss->error_count++;
            print_error(ss, "expected a statement or declaration");
            break;
    }

    if (ss->cur_token != SEMI) {
        ss->error_count++;
        print_error(ss, "expected a semicolon");
        while (ss->cur_token != SEMI) {
            next_token(ss);
            if (ss->cur_token == EOFT) return;
        }
    }

    next_token(ss);

    switch (ss->cur_token) {
        case DBLTYPE: case EOFT: case IDENT: case INTTYPE: case LPAREN: 
        case MINUS: case MINUSMINUS: case NOT: case NUMLIT: case PLUS: 
        case PLUSPLUS: case TILDA:
            more_stmts(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected a statement or declaration");
            break;
    }
}

void term(syn_state *ss) {
    if (ss->cur_token == LPAREN) {
        next_token(ss);
        stmt(ss);
        if (ss->cur_token != RPAREN) {
            ss->error_count++;
            print_error(ss, "expected a right parenthesis");
        }
        else {
            next_token(ss);
        }
    }

    else {

        switch (ss->cur_token) {
            case IDENT: case MINUS: case MINUSMINUS: case NOT: case NUMLIT: 
            case PLUS: case PLUSPLUS: case TILDA:
                pre(ss);
                break;
            default:
                ss->error_count++;
                print_error(ss, "expected a unary operator");
                break;
        }

        switch (ss->cur_token) {
            case IDENT: case NUMLIT:
                var(ss);
                break;
            default:
                ss->error_count++;
                print_error(ss, "expected an identifier or numerical literal");
                break;
        }

        switch (ss->cur_token) {
            case AND: case ANDEQ: case ASSIGN: case COLON: case COMMA: case DIV: 
            case DIVEQ: case EQUALTO: case EXP: case EXPEQ: case GT: case GTE: 
            case LOGAND: case LOGOR: case LT: case LTE: case MINUS: case MINUSEQ: 
            case MINUSMINUS: case MOD: case MODEQ: case MULT: case MULTEQ: 
            case NOTEQ: case OR: case OREQ: case PLUS: case PLUSEQ: case PLUSPLUS: 
            case QUEST: case RPAREN: case SEMI: case SHIFTL: case SHIFTLEQ: 
            case SHIFTR: case SHIFTREQ: case XOR: case XOREQ:
                post(ss);
                break;
            default:
                ss->error_count++;
                print_error(ss, "expected a postfix operator");
                break;
        }
    }
}

void stmt_tail(syn_state *ss) {
    if (ss->cur_token == QUEST) {
        next_token(ss);
        
        switch (ss->cur_token) {
            case IDENT: case LPAREN: case MINUS: case MINUSMINUS: case NOT: 
            case NUMLIT: case PLUS: case PLUSPLUS: case TILDA:
                stmt(ss);
                break;
            default:
                ss->error_count++;
                print_error(ss, "expected a statement");
                break;
        }

        if (ss->cur_token == COLON) {
            next_token(ss);
        }
        else {
            ss->error_count++;
            print_error(ss, "expected a colon");
        }

        switch (ss->cur_token) {
            case IDENT: case LPAREN: case MINUS: case MINUSMINUS: case NOT: 
            case NUMLIT: case PLUS: case PLUSPLUS: case TILDA:
                stmt(ss);
                break;
            default:
                ss->error_count++;
                print_error(ss, "expected a statement");
                break;
        }

    }

    else {
        
        switch (ss->cur_token) {
            case AND: case ANDEQ: case ASSIGN: case COMMA: case DIV: case DIVEQ: 
            case EQUALTO: case EXP: case EXPEQ: case GT: case GTE: case LOGAND: 
            case LOGOR: case LT: case LTE: case MINUS: case MINUSEQ: case MOD: 
            case MODEQ: case MULT: case MULTEQ: case NOTEQ: case OR: case OREQ: 
            case PLUS: case PLUSEQ: case SHIFTL: case SHIFTLEQ: case SHIFTR: 
            case SHIFTREQ: case XOR: case XOREQ:
                binop(ss);
                break;
            case COLON: case RPAREN: case SEMI:
                return;
            default:
                ss->error_count++;
                print_error(ss, "expected a binary operator");
                break;
        }

        switch (ss->cur_token) {
            case IDENT: case LPAREN: case MINUS: case MINUSMINUS: case NOT: 
            case NUMLIT: case PLUS: case PLUSPLUS: case TILDA:
                stmt(ss);
                break;
            default:
                ss->error_count++;
                print_error(ss, "expected a statement");
                break;
        }
    }
}

void ntype(syn_state *ss) {
    switch (ss->cur_token) {
        case INTTYPE: case DBLTYPE:
            next_token(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected a type declaration");
            break;
    }
}

void decl_tail(syn_state *ss) {
    if (ss->cur_token == ASSIGN) {
        next_token(ss);
        switch (ss->cur_token) {
            case IDENT: case LPAREN: case MINUS: case MINUSMINUS: case NOT: 
            case NUMLIT: case PLUS: case PLUSPLUS: case TILDA:
                stmt(ss);
                break;
            default:
                ss->error_count++;
                print_error(ss, "expected a statement");
                break;
        }
    }
    else {
        switch (ss->cur_token) {
            case COMMA: case SEMI:
                return;
            default:
                ss->error_count++;
                print_error(ss, "expected a comma or semicolon");
                break;
        }
    }
}

void more_decls(syn_state *ss) {
    if (ss->cur_token == COMMA) {
        next_token(ss);
        
        if (ss->cur_token == IDENT) {
            next_token(ss);
        }
        else {
            ss->error_count++;
            print_error(ss, "expected an identifier");
        }

        switch (ss->cur_token) {
            case ASSIGN: case COMMA: case SEMI:
                decl_tail(ss);
                break;
            default:
                ss->error_count++;
                print_error(ss, "expected an assignment");
                break;
        }

        switch (ss->cur_token) {
            case COMMA: case SEMI:
                more_decls(ss);
                break;
            default:
                ss->error_count++;
                print_error(ss, "expected a comma or semicolon");
                break;
        }
    }
    else {
        if (ss->cur_token == SEMI) {
            return;
        }
        else {
            ss->error_count++;
            print_error(ss, "expected a semicolon");
        }
    }
}

void pre(syn_state *ss) {

    if (ss->cur_token == IDENT || ss->cur_token == NUMLIT) return;

    switch (ss->cur_token) {
        case MINUS: case MINUSMINUS: case NOT: case PLUS: case PLUSPLUS: 
        case TILDA:
            uoppre(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected a unary operator");
            break;
    }

    switch (ss->cur_token) {
        case IDENT: case NUMLIT: case MINUS: case MINUSMINUS: case NOT:
        case PLUS: case PLUSPLUS:
            pre(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected an identifier, numeric literal, or unary operator");
            break;
    }
}

void var(syn_state *ss) {
    switch (ss->cur_token) {
        case IDENT: case NUMLIT:
            next_token(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected an identifier or numeric literal");
            break;
    }
}

void post(syn_state *ss) {
    switch (ss->cur_token) {
        case PLUSPLUS: case MINUSMINUS:
            next_token(ss);
            break;
        case AND: case ANDEQ: case ASSIGN: case COLON: case COMMA: case DIV: 
        case DIVEQ: case EQUALTO: case EXP: case EXPEQ: case GT: case GTE: 
        case LOGAND: case LOGOR: case LT: case LTE: case MINUS: case MINUSEQ: 
        case MOD: case MODEQ: case MULT: case MULTEQ: case NOTEQ: case OR: 
        case OREQ: case PLUS: case PLUSEQ: case QUEST: case RPAREN: case SEMI: 
        case SHIFTL: case SHIFTLEQ: case SHIFTR: case SHIFTREQ: case XOR: 
        case XOREQ:
            return;
        default:
            ss->error_count++;
            print_error(ss, "expected a postfix operator");
            break;
    }
}

void binop(syn_state *ss) {
    switch (ss->cur_token) {
        case AND: case ANDEQ: case ASSIGN: case COMMA: case DIV: case DIVEQ: 
        case EQUALTO: case EXP: case EXPEQ: case GT: case GTE: case LOGAND: 
        case LOGOR: case LT: case LTE: case MINUS: case MINUSEQ: case MOD: 
        case MODEQ: case MULT: case MULTEQ: case NOTEQ: case OR: case OREQ: 
        case PLUS: case PLUSEQ: case SHIFTL: case SHIFTLEQ: case SHIFTR: 
        case SHIFTREQ: case XOR: case XOREQ:
            next_token(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected a binary operator");
            break;
    }
}

void uoppre(syn_state *ss) {
    switch (ss->cur_token) {
        case MINUS: case MINUSMINUS: case NOT: case PLUS: case PLUSPLUS: 
        case TILDA:
            next_token(ss);
            break;
        default:
            ss->error_count++;
            print_error(ss, "expected a unary operator");
            break;
    }
}


void print_error(syn_state *ss, char *error_message) {
    lex_state *ls = ss->ls;
    fprintf(ss->output, "Error at %d,%d: %s; %s found\n", ls->line_count, ls->pos, error_message, ls->lexeme);
}
