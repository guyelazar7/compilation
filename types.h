#ifndef HW3_TYPES_H
#define HW3_TYPES_H

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

class Node {
public:
    string value;

    Node(const string value = "") : value(value) {
    };
    Node(const Node &node): value(node.value){
    };
    virtual ~Node() = default;
};

#define YYSTYPE Node*

class Program : public Node {
public:
    Program();

    virtual ~Program() = default;
};


class Type : public Node {
public:
    string type;

    Type(const string type);

    virtual ~Type() = default;
};


class Exp : public Node {
public:
    string type;
    string value;
    bool var=false;

    Exp() : type("void"), value("") {}

    Exp(Node *terminal, string type);

    Exp(Node *terminal1, Node *terminal2, const string op, const string type);

    Exp(bool var, Node *terminal);

    Exp(Node *exp, Node *type);

    Exp(Exp *exp);

    virtual ~Exp() = default;
};


class Call : public Node {
public:
    string type;
    Call(Node *terminal);
    Call(Node *terminal, Node *exp);
    virtual ~Call() = default;
};

class Statement : public Node {
public:
    Statement(Node *node);

    Statement(Type *type, Node *id);

    Statement(Type *type, Node *id, Exp *exp);

    Statement(Node *id, Exp *exp);

    Statement(const string name, Exp *exp);

    Statement(Exp *exp, bool isRet=false);

    Statement(Call *call);

    virtual ~Statement() = default;
};

class Statements : public Node {
public:
    Statements(Statement *statement) : Node() {};

    Statements(Statements *statements, Statement *statement) : Node() {};

    virtual ~Statements() = default;
};

void isBool(Node* node);

#endif
