// need to handle left and right parens
// start/end queue
#include <iostream>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
    indent = 10;
    lex = new LexicalAnalyzer (filename);
    interp = new Interpreter(filename, lex);
    token = lex->get_token ();
    program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
    delete lex;
}

void SyntacticalAnalyzer::program()
{
/*
1. <program> -> <stmt> SEMI <more_stmts> EOFT
2. <program> -> <decl> SEMI <more_stmts> EOFT
*/
    lex->debug << string(2 * ++indent, ' ') << "Entering <program>\n";
    if (token == INTTYPE || token == DBLTYPE)
        decl ();
    else
        stmt ();
    if (token == SEMI)
        token = lex->get_token();
    else
    {
        lex->error_message ("Semi colon expected; " + lex->get_lexeme() + " found");
        while (token != SEMI && token != EOFT)
            token = lex->get_token();
    }
    more_stmts ();
    if (token != EOFT)
    {
        lex->error_message ("End of File expected; " + lex->get_lexeme() + " found");
        while (token != SEMI && token != EOFT)
            token = lex->get_token();
    }
    lex->debug << string(2 * indent--, ' ') << "Exiting <program>\n";
}

void SyntacticalAnalyzer::more_stmts()
{
/*
3. <more_stmts> -> <stmt> SEMI <more_stmts>
4. <more_stmts> -> <decl> SEMI <more_stmts>
5. <more_stmts> -> {}
*/
    lex->debug << string(2 * ++indent, ' ') << "Entering <more_stmts>\n";
    if (token != EOFT)
    {
        if (token == INTTYPE || token == DBLTYPE)
            decl ();
        else
            stmt ();
        if (token == SEMI)
            token = lex->get_token();
        else
        {
            lex->error_message ("Semi colon expected; " + lex->get_lexeme() + " found");
            while (token != SEMI && token != EOFT)
                token = lex->get_token();
        }
        more_stmts ();
    }
    lex->debug << string(2 * indent--, ' ') << "Exiting <more_stmts>\n";
}

void SyntacticalAnalyzer::stmt()
{
/*
6. <stmt> -> <term> <stmt_tail>
*/
    lex->debug << string(2 * ++indent, ' ') << "Entering <stmts>\n";
    term ();
    stmt_tail ();
    lex->debug << string(2 * indent--, ' ') << "Exiting <stmts>\n";
}

void SyntacticalAnalyzer::decl()
{
/*
7. <decl> -> <ntype> IDENT <decl_tail> <more_decls>
*/
    lex->debug << string(2 * ++indent, ' ') << "Entering <decl>\n";
    ntype ();
    if (token == IDENT)
    {
        interp->NewDeclaration(lex->get_lexeme());
        token = lex->get_token();
    }
    else
    {
        lex->error_message ("Identifier expected; " + lex->get_lexeme() + " found");
        while (token != SEMI && token != EOFT)
            token = lex->get_token();
    }
    decl_tail ();
    // more_decls ();
    lex->debug << string(2 * indent--, ' ') << "Exiting <decl>\n";
}

void SyntacticalAnalyzer::ntype()
{
/*
8. <ntype> -> INTTYPE
9. <ntype> -> DBLTYPE
*/
    lex->debug << string(2 * ++indent, ' ') << "Entering <ntype>\n";
    if (token == INTTYPE || token == DBLTYPE) {
        interp->SaveDeclType(token);
        token = lex->get_token();
    }
    lex->debug << string(2 * indent--, ' ') << "Exiting <ntype>\n";
}

void SyntacticalAnalyzer::decl_tail()
{
/*
10. <decl_tail> -> ASSIGN <stmt>
11. <decl_tail> -> {}
*/
    lex->debug << string(2 * ++indent, ' ') << "Entering <decl_tail>\n";
    if (token != SEMI)
    {
        if (token == ASSIGN) {
            interp->OperatorFound(ASSIGNMENT);
            token = lex->get_token();
        }
        else
        {
            lex->error_message ("= expected; " + lex->get_lexeme() + " found");
            while (token != SEMI && token != EOFT)
                token = lex->get_token();
        }
        stmt ();
    }
    lex->debug << string(2 * indent--, ' ') << "Exiting <decl_tail>\n";
}

void SyntacticalAnalyzer::more_decls()
{
/*
12. <more_decls> -> COMMA IDENT <decl_tail> <more_decls>
13. <more_decls> -> {}
*/
// Not implemented
}

void SyntacticalAnalyzer::term()
{
/*
14. <term> -> <pre> <var> <post>
15. <term> -> LPAREN <stmt> RPAREN
*/
    lex->debug << string(2 * ++indent, ' ') << "Entering <term>\n";
    if (token == LPAREN)
    {
        token = lex->get_token();
        stmt ();
        if (token == RPAREN)
            token = lex->get_token();
        else
        {
            lex->error_message (") expected; " + lex->get_lexeme() + " found");
            while (token != SEMI && token != EOFT)
                token = lex->get_token();
        }
    }
    else
    {
        pre ();
        var ();
        post ();
    }
    lex->debug << string(2 * indent--, ' ') << "Exiting <term>\n";
}

void SyntacticalAnalyzer::pre()
{
/*
16. <pre> -> <uoppre> <pre>
17. <pre> -> {}
*/

    lex->debug << string(2 * ++indent, ' ') << "Entering <pre>\n";

    if (token == PLUS || token == MINUS || token == NOT ||
        token == TILDA || token == PLUSPLUS || token == MINUSMINUS) 
    {
        if (token == PLUS) interp->OperatorFound(UPLUS);
        else if (token == MINUS) interp->OperatorFound(UMINUS);
        else if (token == NOT) interp->OperatorFound(NEGATION);
        else if (token == TILDA) interp->OperatorFound(BITNOT);
        else if (token == PLUSPLUS) interp->OperatorFound(PREPP);
        else if (token == MINUSMINUS) interp->OperatorFound(PREMM);

        uoppre ();
        pre ();

    }

    lex->debug << string(2 * indent--, ' ') << "Exiting <pre>\n";
}

void SyntacticalAnalyzer::uoppre()
{
/*
18. <uoppre> -> PLUS
19. <uoppre> -> MINUS
20. <uoppre> -> NOT
21. <uoppre> -> TILDA
22. <uoppre> -> PLUSPLUS
23. <uoppre> -> MINUSMINUS
*/
    lex->debug << string(2 * ++indent, ' ') << "Entering <uoppre>\n";
    if (token == PLUS || token == MINUS || token == NOT ||
        token == TILDA || token == PLUSPLUS || token == MINUSMINUS) 
    {
        token = lex->get_token();
    }
    else
    {
        lex->error_message ("+, -, !, ~, ++ or -- expected; " + lex->get_lexeme() + " found");
        while (token != IDENT && token != NUMLIT)
            token = lex->get_token();
    }
    lex->debug << string(2 * indent--, ' ') << "Exiting <uoppre>\n";
}

void SyntacticalAnalyzer::var()
{
/*
24. <var> -> IDENT
25. <var> -> NUMLIT
*/
    lex->debug << string(2 * ++indent, ' ') << "Entering <var>\n";
    if (token == IDENT)
    {
        interp->VariableFound(lex->get_lexeme());
        token = lex->get_token();
    }
    else if (token == NUMLIT)
    {
        interp->NumLitFound(lex->get_lexeme());
        token = lex->get_token();
    }
    else
    {
        lex->error_message ("Identifier or numeric literal expected; " + lex->get_lexeme() + " found");
        while (token != SEMI && token != EOFT)
            token = lex->get_token();
    }
    lex->debug << string(2 * indent--, ' ') << "Exiting <var>\n";
}

void SyntacticalAnalyzer::post()
{
/*
26. <post> -> PLUSPLUS
27. <post> -> MINUSMINUS
28. <post> -> {}
*/
    lex->debug << string(2 * ++indent, ' ') << "Entering <post>\n";
    if (token == PLUSPLUS || token == MINUSMINUS) {
        if (token == PLUSPLUS) interp->OperatorFound(POSTPP);
        else if (token == MINUSMINUS) interp->OperatorFound(POSTMM);
        token = lex->get_token();
    }
    lex->debug << string(2 * indent--, ' ') << "Exiting <post>\n";
}

void SyntacticalAnalyzer::stmt_tail()
{
/*
29. <stmt_tail> -> <binop> <stmt>
30. <stmt_tail> -> QUEST <stmt> COLON <stmt>
31. <stmt_tail> -> {}
*/
    lex->debug << string(2 * ++indent, ' ') << "Entering <stmt_tail>\n";
    if (token != RPAREN && token != SEMI && token != EOFT)
    {
        if (token == QUEST)
        {
        }
        else
        {
            binop ();
            stmt ();
        }
    }
    lex->debug << string(2 * indent--, ' ') << "Exiting <stmt_tail>\n";
}

void SyntacticalAnalyzer::binop()
{
/*
32. <binop> -> PLUS
33. <binop> -> MINUS
34. <binop> -> MULT
35. <binop> -> DIV
36. <binop> -> MOD
37. <binop> -> AND
38. <binop> -> OR
39. <binop> -> XOR
40. <binop> -> ASSIGN
41. <binop> -> LT
42. <binop> -> GT
43. <binop> -> SHIFTL
44. <binop> -> SHIFTR
45. <binop> -> PLUSEQ
46. <binop> -> MINUSEQ
47. <binop> -> MULTEQ
48. <binop> -> DIVEQ
49. <binop> -> MODEQ
50. <binop> -> NOTEQ
51. <binop> -> LOGAND
52. <binop> -> ANDEQ
53. <binop> -> LOGOR
54. <binop> -> OREQ
55. <binop> -> XOREQ
56. <binop> -> EQUALTO
57. <binop> -> SHIFTLEQ
58. <binop> -> LTE
59. <binop> -> SHIFTREQ
60. <binop> -> GTE
61. <binop> -> COMMA
62. <binop> -> EXP
63. <binop> -> EXPEQ
*/
    lex->debug << string(2 * ++indent, ' ') << "Entering <binop>\n";
    if (token == PLUS || token == MINUS || token == MULT ||
        token == DIV || token == MOD || token == AND ||
        token == OR || token == XOR || token == ASSIGN ||
        token == LT || token == GT || token == SHIFTL ||
        token == SHIFTR || token == PLUSEQ || token == MINUSEQ ||
        token == MULTEQ || token == DIVEQ || token == MODEQ ||
        token == NOTEQ || token == LOGAND || token == ANDEQ ||
        token == LOGOR || token == OREQ || token == XOREQ ||
        token == EQUALTO || token == SHIFTLEQ || token == LTE ||
        token == SHIFTREQ || token == GTE)
    {

        if (token == PLUS) interp->OperatorFound(BPLUS);
        else if (token == MINUS) interp->OperatorFound(BMINUS);
        else if (token == MULT) interp->OperatorFound(MULTIPLY);
        else if (token == DIV) interp->OperatorFound(DIVIDE);
        else if (token == MOD) interp->OperatorFound(REMAINDER);
        else if (token == AND) interp->OperatorFound(BITAND);
        else if (token == OR) interp->OperatorFound(BITOR);
        else if (token == XOR) interp->OperatorFound(BITXOR);
        else if (token == ASSIGN) interp->OperatorFound(ASSIGNMENT);
        else if (token == LT) interp->OperatorFound(LESSTHAN);
        else if (token == GT) interp->OperatorFound(GREATERTHAN);
        else if (token == SHIFTL) interp->OperatorFound(SHIFTLEFT);
        else if (token == SHIFTR) interp->OperatorFound(SHIFTRIGHT);
        else if (token == PLUSEQ) interp->OperatorFound(PLUSEQUAL);
        else if (token == MINUSEQ) interp->OperatorFound(MINUSEQUAL);
        else if (token == MULTEQ) interp->OperatorFound(MULTEQUAL);
        else if (token == DIVEQ) interp->OperatorFound(DIVEQUAL);
        else if (token == MODEQ) interp->OperatorFound(REMEQUAL);
        else if (token == NOTEQ) interp->OperatorFound(NOTEQUAL);
        else if (token == LOGAND) interp->OperatorFound(LOGICALAND);
        else if (token == ANDEQ) interp->OperatorFound(BITANDEQUAL);
        else if (token == LOGOR) interp->OperatorFound(LOGICALOR);
        else if (token == OREQ) interp->OperatorFound(BITOREQUAL);
        else if (token == XOREQ) interp->OperatorFound(BITXOREQUAL);
        else if (token == EQUALTO) interp->OperatorFound(EQUAL);
        else if (token == SHIFTLEQ) interp->OperatorFound(SHIFTLEFTEQUAL);
        else if (token == LTE) interp->OperatorFound(LESSTHANEQ);
        else if (token == SHIFTREQ) interp->OperatorFound(SHIFTRIGHTEQUAL);
        else if (token == GTE) interp->OperatorFound(GREATERTHANEQ);

        token = lex->get_token();
    }
    else
    {
        lex->error_message ("Binary operator expected; " + lex->get_lexeme() + " found");
        while (token != SEMI && token != EOFT)
            token = lex->get_token();
    }
    lex->debug << string(2 * indent--, ' ') << "Exiting <binop>\n";
}
