#include "symbol.h"

void sym_table::add_sym(symbol s) {
    if (s.type == PARA_INT| s.type == VAR_REF) {
        parameter_sym.push_back(s);
    } else {
        declared_sym.push_back(s);

        if (s.type == VAR_INT) size += sizeof(int);
        if (s.type == VAR_ARRAY) {
            size += sizeof(int) * s.size_of_array;
        }
    }
}

bool sym_table::is_declared(const symbol s) {
    for (size_t i = 0; i < parameter_sym.size(); i++) {
        if (s.name == parameter_sym[i].name) return true;
    }

    for (size_t i = 0; i < declared_sym.size(); i++) {
        if (s.name == declared_sym[i].name) return true;
    }

    return false;
}

symbol* sym_table::find(const std::string ident) {
    sym_table *current = this;
    while (current) {
        for (size_t i = 0; i < current->parameter_sym.size(); i++) {
            if (current->parameter_sym[i].name == ident) {
                return &(current->parameter_sym[i]);
                break;
            }
        }

        // if not found symbol in parameter, find it in declared list instead
        for (size_t i = 0; i < current->declared_sym.size(); i++) {
            if (current->declared_sym[i].name == ident) {
                return &(current->declared_sym[i]);
                break;
            }
        }

        current = current->parent;
    }

    return NULL;
}

bool sym_table::check_para(Category cate, int index)  {
    if (parameter_sym[index].type == VAR_REF && cate != LVALUE) return false;
    return true;
}
