#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#define MAX_LENGTH_NUMBER 9
#define MAX_LENGTH_IDENT 10


enum Token_Type {
    // -------------------Invalid Symbol---------------------
	None = 0, Eof,

    // -----------------Identifier, Number-------------------
    Ident, Number,

    // ------------------Keywords----------------------------
    BEGIN, END, IF, THEN, WHILE, DO, CALL, ODD, TO,
    CONST, VAR, PROCEDURE, PROGRAM, ELSE, FOR, 
    // WRITE, WRITELN, READ, READLN,

    // ------------------Operators---------------------------
    Plus, Minus, Mul, Slash, Percent, 					// + - * / %
    Assign,												// :=
    Eq, Ne, Lt, Gt, Ge, Le,								// = <> < > >= <=

    // ------------------------------------------------------
    LParent, RParent,               					// ( )
    LBracket, RBracket,             					// [ ] 
    Dot, Comma, Semicolon,								// . , ;
};

struct Token {
    Token_Type type;
    std::string name;
    //std::vector<char> name;

    Token(Token_Type _type, std::string str) {
        type = _type;
        name = str;
    }

    Token() {
		type = None;
	}
	
	bool operator == (const Token_Type& type) const {
		return (this->type == type);
	}
	
    int get_num() {
        return std::stoi(name);
    }

	friend const char* operator + (const char* str, const Token& token) {
		static char result[100];
		sprintf(result, "%s \'%s\' token", str, token.name.c_str());
		return result;
	}

    void show() {
		std::string type_str;
		switch(type) {
			case None: type_str = "None"; break;
			case Ident: type_str = "Ident"; break;
			case Number: type_str = "Number"; break;

			case BEGIN: type_str = "BEGIN"; break;
			case END: type_str = "END"; break;
			case IF: type_str = "IF"; break;
			case THEN: type_str = "THEN"; break;
			case WHILE: type_str = "WHILE"; break;
			case DO: type_str = "DO"; break;
			case CALL: type_str = "CALL"; break;
			case ODD: type_str = "ODD"; break;
			case TO: type_str = "TO"; break;
			case CONST: type_str = "CONST"; break;
			case VAR: type_str = "VAR"; break;
			case PROCEDURE: type_str = "PROCEDURE"; break;
			case PROGRAM: type_str = "PROGRAM"; break;
			case ELSE: type_str = "ELSE"; break;
			case FOR: type_str = "FOR"; break;
			
			/*
			case WRITE: type_str = "WRITE"; break;
			case WRITELN: type_str = "WRITELN"; break;
			case READ: type_str = "READ"; break;
			case READLN: type_str = "READLN"; break;
			*/

			case Plus: type_str = "Plus"; break;
			case Percent: type_str = "Percent"; break;
			case Mul: type_str = "Mul"; break;
			case Slash: type_str = "Slash"; break;
			case Minus: type_str = "Minus"; break;
			case Assign: type_str = "Assign"; break;
			case Eq: type_str = "Eq"; break;
			case Ne: type_str = "Ne"; break;
			case Lt: type_str = "Lt"; break;
			case Gt: type_str = "Gt"; break;
			case Ge: type_str = "Ge"; break;
			case Le: type_str = "Le"; break;

			case LParent: type_str = "LParent"; break;
			case RParent: type_str = "RParent"; break;
			case LBracket: type_str = "LBracket"; break;
			case RBracket: type_str = "RBracket"; break;
			case Dot: type_str = "Dot"; break;
			case Comma: type_str = "Comma"; break;
			case Semicolon: type_str = "Semicolon"; break;

			default: break;
		}

		std::cout << type_str << " (" << name << ")\n";
   }
};


#endif

