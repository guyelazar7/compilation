#include "types.h"
#include "symTable.h"
#include "hw3_output.hpp"

extern TableStack tables;
extern int yylineno;

static bool checkTypes(string type1, string type2) {
    if (type1 == type2)
        return true;

    if (type1 == "bool" || type2 == "bool" || type1 == "string" || type2 == "string"||type2=="void")
        return false;
    return true;
}

Statement::Statement(Node *node) {
    if (node->value == "break") {
        if (!tables.inLoop()) {
            output::errorUnexpectedBreak(yylineno);
            exit(0);
        }
    } else if (node->value == "continue") {
        if (!tables.inLoop()) {
            output::errorUnexpectedContinue(yylineno);
            exit(0);
        }
    }
}

Statement::Statement(Exp *exp,bool isRet) : Node() {
    SymTable *scope = tables.currentScope();
    string retType = scope->retType;

    if (retType != "" && retType != exp->type) {
        if (retType != "int" || exp->type != "byte") {
            output::errorMismatch(yylineno);
            exit(0);
        }

    }
    if (exp->var) {
        Symbol *symbol = tables.getSymbol(exp->value);
        if (symbol->func) {
            output::errorUndef(yylineno, symbol->name);
            exit(0);
        }
    }
}

Statement::Statement(Type *type, Node *id) : Node() {
    if (tables.getSymbol(id->value)) {
        output::errorDef(yylineno, id->value);
        exit(0);
    }
    tables.addSymbol(id->value, type->type, false);
    value = type->value;//

}

Statement::Statement(Type *type, Node *id, Exp *exp) : Node() {
    if (tables.getSymbol(id->value)) {
        output::errorDef(yylineno, id->value);
        exit(0);
    }
    if (type) {
        if(exp->var&&tables.getSymbol(exp->value)->func){
            output::errorUndef(yylineno,exp->value);
            exit(0);
        }
        if (!checkTypes(type->type, exp->type)) {
            output::errorMismatch(yylineno);
            exit(0);
        }
        if (type->type == "byte" && exp->type == "int") {
            output::errorMismatch(yylineno);
            exit(0);
        }
        tables.addSymbol(id->value, type->type, false);
    } else {
        if (exp->type == "string") {
            output::errorMismatch(yylineno);
            exit(0);
        }
        tables.addSymbol(id->value, exp->type, false);
    }
}
Statement::Statement(Node *id, Exp *exp) : Node() {
    if (!tables.getSymbol(id->value)) {
        output::errorUndef(yylineno, id->value);
        exit(0);
    }
    Symbol *symbol = tables.getSymbol(id->value);
    if(exp->var&&tables.getSymbol(exp->value)->func){
            output::errorUndef(yylineno,exp->value);
            exit(0);
    }
    if (symbol->func || !checkTypes(symbol->type, exp->type)) {
        output::errorMismatch(yylineno);
        exit(0);
    }
    if(symbol->type == "byte" && exp->type == "int"){
        output::errorMismatch(yylineno);
        exit(0);
    }
}

Statement::Statement(Call *call) : Node() {
    if (!tables.getSymbol(call->value)) {
        output::errorUndefFunc(yylineno, call->value);
        exit(0);
    }
    Symbol *symbol = tables.getSymbol(call->value);
    if (!symbol->func) {
        output::errorUndefFunc(yylineno, call->value);
        exit(0);
    }
}

Statement::Statement(const string name, Exp *exp) {
    if (exp->type != "bool") {
        output::errorMismatch(yylineno);
        exit(0);
    }
}

Type::Type(
        const string type) : Node(), type(type) {

}

Exp::Exp(Exp *exp) : Node(exp->value), type(exp->type) {
}

Exp::Exp(Node *terminal, string
type) : Node(terminal->value), type(type) {
    if (type == "byte") {
        int value = stoi(terminal->value);
        if (value > 255) {
            output::errorByteTooLarge(yylineno, terminal->value);
            exit(0);
        }
    }
}

Exp::Exp(bool var, Node *terminal) : Node(), var(var) {
    if (var && !tables.getSymbol(terminal->value)) {
        output::errorUndef(yylineno, terminal->value);
        exit(0);
    }
    Symbol *symbol = tables.getSymbol(terminal->value);
    value = terminal->value;
    type = symbol->type;
}

Exp::Exp(Node *terminal1, Node *terminal2,const string op,const string type) {
    Exp *exp1 = dynamic_cast<Exp *>(terminal1);
    Exp *exp2 = dynamic_cast<Exp *>(terminal2);
    if (exp1->var && !tables.getSymbol(exp1->value)) {
        output::errorUndef(yylineno, terminal1->value);
        exit(0);
    }

    if (exp2->var && !tables.getSymbol(exp2->value)) {
        output::errorUndef(yylineno, terminal2->value);
        exit(0);
    }

    if (!checkTypes(exp1->type, exp2->type)) {
        output::errorMismatch(yylineno);
        exit(0);
    }

    if (type == "bool") {
        if (exp1->type != "bool" || exp2->type != "bool") {
            output::errorMismatch(yylineno);
            exit(0);
        }

        this->type = "bool";
    } else if (type == "int") {

        if ((exp1->type != "int" && exp1->type != "byte") || (exp1->type != "int" && exp1->type != "byte")) {
            output::errorMismatch(yylineno);
            exit(0);
        }
        if (exp1->type == "int" || exp2->type == "int") {
            this->type = "int";
        } else {
            this->type = "byte";
        }
    } else if (type == "relop") {
        if (!checkTypes(exp1->type, exp2->type)) {
            output::errorMismatch(yylineno);
            exit(0);
        }
        if ((exp1->type != "int" && exp1->type != "byte") || (exp1->type != "int" && exp1->type != "byte")) {
            output::errorMismatch(yylineno);
            exit(0);
        }
        this->type = "bool";
    }
}

Exp::Exp(Node *exp, Node *type) {
    Type *convertedType = dynamic_cast<Type *>(type);
    Exp *convertedExp = dynamic_cast<Exp *>(exp);
    if (!checkTypes(convertedExp->type, convertedType->type)) {
        output::errorMismatch(yylineno);
        exit(0);
    }
    this->type = convertedType->type;
    this->value = convertedExp->value;
}
Call::Call(Node *terminal) : Node() {
    string name = terminal->value;
    if (!tables.getSymbol(name)) {
        output::errorUndefFunc(yylineno, name);
        exit(0);
    }

    Symbol *symbol = tables.getSymbol(name);
    if (!symbol->func) {
        output::errorUndefFunc(yylineno, name);
        exit(0);
    }
    type = symbol->type;
    value = symbol->name;
}
Call::Call(Node *terminal, Node* exp) : Node() {
    Exp* e = dynamic_cast<Exp*>(exp);
    string name = terminal->value;
    if (!tables.getSymbol(name)) {
        output::errorUndefFunc(yylineno, name);
        exit(0);
    }
    Symbol *symbol = tables.getSymbol(name);
    if (!symbol->func) {
        output::errorUndefFunc(yylineno, name);
        exit(0);
    }
    if (symbol->param != e->type) {
        if (symbol->param != "int" || e->type != "byte") {
            output::errorPrototypeMismatch(yylineno, name,upperCase(symbol->param));
            exit(0);
        }
    }
    type = symbol->type;
    value = symbol->name;
}


Program::Program() {}

void isBool(Node *node) {
    Exp *exp = dynamic_cast<Exp *>(node);
    if (exp->type != "bool") {
        output::errorMismatch(yylineno);
        exit(0);
    }
}

