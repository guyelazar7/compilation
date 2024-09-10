#ifndef COMPI_HW3_SYM_TABLE_H
#define COMPI_HW3_SYM_TABLE_H

#include <string>
#include <vector>
#include <iostream>
#include<cctype>
using std::string;
using std::vector;

string upperCase(const string &str);

class Symbol {
public:
    string name;
    string type;
    int offset;
    bool func;
    string param;

    Symbol(const string name, const string type, int offset, bool func, string param="") : name(name),
    type(type),offset(offset),func(func),param(param){}

    ~Symbol() = default;
};

class SymTable {

public:
    vector<Symbol*> symbols;
    int lastOffset;
    bool inLoop;
    string retType;

    SymTable(int offset, bool inLoop, string return_type = "")
            : symbols(), lastOffset(offset), inLoop(inLoop),retType(return_type) {
    }
    ~SymTable(){
        for(auto it = symbols.begin(); it != symbols.end(); it++)
            delete (*it);
    }

    Symbol *getSymbol(const string &name);
    void addSymbol(const Symbol &symbol);
};

class TableStack {
public:
    vector<SymTable*> tableStack;
    vector<int> offsets;

    TableStack();
    ~TableStack(){
        for(auto it = tableStack.begin(); it != tableStack.end(); it++){
            SymTable* current = *it;
            delete current;
        }

    }

    SymTable *currentScope();
    void addScope(bool is_loop = false, string return_type = "");
    void removeScope();

    Symbol *getSymbol(const string &name);
    void addSymbol(const string &name, const string &type, bool is_function,string param="");

    bool inLoop();
    void checkSemanticsProgram();
};

#endif
