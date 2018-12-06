#ifndef SYMBOL_H
#define SYMBOL_H
#include "Token.h"

enum Type {
    DEFAULT = -1, 
    CONST_INT, VAR_INT, VAR_ARRAY, PROC, // Declare
    VAR_REF, PARA_INT, // Parameter
};

enum Category {
    LVALUE, // variable
    RVALUE,  // const, number
};

struct sym_table;

// define struct of a symbol
struct symbol {
    std::string name;
    union {
        int value;
        int size_of_array;
    };

    Type type;
    Category category = LVALUE;
    sym_table *next = nullptr;

    void clear() {name.clear(); value = -1; type = DEFAULT; next = nullptr; category = LVALUE;}
};

// define symbol table of a procedure
struct sym_table{
    std::vector<symbol> declared_sym;
    std::vector<symbol> parameter_sym;
    int size = 0;
    sym_table *parent = nullptr;

    // Them 1 symbol vao bang ki hieu
    void add_sym(symbol s);

    // Kiem tra khai bao cua symbol trong scope hien tai
    bool is_declared(const symbol s);

    // Tim symbol tu scope hien tai cho toi het global scope
    symbol* find(const std::string ident);

    int get_nPara() {return (int)parameter_sym.size();}
    bool check_para(Category cate, int index);

    // just for debug
    std::string name;
    int id;
    sym_table(int _id) {id = _id;}
    sym_table() {}
    sym_table(std::string _name) {name = _name;}
};

#endif
