#ifndef INT_H
#define INT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "LexicalAnalyzer.h"
#include "OpCodes.h"

using namespace std;

enum symbol_type {SYMBOL, OPERAND, VARIABLE, NUMBER, TEMPORARY, OPERATOR};
enum value_type {NONE, INTEGER, DOUBLE};
enum order_type {LEFT_TO_RIGHT, RIGHT_TO_LEFT};

class Interpreter 
{
    public:
    Interpreter (char * filename, LexicalAnalyzer * LP);
    ~Interpreter ();
    void SaveDeclType (token_type T);
    void NewDeclaration (string L);
    void NumLitFound (string N);
    void VariableFound (string N);
    void OperatorFound(OpCode_type T);
    friend ostream & operator << (ostream & outs, const Interpreter & I);
    struct Symbol;
    struct Operand;
    struct Variable;
    struct Number;
    struct Temporary;
    struct Operator;
    void EvaluateQ();
    void StartQ();
    ofstream symbollist;
    private:
    Symbol* Eval(Operator *op, Operand *left, Operand *right);
    map <string, Variable *> variables;
    map <string, Number *> numbers;
    map <int, Temporary *> temporaries;
    vector <Operator *> operators;
    vector <vector<Symbol*> > qs;
    LexicalAnalyzer * lex;
    string file;
    bool error_found;
    value_type current_type;
    Symbol* postpp(Operator *op, Operand *left, Operand *right);
    Symbol* postmm(Operator *op, Operand *left, Operand *right);
    Symbol* prepp(Operator *op, Operand *left, Operand *right);
    Symbol* premm(Operator *op, Operand *left, Operand *right);
    Symbol* uplus(Operator *op, Operand *left, Operand *right);
    Symbol* uminus(Operator *op, Operand *left, Operand *right);
    Symbol* negation(Operator *op, Operand *left, Operand *right);
    Symbol* bitnot(Operator *op, Operand *left, Operand *right);
    Symbol* multiply(Operator *op, Operand *left, Operand *right);
    Symbol* divide(Operator *op, Operand *left, Operand *right); 
    Symbol* remainder(Operator *op, Operand *left, Operand *right);
    Symbol* bplus(Operator *op, Operand *left, Operand *right);
    Symbol* bminus(Operator *op, Operand *left, Operand *right);
    Symbol* shiftleft(Operator *op, Operand *left, Operand *right);
    Symbol* shiftright(Operator *op, Operand *left, Operand *right);
    Symbol* lessthan(Operator *op, Operand *left, Operand *right);
    Symbol* lessthaneq(Operator *op, Operand *left, Operand *right); 
    Symbol* greaterthan(Operator *op, Operand *left, Operand *right);
    Symbol* greaterthaneq(Operator *op, Operand *left, Operand *right);
    Symbol* equal(Operator *op, Operand *left, Operand *right);
    Symbol* notequal(Operator *op, Operand *left, Operand *right);
    Symbol* bitwiseand(Operator *op, Operand *left, Operand *right);
    Symbol* bitxor(Operator *op, Operand *left, Operand *right);
    Symbol* bitwiseor(Operator *op, Operand *left, Operand *right); 
    Symbol* logicaland(Operator *op, Operand *left, Operand *right);
    Symbol* logicalor(Operator *op, Operand *left, Operand *right);
    Symbol* assignment(Operator *op, Operand *left, Operand *right);
    Symbol* plusequal(Operator *op, Operand *left, Operand *right);
    Symbol* minusequal(Operator *op, Operand *left, Operand *right);
    Symbol* multequal(Operator *op, Operand *left, Operand *right); 
    Symbol* divequal(Operator *op, Operand *left, Operand *right);
    Symbol* remequal(Operator *op, Operand *left, Operand *right);
    Symbol* shiftleftequal(Operator *op, Operand *left, Operand *right);
    Symbol* shiftrightequal(Operator *op, Operand *left, Operand *right);
    Symbol* bitandequal(Operator *op, Operand *left, Operand *right); 
    Symbol* bitxorequal(Operator *op, Operand *left, Operand *right);
    Symbol* bitorequal(Operator *op, Operand *left, Operand *right);
};
    
#include "Symbol.h"
#endif
