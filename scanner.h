#ifndef SCANNER_H
#define SCANNER_H

#include <cstdio>
#include <stdlib.h>
#include "Token.h"

struct Scanner {
    char current_char;
	unsigned curr_line;
    char *buffer = NULL;
    int curr_cursor = 0;


    Scanner(std::string _src) {
        FILE* src;
		current_char = ' ';
		curr_line = 1;
		if (!(src = fopen(_src.c_str(), "r"))) {
			perror(_src.c_str());
			exit(EXIT_FAILURE);
		}


        long lSize;
        fseek(src, 0, SEEK_END);
        lSize = ftell(src);
        rewind(src);

        buffer = new char[lSize];
        fread(buffer, 1, lSize, src);

        fclose(src);

        std::cout << ' ' << curr_line <<  ": ";
    }

    ~Scanner() {
        if (buffer) {
            delete[] buffer;
            buffer = NULL;
        }
    }

	void error(const char*);
    char getCh();
    bool check_keyword(Token& token) const;
    char nextCh() {return buffer[curr_cursor++]; }
    Token getToken();
};

#endif
