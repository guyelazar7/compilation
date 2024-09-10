#include "symTable.h"
#include "hw3_output.hpp"
#include <iostream>

extern TableStack tables;

Symbol *SymTable::getSymbol(const string &name) {
    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        if ((*it)->name == name)
            return (*it);
    }
    return nullptr;
}
void SymTable::addSymbol(const Symbol &symbol) {
    symbols.push_back(new Symbol(symbol));
    if (symbol.offset >= 0)
        lastOffset = symbol.offset;
}





TableStack::TableStack() : tableStack(), offsets() {
    offsets.push_back(0);
    addScope(false);
    addSymbol("print", "void", true, "string");
    addSymbol("printi", "void", true, "int");
    addSymbol("readi","int",true,"int");
}

Symbol *TableStack::getSymbol(const string &name) {
    for (auto it = tableStack.begin(); it != tableStack.end(); ++it) {
        Symbol *symbol = (*it)->getSymbol(name);
        if (symbol)
            return symbol;
    }
    return nullptr;
}

void TableStack::addSymbol(const string &name, const string &type, bool is_function, string param) {
    SymTable *current_scope = tableStack.back();
    int offset;
    if (is_function) {
        offset = 0;
    } else {
        offset = offsets.back();
        offsets.push_back(offset + 1);
    }

    Symbol symbol = Symbol(name, type, offset, is_function, param);
    current_scope->addSymbol(symbol);
}

SymTable *TableStack::currentScope() {
    return tableStack.back();
}
void TableStack::addScope(bool is_loop, string return_type) {
    SymTable* new_scope = new SymTable(offsets.back(), is_loop, return_type);
    this->tableStack.push_back(new_scope);
    SymTable* current_scope = tableStack.back();
    offsets.push_back(current_scope->lastOffset);
}

void TableStack::removeScope() {
    SymTable *scope = tableStack.back();
    tableStack.pop_back();
    output::endScope();
    for (auto it = scope->symbols.begin(); it != scope->symbols.end(); ++it) {
        auto offset = offsets.back();
        offsets.pop_back();
        if ((*it)->func) {
            output::printID((*it)->name, 0,
                            output::makeFunctionType(upperCase((*it)->param),upperCase((*it)->type)));
        } else {
            output::printID((*it)->name, (*it)->offset, upperCase((*it)->type));
        }
    }
    if(offsets.size() > 0)
        offsets.pop_back();
    delete scope;
}
bool TableStack::inLoop() {
    for (auto it = tableStack.rbegin(); it != tableStack.rend(); ++it) {
        SymTable *current = *it;
        if (current->inLoop)
            return true;
    }
    return false;
}
void TableStack::checkSemanticsProgram() {
    tables.removeScope();
    exit(0);
}


string upperCase(const string &str) {
    string res="";
    for(char c:str){
        res+=toupper(c);
    }
    return res;

}