#include "symbol.h"

sym_table *read_tb = new sym_table(new symbol("n", VAR_REF));
sym_table *write_tb = new sym_table(new symbol("n", PARA_INT));
sym_table *writeln_tb = new sym_table(new symbol("n", PARA_INT));

std::vector<symbol*> builtin_fuction = {
    new symbol("READ", PROC, read_tb), new symbol("WRITE", PROC, write_tb),
    new symbol("WRITELN", PROC, writeln_tb)
};

void sym_table::add_sym(symbol *s) {
    if (s->type != CONST_INT) {
        s->offset = memory_size/INT_SIZE_MC;
        s->level = level;
    }

    if (s->type == PARA_INT| s->type == VAR_REF) {
        parameter_sym.push_back(s);
        memory_size += INT_SIZE_MC;
    } else {
        declared_sym.push_back(s);

        if (s->type == VAR_INT) memory_size += INT_SIZE_MC;
        if (s->type == VAR_ARRAY) {
            memory_size += s->size_of_array * INT_SIZE_MC;
        }
    }
}

bool sym_table::is_declared(const symbol *s) {
    if (check_IO(s->name)) return true;

    for (size_t i = 0; i < parameter_sym.size(); i++) {
        if (s->name == parameter_sym[i]->name) return true;
    }

    for (size_t i = 0; i < declared_sym.size(); i++) {
        if (s->name == declared_sym[i]->name) return true;
    }

    return false;
}

symbol* sym_table::find(const std::string ident) {
    for (size_t i = 0; i < builtin_fuction.size(); i++) {
        if (builtin_fuction[i]->name == ident) return builtin_fuction[i];
    }

    sym_table *current = this;
    while (current) {
        for (size_t i = 0; i < current->parameter_sym.size(); i++) {
            if (current->parameter_sym[i]->name == ident) {
                return current->parameter_sym[i];
            }
        }

        // if not found symbol in parameter, find it in declared list instead
        for (size_t i = 0; i < current->declared_sym.size(); i++) {
            if (current->declared_sym[i]->name == ident) {
                return current->declared_sym[i];
            }
        }

        current = current->parent;
    }

    return NULL;
}

bool sym_table::check_para(Category cate, int index)  {
    if (parameter_sym[index]->type == VAR_REF && cate != LVALUE) return false;
    return true;
}

bool sym_table::check_IO(std::string ident) const {
    std::vector<std::string> _stdio = {"READ", "WRITE", "WRITELN"};
    for (size_t i = 0; i < _stdio.size(); i++) {
        if (_stdio[i] == ident) return true;
    }

    return false;
}
