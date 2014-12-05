#include <sstream>
#include "Interpreter.h"

Interpreter::Operator::Operator (const string & N, OpCode_type C, int A, int P, order_type O, value_type L, value_type R) {
    name = N;
    opcode = C;
    arguments = A;
    priority = P;
    order = O;
    validLeft = L;
    validRight = R;
}

symbol_type Interpreter::Operator::IsA() const {
    return OPERATOR;
}

string Interpreter::Operator::ToString() const {
    return name;
}

Interpreter::Temporary::Temporary(int value) {
    std::stringstream ss;
    ss << value;
    name = ss.str();
    vType = INTEGER;
    iVal = value;
    dVal = value;
}

Interpreter::Temporary::Temporary(double value) {
    std::stringstream ss;
    ss << "$";
    ss << tempNum++;
    name = ss.str();
    vType = DOUBLE;
    iVal = value;
    dVal = value;
}

symbol_type Interpreter::Temporary::IsA() const {
    return TEMPORARY;
}

string Interpreter::Temporary::ToString() const {
    return name;
}

Interpreter::Number::Number(string N) {
    name = N;
    vType = N.find('.') != std::string::npos ? DOUBLE : INTEGER;
    std::stringstream ss;
    ss << N;
    ss >> dVal;
    iVal = dVal;
}

symbol_type Interpreter::Number::IsA() const {
    return NUMBER;
}

string Interpreter::Number::ToString() const {
    return name;
}

Interpreter::Variable::Variable(string N, value_type T) {
    name = N;
    vType = T;
}

symbol_type Interpreter::Variable::IsA() const {
    return VARIABLE;
}

string Interpreter::Variable::ToString() const {
    return name;
}
