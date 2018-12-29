#include "parser.h"
extern std::vector<Instruction> lInstructions;

int main(int argc, char** argv) {
    std::string source;
    if (argv[1]) source += argv[1];
    else {
        std::cout << "Input source: ";
        std::cin >> source;
    }

    /*
    Scanner s(source);
    while (s.current_char != '\0') s.getToken();
    */

    Parser parser(source);
    parser.program();
    std::cout << "Phan tich hoan tat!\n";
    std::cout << "\n=======================================================\n";
    std::cout << "Ma dich\n";

    // Code generator
    size_t nInstructions = lInstructions.size();
    for (size_t i = 0; i < nInstructions; i++) {
        if (nInstructions > 100) {
            if (i < 10) std::cout << "  " << i << ": ";    
            else if (i < 100) std::cout << " " << i << ": ";    
            else std::cout << i << ": ";    
        }
        else if (nInstructions > 10) {
            if (i < 10) std::cout << " " << i << ": ";    
            else std::cout << i << ": ";    
        } else std::cout << i << ": ";

        lInstructions[i].show();
    }

    std::cout << "\n===========================*===========================\n";

    return 0;
}
