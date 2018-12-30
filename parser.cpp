#include "parser.h"
#include "code_generator.h"


void Parser::nextToken() {
	do {
		previous_token = curr_token;
		curr_token = scanner->getToken();
	} while (curr_token == None);
}

Category Parser::factor() {
    Category cate = LVALUE;
	if (curr_token == Ident) {
        // Kiem tra dinh danh
        curr_sym = tx->find(curr_token.name);
        if (curr_sym) {
            symbol* restored_sym = curr_sym;

            cate = curr_sym->category;
            if (curr_sym->type == CONST_INT) gen_LC_code(curr_sym->value);
            else {
                if (curr_sym->type == PARA_INT) gen_LV_code(tx->level - curr_sym->level, curr_sym->offset);
                else if (curr_sym->type == VAR_REF) {
                    gen_LV_code(tx->level - curr_sym->level, curr_sym->offset);
                    gen_LI_code();
                } else gen_LA_code(tx->level - curr_sym->level, curr_sym->offset);
            }
            
            nextToken();
            if (curr_token == LBracket) {
                if (curr_sym->type != VAR_ARRAY) error(11, curr_sym->name); // symbol hien tai khong phai la kieu mang 
                nextToken();

                category = expression();
                if (category == LVALUE) gen_LI_code();
                gen_ADD_code();

                curr_sym = restored_sym;

                if (curr_token == RBracket) nextToken();
                else error("Thieu ']' sau" + previous_token);

                cate = LVALUE;
            } else if (curr_sym->type == VAR_ARRAY) error(10, curr_sym->name); // Thieu chi so cua mang

        } else error(4, curr_token.name); // code 4: Bien/Hang chua duoc khai bao
	}
	else if (curr_token == Number) {
        int num = curr_token.get_num();
        nextToken();
        cate = RVALUE;
        gen_LC_code(num);
    }
	else if (curr_token == LParent) {
		nextToken();

		category = expression();
        if (category == LVALUE) gen_LI_code();

        cate = RVALUE;
		if (curr_token == RParent) nextToken();
		else error("Thieu ')' after" + previous_token);
	} else error("Khong xac dinh duoc factor sau" + previous_token);

    return cate;
}

Category Parser::term() {
    Category cate;
	cate = factor();
    if ((curr_token == Mul || curr_token == Percent || curr_token == Slash) && cate == LVALUE) gen_LI_code();

    int _operator; 
    while (curr_token == Mul || curr_token == Percent || curr_token == Slash) { 
        if (curr_token == Mul) _operator = Mul;
        else if (curr_token == Percent) _operator = Percent;
        else _operator = Slash;

		nextToken();

		category = factor();
        if (category == LVALUE) gen_LI_code();

        if (_operator == Mul) gen_MUL_code();
        else if (_operator == Percent) gen_MOD_code();
        else gen_DIV_code();

        cate = RVALUE;
	}

    return cate;
}

Category Parser::expression() {
    Category cate = LVALUE;
    bool isLvalue = true;
    int sign; 
	if (curr_token == Plus || curr_token == Minus) {
        if (curr_token == Plus) sign = Plus;
        else sign = Minus;

        nextToken();
        isLvalue = false;   
        cate = RVALUE;
    }

    if (isLvalue) {
        cate = term();
        if (cate == RVALUE) isLvalue = false;
    }
    else {
        category = term();
        if (category == LVALUE) gen_LI_code();
        if (sign == Minus) gen_NEG_code();
    }

    if (isLvalue && (curr_token == Plus || curr_token == Minus)) gen_LI_code();

	while (curr_token == Plus || curr_token == Minus) {
        if (curr_token == Plus) sign = Plus;
        else sign = Minus;

		nextToken();

        category = term();
        if (category == LVALUE) gen_LI_code();

        if (sign == Plus) gen_ADD_code();
        else gen_SUB_code();

        isLvalue = false;
	}

    if (isLvalue) return LVALUE;
    else return RVALUE;
}

void Parser::condition() {
	if (curr_token == ODD) {
		nextToken();

		category = expression();
        if (category == LVALUE) gen_LI_code();
        gen_ODD_code();
	} else {
		category = expression();
        if (category == LVALUE) gen_LI_code();

        int _cmp_op; // 0: Eq, 1: Gt, 2: Ge, 3: Lt, 4: Le, 5: Ne
		if (curr_token == Eq || curr_token == Gt || curr_token == Ge ||
		    curr_token == Lt || curr_token == Le || curr_token == Ne) {

            if (curr_token == Eq) _cmp_op = Eq;
            else if (curr_token == Gt) _cmp_op = Gt;
            else if (curr_token == Ge) _cmp_op = Ge;
            else if (curr_token == Lt) _cmp_op = Lt;
            else if (curr_token == Le) _cmp_op = Le;
            else _cmp_op = Ne;

			nextToken();

            category = expression();
            if (category == LVALUE) gen_LI_code();

            if (_cmp_op == Eq) gen_EQ_code();
            else if (_cmp_op == Gt) gen_GT_code();
            else if (_cmp_op == Ge) gen_GE_code();
            else if (_cmp_op == Lt) gen_LT_code();
            else if (_cmp_op == Le) gen_LE_code();
            else gen_NE_code();

		} else error("Thieu toan tu so sanh sau" + previous_token);
	} 
}

void Parser::statement() {
	if (curr_token == Ident) {
        curr_sym = tx->find(curr_token.name);
        if (curr_sym) {
            nextToken();
            if (curr_sym->type == VAR_REF) gen_LV_code(tx->level - curr_sym->level, curr_sym->offset);
            else gen_LA_code(tx->level - curr_sym->level, curr_sym->offset);

            if (curr_token == LBracket) {
                nextToken();

                symbol* restored_sym = curr_sym;

                if (curr_sym->type != VAR_ARRAY) error(11, curr_sym->name); // symbol khong phai la kieu mang 

                category = expression();        // expression se thay doi curr_sym
                if (category == LVALUE) gen_LI_code();

                curr_sym = restored_sym;

                if (curr_token == RBracket) {
                    nextToken();
                    gen_ADD_code();
                }
                else error("Thieu ']' sau" + previous_token);
            } else if (curr_sym->type == VAR_ARRAY) error(10); // Thieu chi so cua mang
            
            if (curr_token == Assign) {
                if (curr_sym->type == CONST_INT) error(5, curr_sym->name); // code 5: Day la hang so
                nextToken();

                category = expression();
                if (category == LVALUE) gen_LI_code();

                gen_ST_code();

            } else error("Su dung ':=' thay vi" + curr_token);
        } else error(4, curr_token.name); // code 4: Bien/Hang chua duoc khai bao
	} 
	else if (curr_token == CALL) {
		nextToken();

		if (curr_token == Ident) {
            // kiem tra dinh danh
            curr_sym = tx->find(curr_token.name);
            if (curr_sym && curr_sym->type == PROC) {
                nextToken();

                if (curr_sym->name != "READ" && curr_sym->name != "WRITE" && curr_sym->name != "WRITELN") gen_INT_code(FRAME_REGISTER_NUM);

                symbol* restored_sym = curr_sym;

                called_table = curr_sym->next;
                nParameters = 0;

                if (curr_token == LParent) {
                    do {
                        // kiem tra tham so
                        nextToken();
                        category = expression();

                        curr_sym = restored_sym;

                        nParameters++;
                        if (nParameters <= called_table->get_nPara()) {
                            if (!called_table->check_para(category, nParameters-1)) error(6, curr_sym->name); // Truyen sai kieu tham so      
                            if (called_table->parameter_sym[nParameters-1]->type == PARA_INT && category == LVALUE) gen_LI_code();
                        } else error(7, curr_sym->name); // Thua tham so
                    } while (curr_token == Comma);
                    
                    if (curr_token == RParent) nextToken();
                    else error("Thieu ')' sau" + previous_token);
                }


                if (nParameters < called_table->get_nPara()) error(8, curr_sym->name); // Thieu tham so     

                if (curr_sym->name == "READ") gen_RI_code();
                else if (curr_sym->name == "WRITE") gen_WRI_code();
                else if (curr_sym->name == "WRITELN") {
                    gen_WRI_code();
                    gen_WLN_code();
                } else {
                    gen_DCT_code(FRAME_REGISTER_NUM + nParameters);
                    gen_CALL_code(tx->level - called_table->level + 1, called_table->begin_addr);
                }

            } else error(9, curr_token.name); // code 9: Thu tuc chua khai bao
		} else error("Thieu dinh danh ham sau" + previous_token);
	}
	else if (curr_token == BEGIN) {
		nextToken();
		statement();
		while (curr_token == Semicolon) {
			nextToken();
			statement();
		}
		if (curr_token == END) nextToken();
		else error("Block can duoc ket thuc boi tu khoa 'END' thay vi" + curr_token);
	}
	else if (curr_token == IF) {
		nextToken();
		condition();
        int id_FJ = gen_FJ_code();
		if (curr_token == THEN) {
			nextToken();
			statement();
			if (curr_token == ELSE) {
                int id_J = gen_J_code();
                update_FJ_label(id_FJ, getCurrentCodeAddress());

				nextToken();
				statement();

                update_J_label(id_J, getCurrentCodeAddress());
			} else update_FJ_label(id_FJ, getCurrentCodeAddress());
		}
	}
	else if (curr_token == WHILE) {
		nextToken();
        int begin = getCurrentCodeAddress();
		condition();
        int id_FJ = gen_FJ_code();
		if (curr_token == DO) {
			nextToken();
			statement();
            gen_J_code(begin);
            update_FJ_label(id_FJ, getCurrentCodeAddress());
		} else error("Thieu tu khoa 'DO' sau" + previous_token);
	}
	else if (curr_token == FOR) {
		nextToken();
		if (curr_token == Ident) {
            // Kiem tra khai bao, loai bien
            curr_sym = tx->find(curr_token.name);
            if (curr_sym) {
                nextToken();
                if (curr_sym->category == LVALUE) {
                    int iterator_depth = tx->level - curr_sym->level;
                    int iterator_offset = curr_sym->offset;
                    gen_LA_code(iterator_depth, iterator_offset);

                    if (curr_token == Assign) {
                        nextToken();
                        category = expression();
                        if (category == LVALUE) gen_LI_code();
                        gen_ST_code();

                        int L1 = getCurrentCodeAddress();
                        gen_LV_code(iterator_depth, iterator_offset);

                        if (curr_token == TO) {
                            nextToken();
                            category = expression();
                            if (category == LVALUE) gen_LI_code();
                            gen_LE_code();
                            int id_FJ = gen_FJ_code();
                            if (curr_token == DO) {
                                nextToken();
                                statement();

                                gen_LA_code(iterator_depth, iterator_offset);
                                gen_LV_code(iterator_depth, iterator_offset);
                                gen_LC_code(1);
                                gen_ADD_code();
                                gen_ST_code();
                                gen_J_code(L1);
                                update_FJ_label(id_FJ, getCurrentCodeAddress());
                            } else error("Thieu tu khoa 'DO' sau" + previous_token);
                        } else error("Thieu tu khoa 'TO' sau" + previous_token);
                    } else error("Thieu toan tu ':=' sau" + previous_token);
                } else {
                    if (curr_sym->type == CONST_INT) error(12, curr_sym->name);
                    if (curr_sym->type == VAR_ARRAY) error(13, curr_sym->name);
                }
            } else error(4, curr_token.name);
		} else error("Thieu dinh danh bien sau" + previous_token);
	} 
}

void Parser::block() {
	if (curr_token == CONST) {
        do {
            nextToken();
            compile_const_declaration();
        } while (curr_token == Comma);

        if (curr_token == Semicolon) nextToken();
        else error("Thieu ';' sau" + previous_token);
	} 
	
	if (curr_token == VAR) {
        do {
            nextToken();
            compile_var_declaration();
        } while (curr_token == Comma);

        if (curr_token == Semicolon) nextToken();
        else error("Thieu ';' sau" + previous_token);
	}
	
	while (curr_token == PROCEDURE) {
		nextToken();
		if (curr_token == Ident) {
			
            // them procedure vao scope hien tai: kiem tra su ton tai cua dinh danh tu scope hien tai cho toi global scope
            if (!tx->find(curr_token.name)) {
                curr_sym = new symbol;
                curr_sym->name = curr_token.name;
                curr_sym->type = PROC;
                curr_sym->next = mktable(tx, tx->level + 1);
                tx->add_sym(curr_sym);
                // bat dau vao scope cua procedure vua duoc them
                tx = curr_sym->next;
            } else error(2, curr_sym->name);
			
            nextToken();
            if (curr_token == LParent) {
                // Them cac tham bien vao sym_table cua procedure hien tai
                do {
                    nextToken();
                    curr_sym = new symbol;
                    curr_sym->type = PARA_INT;
                    if (curr_token == VAR) {
                        nextToken();
                        curr_sym->type = VAR_REF;
                    }

                    if (curr_token == Ident) {
                        curr_sym->name = curr_token.name;
                        if (!tx->is_declared(curr_sym)) tx->add_sym(curr_sym);
                        else error(3); // Dinh danh cua tham bien da ton tai

                        nextToken();
                    } else error("Thieu tham so sau" + previous_token);
                } while (curr_token == Semicolon);

                if (curr_token == RParent) nextToken();
                else error("Thieu ')' sau" + previous_token);
            } 

            if (curr_token == Semicolon) {
                nextToken();
                block();
				
                if (curr_token == Semicolon) {
                    nextToken();

                    // Thoat khoi Scope cua procedure hien tai
                    tx = tx->parent;
                    gen_EP_code();
                }
                else error("Thieu ';' sau" + previous_token);
            } else error("Thieu ';' sau" + previous_token);
		} else error("Thieu dinh danh ham sau" + previous_token);
	}
	
	if (curr_token == BEGIN) {
		nextToken();

        gen_INT_code(FRAME_REGISTER_NUM + tx->memory_size/INT_SIZE_MC);
        set_proc_address(&tx->begin_addr);

		statement();
		while (curr_token == Semicolon) {
			nextToken();
			statement();
		}

		if (curr_token == END) {
			nextToken();
		}
		else if (curr_token == Dot) error("Can su dung tu khoa 'END' truoc" + curr_token);
		else error("Su dung tu khoa 'END' thay vi" + curr_token);
	} else if (curr_token.name.empty()) error("Thieu than chuong trinh");
    else error("Khong xac dinh duoc block bat dau voi" + curr_token);
}

void Parser::program() {
    tx = mktable(NULL);
	if (curr_token == PROGRAM) {
		nextToken();
		if (curr_token == Ident) {
			nextToken();
			if (curr_token == Semicolon) {
				nextToken();

                gen_J_code();
				block();
                update_J_label(0, tx->begin_addr);

				if (curr_token == Dot) {
                    gen_HLT_code();
                    std::cout << "\n=======================================================\n";
                    std::cout << "Thanh cong!\n";
                }
				else error("Thieu '.' sau" + previous_token);
			} else error("Thieu ';' sau" + previous_token);
		} else error("Thieu ten chuong trinh");
	} else error("Thieu tu khoa PROGRAM");
}

void Parser::compile_const_declaration() {
    if (curr_token == Ident) {
        // them symbol 
        curr_sym = new symbol;
        curr_sym->name = curr_token.name;
        curr_sym->type = CONST_INT;

        // kiem tra trung ten
        if (!tx->is_declared(curr_sym)) {
            nextToken();
            if (curr_token == Eq) {
                nextToken();
                if (curr_token == Number) {
                    // symbol value
                    curr_sym->value = curr_token.get_num();
                    nextToken();
                } else error("Thieu hang so sau" + previous_token);
            } else error("Thieu '=' sau" + previous_token);

            tx->add_sym(curr_sym);
        } else error(1, curr_sym->name); // code 1: Khai bao trung ten
    } else error("Thieu dinh danh hang sau" + previous_token);
}

void Parser::compile_var_declaration() {
    if (curr_token == Ident) {
        curr_sym = new symbol;
        curr_sym->name = curr_token.name;
        curr_sym->type = VAR_INT;
        curr_sym->category = LVALUE;
        if (!tx->is_declared(curr_sym)) {
            nextToken();
            if (curr_token == LBracket) {
                nextToken();
                if (curr_token == Number) {
                    curr_sym->type = VAR_ARRAY;
                    curr_sym->category = RVALUE;  // a != a[0]
                    curr_sym->size_of_array = curr_token.get_num();
                    nextToken();
                    if (curr_token == RBracket) nextToken();
                    else error("Thieu ']' sau" + previous_token);
                } else error("Thieu chi dinh so luong phan tu sau" + previous_token);
            } 

            tx->add_sym(curr_sym);
        } else error(1, curr_sym->name); // code 1: khai bao trung ten
    } else error("Thieu dinh danh bien sau" + previous_token);
}

sym_table* Parser::mktable(sym_table *parent) {
    sym_table* table = new sym_table;
    table->parent = parent;
    return table;
}

// Debug
sym_table* Parser::mktable(sym_table *parent, int curr_level) {
    sym_table* table = new sym_table(curr_level);
    table->parent = parent;
    return table;
}
// Debug

void Parser::error(const std::string msg) const {
    int line = scanner->curr_line;
    if (curr_token == Eof) line--;
    std::cout << "\n=======================================================\n";
	std::cout << "[ERROR] Loi cu phap tai dong " << line << ": " << msg << '\n';
	exit(2);
}

// Sematic error
void Parser::error(const int code, std::string msg) const {
    std::cout << "\n=======================================================\n";
    std::cout << "[ERROR] Dong " << scanner->curr_line << ": ";
    switch(code) {
        case 1: 
            std::cout << "Khong the dinh nghia lai bien '" + msg + "' trong scope hien tai";
            break;

        case 2: 
            std::cout << "Ten thu tuc " + msg + " trung voi ten bien/hang/thu tuc da khai bao";
            break;

        case 3: 
            std::cout << "Dinh danh cua tham bien da ton tai";
            break;

        case 4: 
            std::cout << "Bien/Hang " + msg + " chua duoc khai bao";
            break;

        case 5: 
            std::cout << "Khong the thay doi gia tri cua hang so " + msg;
            break;

        case 6: 
            std::cout << "Khong khop voi khai bao cua thu tuc " + msg + " (Tham so thu " << nParameters << " la phai la mot bien hoac mot phan tu cua mang)";
            break;

        case 7: 
            std::cout << "Khong khop voi khai bao cua thu tuc " + msg + " (Thua tham so)";
            break;

        case 8: 
            std::cout << "Khong khop voi khai bao cua thu tuc " + msg + " (Thieu tham so)";
            break;

        case 9: 
            std::cout << "Khong ton tai thu tuc " + msg; 
            break;
			
        case 10: 
            std::cout << "Thieu chi so cua mang " + msg;
            break;

        case 11: 
            std::cout << msg + " khong phai la kieu mang";
            break;

        case 12:
            std::cout << "Hang so " + msg + " khong the duoc dung lam con chay";
            break;

        case 13:
            std::cout << "Bien mang " + msg + " khong the duoc dung lam con chay";
            break;
    };

    std::cout << '\n';

    exit(EXIT_FAILURE);
}

