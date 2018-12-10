#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"
#include "symbol.h"

struct Parser {
    // Lexema module
	Token previous_token;
	Token curr_token;
	Scanner* scanner = NULL;

    // Sematic module
    sym_table *tx;
    sym_table *called_table = NULL; // bang ky hieu dang duoc goi trong cau lenh CALL
    symbol *curr_sym = new symbol;
    int nParameters = 0;
    Category category;

	
	Parser(std::string src) {
		if (!scanner) scanner = new Scanner(src);
		curr_token = scanner->getToken();
	}
	
	~Parser() {
		if (scanner) {
			delete scanner;
			scanner = NULL;
		}
	}

	void nextToken();
	void error(const char* msg) const;    // throw error
    void error(const int code, std::string msg = "") const; 
	Category factor();			
	Category term();
	Category expression();
	void condition();
	void statement();
	void block();
	void program();

    // sematic
    void compile_const_declaration();
    void compile_var_declaration();

    // sematic actions
    sym_table *mktable(sym_table* parent);
    sym_table *mktable(sym_table* parent, std::string name);
};

#endif
