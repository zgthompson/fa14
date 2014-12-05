#include "Interpreter.h"

Interpreter::Interpreter(char *filename, LexicalAnalyzer *LP) {
}

void Interpreter::SaveDeclType(token_type T) {
    if (T == INTTYPE) current_type = INTEGER;
    else if (T == DBLTYPE) current_type = DOUBLE;
    else current_type = NONE;
}

void Interpreter::NewDeclaration(string L) {
    variables.push_back( new Variable(L, current_type) );
}

void Interpreter::NumLitFound(string N) {
}

void Interpreter::VariableFound(string N) {
}

void Interpreter::OperatorFound(OpCode_type T) {
}
