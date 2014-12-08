#include "Interpreter.h"
#include <stack>
#include <sstream>
#include <iomanip>

ostream & operator << (ostream & outs, const Interpreter & I) {
    outs << "Input file: " << I.file << endl << endl;

    outs << "Symbol Table Variables:" << endl;
    for (map<string, Interpreter::Variable*>::const_iterator it = I.variables.begin(); it != I.variables.end(); ++it) {
        outs << it->second->ToString();
    }
    outs << endl;

    outs << "Symbol Table Numbers:" << endl;
    for (map<string, Interpreter::Number*>::const_iterator it = I.numbers.begin(); it != I.numbers.end(); ++it) {
        outs << it->second->ToString();
    }
    outs << endl;

    outs << "Symbol Table Temporaries:" << endl;
    for (map<int, Interpreter::Temporary*>::const_iterator it = I.temporaries.begin(); it != I.temporaries.end(); ++it) {
        outs << it->second->ToString();
    }
    outs << endl;

    outs << "Symbol Table Operators:" << endl;
    for (vector<Interpreter::Operator*>::const_iterator it = I.operators.begin(); it != I.operators.end(); ++it) {
        outs << (*it)->ToString();
    }
    outs << endl;
}

Interpreter::Interpreter(char *filename, LexicalAnalyzer *LP) {

    error_found = false;
    file = filename;

    string symbolname = file.substr (0, file.length()-3) + ".sym"; 
    symbollist.open(symbolname.c_str());

    operators.push_back( new Operator("++",      POSTPP,           1,       1,     LEFT_TO_RIGHT,  INTEGER,   NONE) );      
    operators.push_back( new Operator("--",      POSTMM,           1,       1,     LEFT_TO_RIGHT,  INTEGER,   NONE) );      
    operators.push_back( new Operator("++",      PREPP,            1,       2,     RIGHT_TO_LEFT,  INTEGER,   NONE) );      
    operators.push_back( new Operator("--",      PREMM,            1,       2,     RIGHT_TO_LEFT,  INTEGER,   NONE) );      
    operators.push_back( new Operator("+",       UPLUS,            1,       2,     RIGHT_TO_LEFT,  DOUBLE,    NONE) );      
    operators.push_back( new Operator("-",       UMINUS,           1,       2,     RIGHT_TO_LEFT,  DOUBLE,    NONE) );      
    operators.push_back( new Operator("!",       NEGATION,         1,       2,     RIGHT_TO_LEFT,  DOUBLE,    NONE) );      
    operators.push_back( new Operator("~",       BITNOT,           1,       2,     RIGHT_TO_LEFT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("*",       MULTIPLY,         2,       3,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("/",       DIVIDE,           2,       3,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("%",       REMAINDER,        2,       3,     LEFT_TO_RIGHT,  INTEGER,   INTEGER) );   
    operators.push_back( new Operator("+",       BPLUS,            2,       4,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("-",       BMINUS,           2,       4,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("<<",      SHIFTLEFT,        2,       5,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator(">>",      SHIFTRIGHT,       2,       5,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("<",       LESSTHAN,         2,       6,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("<=",      LESSTHANEQ,       2,       6,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator(">",       GREATERTHAN,      2,       6,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator(">=",      GREATERTHANEQ,    2,       6,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("==",      EQUAL,            2,       7,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("!=",      NOTEQUAL,         2,       7,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("&",       BITAND,           2,       8,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("^",       BITXOR,           2,       9,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("|",       BITOR,            2,      10,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("&&",      LOGICALAND,       2,      11,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("||",      LOGICALOR,        2,      12,     LEFT_TO_RIGHT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("=",       ASSIGNMENT,       2,      14,     RIGHT_TO_LEFT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("+=",      PLUSEQUAL,        2,      14,     RIGHT_TO_LEFT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("-=",      MINUSEQUAL,       2,      14,     RIGHT_TO_LEFT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("*=",      MULTEQUAL,        2,      14,     RIGHT_TO_LEFT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("/=",      DIVEQUAL,         2,      14,     RIGHT_TO_LEFT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("%=",      REMEQUAL,         2,      14,     RIGHT_TO_LEFT,  INTEGER,   INTEGER) );   
    operators.push_back( new Operator("<<=",     SHIFTLEFTEQUAL,   2,      14,     RIGHT_TO_LEFT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator(">>=",     SHIFTRIGHTEQUAL,  2,      14,     RIGHT_TO_LEFT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("&=",      BITANDEQUAL,      2,      14,     RIGHT_TO_LEFT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("^=",      BITXOREQUAL,      2,      14,     RIGHT_TO_LEFT,  DOUBLE,    DOUBLE) );    
    operators.push_back( new Operator("|=",      BITOREQUAL,       2,      14,     RIGHT_TO_LEFT,  DOUBLE,    DOUBLE) );    
}

void Interpreter::SaveDeclType(token_type T) {
    if (error_found) return;
    if (T == INTTYPE) current_type = INTEGER;
    else if (T == DBLTYPE) current_type = DOUBLE;
    else {
        cout << "Unknown type" << endl;
        error_found = true;
        current_type = NONE;
    }
}

void Interpreter::NewDeclaration(string L) {
    if (error_found) return;
    variables[L] =  new Variable(L, current_type);
    qs.back().push_back( variables[L] );
}

void Interpreter::NumLitFound(string N) {
    if (error_found) return;
    map<string,Number *>::iterator it = numbers.find(N);
    if (it == numbers.end()) {
        numbers[N] = new Number(N);
    }

    qs.back().push_back( numbers[N] );
}

void Interpreter::VariableFound(string N) {
    if (error_found) return;
    map<string, Variable *>::iterator it = variables.find(N);
    if (it == variables.end()) {
        cout << "Undeclared variable" << endl;
        error_found = true;
    }

    qs.back().push_back( variables[N] );
}

void Interpreter::OperatorFound(OpCode_type T) {
    if (error_found) return;
    qs.back().push_back( operators[T] );
}

void Interpreter::StartQ() {
    if (error_found) return;
    qs.push_back( vector<Symbol*>() );
}

void Interpreter::EvaluateQ() {
    if (error_found) return;

    /*
    for (vector<Symbol*>::iterator it = qs.back().begin(); it != qs.back().end(); ++it) {
        cout << (*it)->ToString() << " ";
    }
    cout << endl;
    */

    stack<Operator*> opStack;
    stack<Symbol*> varStack;
    bool rightToLeftUnary = false;

    for (vector<Symbol*>::iterator it = qs.back().begin(); it != qs.back().end(); ++it) {
        if ((*it)->IsA() != OPERATOR) {

            varStack.push(*it);

            if (rightToLeftUnary) {

                while (!opStack.empty() && opStack.top()->order == RIGHT_TO_LEFT && opStack.top()->arguments == 1) {
                    Operand *left = (Operand*) varStack.top();
                    varStack.pop();
                    Symbol *result = Eval(opStack.top(), left, NULL);
                    opStack.pop();
                    varStack.push(result);

                }

                rightToLeftUnary = false;
            }
        }
        else {
            Operator *curOp = (Operator*) *it, *topOp = !opStack.empty() ? opStack.top() : NULL;
            if (curOp->order == RIGHT_TO_LEFT && curOp->arguments == 1) {
                opStack.push(curOp);
                rightToLeftUnary = true;
            }
            else {
                while (topOp != NULL && ((topOp->priority == curOp->priority && topOp->order == LEFT_TO_RIGHT) || topOp->priority < curOp->priority)) {
                    Operand *left = NULL, *right = NULL;
                    left = (Operand*) varStack.top();
                    varStack.pop();

                    if (topOp->arguments == 2) {
                        right = left;
                        left = (Operand*) varStack.top();
                        varStack.pop();
                    }
                    Symbol *result = Eval(topOp, left, right);
                    varStack.push(result);

                    opStack.pop();
                    topOp = !opStack.empty() ? opStack.top() : NULL;
                }
                opStack.push(curOp);
            }
        }
    }

    while (!opStack.empty()) {
        Operator *topOp = opStack.top();
        Operand *left = NULL, *right = NULL;
        left = (Operand*) varStack.top();
        varStack.pop();

        if (topOp->arguments == 2) {
            right = left;
            left = (Operand*) varStack.top();
            varStack.pop();
        }
        Symbol *result = Eval(topOp, left, right);
        varStack.push(result);

        opStack.pop();
    }

    qs.pop_back();
    if (!qs.empty()) {
        qs.back().push_back(varStack.top());
    }
}

Interpreter::Symbol* Interpreter::Eval(Operator *op, Operand *left, Operand *right) {
    switch (op->opcode) {
        case POSTPP:
            return postpp(op, left, right);
        case POSTMM:
            return postmm(op, left, right);
        case PREPP:
            return prepp(op, left, right);
        case PREMM:
            return premm(op, left, right);
        case UPLUS:
            return uplus(op, left, right);
        case UMINUS:
            return uminus(op, left, right);
        case NEGATION:
            return negation(op, left, right);
        case BITNOT:
            return bitnot(op, left, right);
        case MULTIPLY:
            return multiply(op, left, right);
        case DIVIDE:
            return divide(op, left, right);
        case REMAINDER:
            return remainder(op, left, right);
        case BPLUS:
            return bplus(op, left, right);
        case BMINUS:
            return bminus(op, left, right);
        case SHIFTLEFT:
            return shiftleft(op, left, right);
        case SHIFTRIGHT:
            return shiftright(op, left, right);
        case LESSTHAN:
            return lessthan(op, left, right);
        case LESSTHANEQ:
            return lessthaneq(op, left, right);
        case GREATERTHAN:
            return greaterthan(op, left, right);
        case GREATERTHANEQ:
            return greaterthaneq(op, left, right);
        case EQUAL:
            return equal(op, left, right);
        case NOTEQUAL:
            return notequal(op, left, right);
        case BITAND:
            return bitwiseand(op, left, right);
        case BITXOR:
            return bitxor(op, left, right);
        case BITOR:
            return bitwiseor(op, left, right);
        case LOGICALAND:
            return logicaland(op, left, right);
        case LOGICALOR:
            return logicalor(op, left, right);
        case ASSIGNMENT:
            return assignment(op, left, right);
        case PLUSEQUAL:
            return plusequal(op, left, right);
        case MINUSEQUAL:
            return minusequal(op, left, right);
        case MULTEQUAL:
            return multequal(op, left, right);
        case DIVEQUAL:
            return divequal(op, left, right);
        case REMEQUAL:
            return remequal(op, left, right);
        case SHIFTLEFTEQUAL:
            return shiftleftequal(op, left, right);
        case SHIFTRIGHTEQUAL:
            return shiftrightequal(op, left, right);
        case BITANDEQUAL:
            return bitandequal(op, left, right);
        case BITXOREQUAL:
            return bitxorequal(op, left, right);
        case BITOREQUAL:
            return bitorequal(op, left, right);
    }
}


Interpreter::Symbol* Interpreter::postpp(Operator *op, Operand *left, Operand *right) {
    Temporary *result = new Temporary(left->iVal);
    temporaries[result->id] = result;
    left->iVal++;
    left->dVal++;
    return result;
}

Interpreter::Symbol* Interpreter::postmm(Operator *op, Operand *left, Operand *right) {
    Temporary *result = new Temporary(left->iVal);
    temporaries[result->id] = result;
    left->iVal--;
    left->dVal--;
    return result;
}

Interpreter::Symbol* Interpreter::prepp(Operator *op, Operand *left, Operand *right) {
    left->iVal++;
    left->dVal++;
    return left;
}

Interpreter::Symbol* Interpreter::premm(Operator *op, Operand *left, Operand *right) {
    left->iVal--;
    left->dVal--;
    return left;
}

Interpreter::Symbol* Interpreter::uplus(Operator *op, Operand *left, Operand *right) {
    Temporary *result;
    if (left->vType == INTEGER) result = new Temporary(left->iVal);
    else result = new Temporary(left->dVal);
    temporaries[result->id] = result;
    return result;
}

Interpreter::Symbol* Interpreter::uminus(Operator *op, Operand *left, Operand *right) {
    Temporary *result;
    if (left->vType == INTEGER) result = new Temporary( -(left->iVal) );
    else result = new Temporary( -(left->dVal) );
    temporaries[result->id] = result;
    return result;
}

Interpreter::Symbol* Interpreter::negation(Operator *op, Operand *left, Operand *right) {
    Temporary *result;
    if (left->vType == INTEGER) result = new Temporary( !(left->iVal) );
    else result = new Temporary( !(left->dVal) );
    temporaries[result->id] = result;
    return result;
}

Interpreter::Symbol* Interpreter::bitnot(Operator *op, Operand *left, Operand *right) {
    Temporary *result = new Temporary( ~(left->iVal) );
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::multiply(Operator *op, Operand *left, Operand *right) {
    Temporary *result;
    if (left->vType == INTEGER && right->vType == INTEGER) {
        result = new Temporary(left->iVal * right->iVal);
    }
    else result = new Temporary(left->dVal * right->dVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::divide(Operator *op, Operand *left, Operand *right) {
    Temporary *result;
    if (left->vType == INTEGER && right->vType == INTEGER) {
        result = new Temporary(left->iVal / right->iVal);
    }
    else result = new Temporary(left->dVal / right->dVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::remainder(Operator *op, Operand *left, Operand *right) {
    Temporary *result = new Temporary(left->iVal % right->iVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::bplus(Operator *op, Operand *left, Operand *right) {
    Temporary *result; 
    if (left->vType == INTEGER && right->vType == INTEGER) {
        result = new Temporary(left->iVal + right->iVal);
    }
    else result = new Temporary(left->dVal + right->dVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::bminus(Operator *op, Operand *left, Operand *right) {
    Temporary *result; 
    if (left->vType == INTEGER && right->vType == INTEGER) {
        result = new Temporary(left->iVal - right->iVal);
    }
    else result = new Temporary(left->dVal - right->dVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::shiftleft(Operator *op, Operand *left, Operand *right) {
    Temporary *result = new Temporary(left->iVal << right->iVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::shiftright(Operator *op, Operand *left, Operand *right) {
    Temporary *result = new Temporary(left->iVal >> right->iVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::lessthan(Operator *op, Operand *left, Operand *right) {
    Temporary *result; 
    if (left->vType == INTEGER && right->vType == INTEGER) {
        result = new Temporary(left->iVal < right->iVal);
    }
    else result = new Temporary(left->dVal < right->dVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::lessthaneq(Operator *op, Operand *left, Operand *right) {
    Temporary *result; 
    if (left->vType == INTEGER && right->vType == INTEGER) {
        result = new Temporary(left->iVal <= right->iVal);
    }
    else result = new Temporary(left->dVal <= right->dVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::greaterthan(Operator *op, Operand *left, Operand *right) {
    Temporary *result; 
    if (left->vType == INTEGER && right->vType == INTEGER) {
        result = new Temporary(left->iVal > right->iVal);
    }
    else result = new Temporary(left->dVal > right->dVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::greaterthaneq(Operator *op, Operand *left, Operand *right) {
    Temporary *result; 
    if (left->vType == INTEGER && right->vType == INTEGER) {
        result = new Temporary(left->iVal >= right->iVal);
    }
    else result = new Temporary(left->dVal >= right->dVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::equal(Operator *op, Operand *left, Operand *right) {
    Temporary *result; 
    if (left->vType == INTEGER && right->vType == INTEGER) {
        result = new Temporary(left->iVal == right->iVal);
    }
    else result = new Temporary(left->dVal == right->dVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::notequal(Operator *op, Operand *left, Operand *right) {
    Temporary *result; 
    if (left->vType == INTEGER && right->vType == INTEGER) {
        result = new Temporary(left->iVal != right->iVal);
    }
    else result = new Temporary(left->dVal != right->dVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::bitwiseand(Operator *op, Operand *left, Operand *right) {
    Temporary *result = new Temporary(left->iVal & right->iVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::bitxor(Operator *op, Operand *left, Operand *right) {
    Temporary *result = new Temporary(left->iVal ^ right->iVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::bitwiseor(Operator *op, Operand *left, Operand *right) {
    Temporary *result = new Temporary(left->iVal | right->iVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::logicaland(Operator *op, Operand *left, Operand *right) {
    Temporary *result = new Temporary(left->iVal && right->iVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::logicalor(Operator *op, Operand *left, Operand *right) {
    Temporary *result = new Temporary(left->iVal || right->iVal);
    temporaries[result->id] = result;
    return result;
}
Interpreter::Symbol* Interpreter::assignment(Operator *op, Operand *left, Operand *right) {
    left->iVal = right->iVal;
    left->dVal = right->dVal;
    return left;
}
Interpreter::Symbol* Interpreter::plusequal(Operator *op, Operand *left, Operand *right) {
    if (left->vType == INTEGER) {
        left->iVal += right->iVal;
        left->dVal = left->iVal;
    }
    else {
        left->dVal += right->dVal;
        left->iVal = left->dVal;
    }
    return left;
}
Interpreter::Symbol* Interpreter::minusequal(Operator *op, Operand *left, Operand *right) {
    if (left->vType == INTEGER) {
        left->iVal -= right->iVal;
        left->dVal = left->iVal;
    }
    else {
        left->dVal -= right->dVal;
        left->iVal = left->dVal;
    }
    return left;
}
Interpreter::Symbol* Interpreter::multequal(Operator *op, Operand *left, Operand *right) {
    if (left->vType == INTEGER) {
        left->iVal *= right->iVal;
        left->dVal = left->iVal;
    }
    else {
        left->dVal *= right->dVal;
        left->iVal = left->dVal;
    }
    return left;
}
Interpreter::Symbol* Interpreter::divequal(Operator *op, Operand *left, Operand *right) {
    if (left->vType == INTEGER) {
        left->iVal /= right->iVal;
        left->dVal = left->iVal;
    }
    else {
        left->dVal /= right->dVal;
        left->iVal = left->dVal;
    }
    return left;
}
Interpreter::Symbol* Interpreter::remequal(Operator *op, Operand *left, Operand *right) {
    left->iVal %= right->iVal;
    left->dVal = left->iVal;
    return left;
}
Interpreter::Symbol* Interpreter::shiftleftequal(Operator *op, Operand *left, Operand *right) {
    left->iVal <<= right->iVal;
    left->dVal = left->iVal;
    return left;
}
Interpreter::Symbol* Interpreter::shiftrightequal(Operator *op, Operand *left, Operand *right) {
    left->iVal >>= right->iVal;
    left->dVal = left->iVal;
    return left;
}
Interpreter::Symbol* Interpreter::bitandequal(Operator *op, Operand *left, Operand *right) {
    left->iVal &= right->iVal;
    left->dVal = left->iVal;
    return left;
}
Interpreter::Symbol* Interpreter::bitxorequal(Operator *op, Operand *left, Operand *right) {
    left->iVal ^= right->iVal;
    left->dVal = left->iVal;
    return left;
}
Interpreter::Symbol* Interpreter::bitorequal(Operator *op, Operand *left, Operand *right) {
    left->iVal |= right->iVal;
    left->dVal = left->iVal;
    return left;
}


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

static string opCodes[] = { 
    "POSTPP", "POSTMM", "PREPP", "PREMM", "UPLUS", "UMINUS", "NEGATION", "BITNOT", "MULTIPLY", "DIVIDE", 
    "REMAINDER", "BPLUS", "BMINUS", "SHIFTLEFT", "SHIFTRIGHT", "LESSTHAN", "LESSTHANEQ", 
    "GREATERTHAN", "GREATERTHANEQ", "EQUAL", "NOTEQUAL", "BITAND", "BITXOR", "BITOR", 
    "LOGICALAND", "LOGICALOR", "ASSIGNMENT", "PLUSEQUAL", "MINUSEQUAL", "MULTEQUAL", 
    "DIVEQUAL", "REMEQUAL", "SHIFTLEFTEQUAL", "SHIFTRIGHTEQUAL", "BITANDEQUAL", 
    "BITXOREQUAL", "BITOREQUAL" 
};

string Interpreter::Operator::ToString() const {
    stringstream ss;
    string leftType = validLeft == INTEGER ? "INTEGER" : validLeft == DOUBLE ? "DOUBLE" : "NONE";
    string rightType = validRight == INTEGER ? "INTEGER" : validRight == DOUBLE ? "DOUBLE" : "NONE";
    string orderType = order == LEFT_TO_RIGHT ? "LEFT_TO_RIGHT" : "RIGHT_TO_LEFT";
    ss << "\t" << setw(5) << name << setw(20) << opCodes[opcode] << setw(5) << arguments << setw(5) << priority << setw(15) << orderType << setw(10) << leftType << setw(10) << rightType << endl;
    return ss.str();
}

static int tempNum = 0;
Interpreter::Temporary::Temporary(int value) {
    id = tempNum;
    stringstream ss;
    ss << "$";
    ss << tempNum++;
    name = ss.str();
    vType = INTEGER;
    iVal = value;
    dVal = value;
}

Interpreter::Temporary::Temporary(double value) {
    id = tempNum;
    stringstream ss;
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
    string typeName = vType == INTEGER ? "INTEGER" : "DOUBLE";
    stringstream ss;
    ss << "\t" << setw(10) << name << setw(10) << typeName << setw(10) << (vType == INTEGER ? iVal : dVal) << endl;
    return ss.str();
}

Interpreter::Number::Number(string N) {
    name = N;
    vType = N.find('.') != string::npos ? DOUBLE : INTEGER;
    stringstream ss;
    ss << N;
    ss >> dVal;
    iVal = dVal;
}

symbol_type Interpreter::Number::IsA() const {
    return NUMBER;
}

string Interpreter::Number::ToString() const {
    string typeName = vType == INTEGER ? "INTEGER" : "DOUBLE";
    stringstream ss;
    ss << "\t" << setw(10) << name << setw(10) << typeName << setw(10) << (vType == INTEGER ? iVal : dVal) << endl;
    return ss.str();
}

Interpreter::Variable::Variable(string N, value_type T) {
    name = N;
    vType = T;
}

symbol_type Interpreter::Variable::IsA() const {
    return VARIABLE;
}

string Interpreter::Variable::ToString() const {
    string typeName = vType == INTEGER ? "INTEGER" : "DOUBLE";
    stringstream ss;
    ss << "\t" << setw(10) << name << setw(10) << typeName << setw(10) << (vType == INTEGER ? iVal : dVal) << endl;
    return ss.str();
}

Interpreter::Symbol::Symbol () {}
Interpreter::Symbol::~Symbol() {}
Interpreter::Operand::Operand () {}
Interpreter::Operand::~Operand() {}
Interpreter::Variable::Variable () {}
Interpreter::Variable::~Variable() {}
Interpreter::Number::Number () {}
Interpreter::Number::~Number() {}
Interpreter::Temporary::Temporary () {}
Interpreter::Temporary::~Temporary() {}
Interpreter::Operator::Operator () {}
Interpreter::Operator::~Operator() {}
