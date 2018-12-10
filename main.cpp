// sai test var 
// check ident khac keyword
#include "parser.h"

extern std::vector<std::string> keywords;
extern Token_Type kw[];

int main(int argc, char** argv) {
    std::string source;
    if (argv[1]) source += argv[1];
    else {
        std::cout << "Input source: ";
        std::cin >> source;
    }

    /*
    Scanner s(source);
    while (s.current_char != '\0') s.getToken().show();
    return 0;
    */
    Parser parser(source);
    parser.program();
    std::cout << "Phan tich hoan tat!\n";

    return 0;
}
