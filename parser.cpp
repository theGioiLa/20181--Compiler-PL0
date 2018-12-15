#include "parser.h"

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
            cate = curr_sym->category;

            nextToken();
            if (curr_token == LBracket) {
                if (curr_sym->type != VAR_ARRAY) error(11, curr_sym->name); // symbol hien tai khong phai la kieu mang 
                nextToken();
                expression();
                if (curr_token == RBracket) nextToken();
                else error("Thieu ']' sau" + previous_token);
                cate = LVALUE;
            } else if (curr_sym->type == VAR_ARRAY) error(10, curr_sym->name); // Thieu chi so cua mang

        } else error(4, curr_token.name); // code 4: Bien/Hang chua duoc khai bao
	}
	else if (curr_token == Number) {
        nextToken();
        cate = RVALUE;
    }
	else if (curr_token == LParent) {
		nextToken();
		expression();
        cate = RVALUE;
		if (curr_token == RParent) nextToken();
		else error("Thieu ')' after" + previous_token);
	} else error("Khong xac dinh duoc factor sau" + previous_token);

    return cate;
}

Category Parser::term() {
    Category cate;
	cate = factor();
	while (curr_token == Mul || curr_token == Percent || curr_token == Slash) {
		nextToken();
		factor();
        cate = RVALUE;
	}

    return cate;
}

Category Parser::expression() {
    Category cate = LVALUE;
    bool isLvalue = true;
	if (curr_token == Plus || curr_token == Minus) {
        nextToken();
        isLvalue = false;   
        cate = RVALUE;
    }
    if (isLvalue) {
        cate = term();
        if (cate == RVALUE) isLvalue = false;
    }
    else term();

	while (curr_token == Plus || curr_token == Minus) {
		nextToken();
		term();
        isLvalue = false;
	}


    if (isLvalue) return LVALUE;
    else return RVALUE;
}

void Parser::condition() {
	if (curr_token == ODD) {
		nextToken();
		expression();
	} else {
		expression();
		if (curr_token == Eq || curr_token == Gt || curr_token == Ge ||
		    curr_token == Lt || curr_token == Le || curr_token == Ne) {
			nextToken();
			expression();
		} else error("Thieu toan tu so sanh sau" + previous_token);
	} 
}

void Parser::statement() {
	if (curr_token == Ident) {
        curr_sym = tx->find(curr_token.name);
        if (curr_sym) {
            nextToken();
            if (curr_token == LBracket) {
                nextToken();
                expression();
                if (curr_sym->type != VAR_ARRAY) error(11, curr_sym->name); // symbol hien tai khong phai la kieu mang 
                if (curr_token == RBracket) nextToken();
                else error("Thieu ']' sau" + previous_token);
            } else if (curr_sym->type == VAR_ARRAY) error(10); // Thieu chi so cua mang
            
            if (curr_token == Assign) {
                if (curr_sym->type == CONST_INT) error(5, curr_sym->name); // code 5: Day la hang so
                nextToken();
                expression();
            } else error("Su dung ':=' thay vi" + curr_token);
        } else error(4, curr_token.name); // code 4: Bien/Hang chua duoc khai bao
	} 
	else if (curr_token == CALL) {
		nextToken();
		if (curr_token == Ident) {
            // processIO();
            // kiem tra dinh danh
            curr_sym = tx->find(curr_token.name);
            if (curr_sym && curr_sym->type == PROC) {
                nextToken();

                called_table = curr_sym->next;
                nParameters = 0;

                if (curr_token == LParent) {
                    do {
                        // kiem tra tham so
                        nextToken();
                        category = expression();
                        nParameters++;
                        if (nParameters <= called_table->get_nPara()) {
                            if (!called_table->check_para(category, nParameters-1)) error(6, called_table->name); // Truyen sai kieu tham so      
                        } else error(7, called_table->name); // Thua tham so
                    } while (curr_token == Comma);
                    
                    if (curr_token == RParent) nextToken();
                    else error("Thieu ')' sau" + previous_token);
                }

                if (nParameters < called_table->get_nPara()) error(8, called_table->name); // Thieu tham so                }
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
		if (curr_token == THEN) {
			nextToken();
			statement();
			if (curr_token == ELSE) {
				nextToken();
				statement();
			}
		}
	}
	else if (curr_token == WHILE) {
		nextToken();
		condition();
		if (curr_token == DO) {
			nextToken();
			statement();
		} else error("Thieu tu khoa 'DO' sau" + previous_token);
	}
	else if (curr_token == FOR) {
		nextToken();
		if (curr_token == Ident) {
			nextToken();
			if (curr_token == Assign) {
				nextToken();
				expression();
				if (curr_token == TO) {
					nextToken();
					expression();
					if (curr_token == DO) {
						nextToken();
						statement();
					} else error("Thieu tu khoa 'DO' sau" + previous_token);
				} else error("Thieu tu khoa 'TO' sau" + previous_token);
			} else error("Thieu toan tu ':=' sau" + previous_token);
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
                curr_sym->next = mktable(tx, curr_sym->name);
                tx->add_sym((*curr_sym));
                // bat dau vao scope cua procedure vua duoc them
                tx = curr_sym->next;
            } else error(2, curr_sym->name);
			
            nextToken();
            if (curr_token == LParent) {
                // Them cac tham bien vao sym_table cua procedure hien tai
                do {
                    nextToken();
                    curr_sym->clear();
                    curr_sym->type = PARA_INT;
                    if (curr_token == VAR) {
                        nextToken();
                        curr_sym->type = VAR_REF;
                    }

                    if (curr_token == Ident) {
                        curr_sym->name = curr_token.name;
                        if (!tx->is_declared((*curr_sym))) tx->add_sym((*curr_sym));
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
                }
                else error("Thieu ';' sau" + previous_token);
            } else error("Thieu ';' sau" + previous_token);
		} else error("Thieu dinh danh ham sau" + previous_token);
	}
	
	if (curr_token == BEGIN) {
		nextToken();
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
    tx = mktable(NULL, "global");
	if (curr_token == PROGRAM) {
		nextToken();
		if (curr_token == Ident) {
			nextToken();
			if (curr_token == Semicolon) {
				nextToken();
				block();
				if (curr_token == Dot) {
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
        curr_sym->clear();
        curr_sym->name = curr_token.name;
        curr_sym->type = CONST_INT;
        curr_sym->category = RVALUE;

        // kiem tra trung ten
        if (!tx->is_declared((*curr_sym))) {
            nextToken();
            if (curr_token == Eq) {
                nextToken();
                if (curr_token == Number) {
                    // symbol value
                    curr_sym->value = curr_token.get_num();
                    // 
                    nextToken();
                }
                else error("Thieu hang so sau" + previous_token);
            } else error("Thieu '=' sau" + previous_token);

            tx->add_sym((*curr_sym));
        } else error(1, curr_sym->name); // code 1: Khai bao trung ten
    } else error("Thieu dinh danh hang sau" + previous_token);
}

void Parser::compile_var_declaration() {
    if (curr_token == Ident) {
        curr_sym->clear();
        curr_sym->name = curr_token.name;
        curr_sym->type = VAR_INT;
        if (!tx->is_declared((*curr_sym))) {
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

            tx->add_sym((*curr_sym));
        } else error(1, curr_sym->name); // code 1: khai bao trung ten
    } else error("Thieu dinh danh bien sau" + previous_token);
}

sym_table* Parser::mktable(sym_table *parent) {
    sym_table* table = new sym_table;
    table->parent = parent;
    return table;
}

// Debug
sym_table* Parser::mktable(sym_table *parent, std::string name) {
    sym_table* table = new sym_table(name);
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
    };

    std::cout << '\n';

    exit(EXIT_FAILURE);
}

