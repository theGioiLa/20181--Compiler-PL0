#ifndef SYMBOL_H
#define SYMBOL_H
#include "Token.h"
#define INT_SIZE_MC 2

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

    Type type = DEFAULT;
    Category category = RVALUE;
    
    int offset = -1;
    int level = -1;

    sym_table *next = NULL;         // Sd doi voi PROCE
    symbol() {}
    symbol(std::string _name, Type _type, sym_table *_next = NULL) {
        name = _name;
        type = _type;
        if (_next) next = _next;
    }
};

// define symbol table of a procedure
struct sym_table {
    std::vector<symbol*> declared_sym;
    std::vector<symbol*> parameter_sym;
    int memory_size = 0;
    int level = 0;
    int begin_addr = -1;
    sym_table *parent = NULL;

    // Them 1 symbol vao bang ki hieu
    void add_sym(symbol *s);

    // Kiem tra khai bao cua symbol trong scope hien tai
    bool is_declared(const symbol *s);

    // Tim symbol tu scope hien tai cho toi het global scope
    symbol* find(const std::string ident);

    int get_nPara() {return (int)parameter_sym.size();}
    bool check_para(Category cate, int index);

    bool check_IO(std::string ident) const;

    // just for debug
    sym_table(int _level) {level = _level;}
    sym_table() {}
    sym_table(symbol *n) {
        parameter_sym.push_back(n);
        memory_size += INT_SIZE_MC;
    }
};

#endif
