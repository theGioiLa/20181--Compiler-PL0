#include "scanner.h"

std::vector<std::string> keywords = {
    "BEGIN", "END", "IF", "THEN", "WHILE", "DO", "CALL", "ODD", "TO", 
    "CONST", "VAR", "PROCEDURE", "PROGRAM", "ELSE", "FOR"
//  "WRITE", "WRITELN", "READ", "READLN"
};

Token_Type kw[] = {
    BEGIN, END, IF, THEN, WHILE, DO, CALL, ODD, TO,
    CONST, VAR, PROCEDURE, PROGRAM, ELSE, FOR
//  WRITE, WRITELN, READ, READLN
};


void Scanner::error(const char* msg) {
	std::cerr << "[ERROR] Loi token tai dong " << curr_line << ": " << msg << std::endl;
	exit(1);
}

char Scanner::getCh() {
	if (current_char == '\n') {
        curr_line++;
        if (curr_line < 10) std::cout << ' ' << curr_line << ": ";
        else std::cout << curr_line << ": ";
    }

	current_char = nextCh();
    std::cout << current_char;

    return current_char;
}

bool Scanner::check_keyword(Token& token) const {
    size_t nKeywords = keywords.size();
	
    for (size_t i = 0; i < nKeywords; i++) {
        if (token.name == keywords[i]) {
            token.type = kw[i];
            return true;
        }
    }

    return false;
}

Token Scanner::getToken() {
    Token token;
    token.name.clear();

    while (current_char == ' ' || current_char == '\n' || current_char == '\t' || current_char == '\r') getCh();

    switch(current_char) {
        //------------ + - * / % -----------
        case '+': 
            getCh();
            return Token(Plus, "+");

        case '-': 
            getCh();
            return Token(Minus, "-");

        case '*': 
            getCh();
            return Token(Mul, "*");

        case '/': 
			getCh();
			return Token(Slash, "/");

        case '%': 
            getCh();
            return Token(Percent, "%");


        //------------> < <> = := >= <= -----------
        case '>':
            token.name.push_back('>');
            token.type = Gt;
            getCh();
            if (current_char == '=') {
                token.name.push_back('=');
                token.type = Ge;
                getCh();
            }

            return token;

        case '<':
            token.name.push_back('<');
            token.type = Lt;
            getCh();
			
            if (current_char == '=') {
                token.name.push_back('=');
                token.type = Le;
                getCh();
            } else if (current_char == '>') {
                token.name.push_back('>');
                token.type = Ne;
                getCh();
            }

            return token;

        case '=':
            token.name.push_back('=');
            token.type = Eq;
            getCh();

            return token;

        //------------Assign Operator/ Semicolon -----------
        case ':':
            token.name.push_back(':');
            token.type = None;
            getCh();
			
            if (current_char == '=') {
                token.name.push_back('=');
                token.type = Assign;
                getCh();
            } else error("Unrecognized \':\' token");

            return token;

        //------------Special Token-----------
        case ';':
            token.name.push_back(';');
            token.type = Semicolon;
            getCh();

            return token;

        case '.':
            token.name.push_back('.');
            token.type = Dot;
            getCh();

            return token;

        case ',':
            token.name.push_back(',');
            token.type = Comma;
            getCh();

            return token;

        case '(':
            token.name.push_back('(');
            token.type = LParent;
            getCh();
			
			if (current_char == '*') {
				bool end = false;
				while (!end) {
					// Check pattern "*)"
					getCh();
					if (current_char == '*' && getCh() == ')') end = true;
				}
				
				token.type = None;
				token.name.push_back(current_char);
				
				getCh();
			}

            return token;

        case ')':
            token.name.push_back(')');
            token.type = RParent;
            getCh();

            return token;

        case '[':
            token.name.push_back('[');
            token.type = LBracket;
            getCh();

            return token;

        case ']':
            token.name.push_back(']');
            token.type = RBracket;
            getCh();

            return token;

        default: break;
    }


    //----------- Number --------------
    if (std::isdigit(current_char)) {
		int length = 0;
        while (std::isdigit(current_char)) {
			if (length < MAX_LENGTH_NUMBER) {
				token.name.push_back(current_char);
				length++;
			} else error("Overflow number, limited to 9 digit");
            getCh();
        }

        token.type = Number;
        return token;
    }

    //----------- Ident --------------
    if (std::isalpha(current_char)) {
		int length = 0;
        while (std::isalpha(current_char)) {
			if (length < MAX_LENGTH_IDENT) {
                token.name += toupper(current_char);
				length++;
			}
			
            getCh();
        }

        token.type = Ident;
        if (std::isdigit(current_char)) {
            while (std::isdigit(current_char) || std::isalpha(current_char)) {
				if (length < MAX_LENGTH_IDENT) {
                    token.name += toupper(current_char);
					length++;
				} else error("Overflow identifier, limited to 10");
				
                getCh();
            }
			
        } else {
            check_keyword(token);
        }

        return token;
    }

    token.type = None;
    token.name.push_back(current_char);
	char msg[50];
	if (current_char != '\0') {
		sprintf(msg, "Khong ton tai '%c' token", current_char);
		error(msg);
		getCh();
	} else {
		token.type = Eof;
		token.name.clear();
	}

    return token;
}

