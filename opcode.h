#ifndef OP_CODE_H
#define OP_CODE_H
#include <iostream>

#define FRAME_REGISTER_NUM 4

enum OpCode {
    OP_LA,   // Load Address:
    OP_LV,   // Load Value:
    OP_LC,   // load Constant 
    OP_LI,   // Load Indirect 

    OP_INT,  // Increment t 
    OP_DCT,  // Decrement t
    OP_J,    // Jump 
    OP_FJ,   // False Jump 
    OP_HL,   // Halt 
    OP_ST,   // Store 

    OP_CALL, // Call
    OP_EP,   // Exit Procedure 
    OP_EF,   // Exit Function 

    OP_RC,   // Read Char 
    OP_RI,   // Read Integer
    OP_WRC,  // Write Char    
    OP_WRI,  // Write Int
    OP_WLN,  // WriteLN 

    OP_ADD,  // Add 
    OP_SUB,  // Substract
    OP_MUL,  // Multiple
    OP_DIV,  // Divide
    OP_NEG,  // Negative
    OP_ODD,  // Odd
    OP_MOD,  // Module

    OP_CV,   // Copy Top

    OP_EQ,   // Equal
    OP_NE,   // Not Equal
    OP_GT,   // Greater
    OP_LT,   // Less
    OP_GE,   // Greater or Equal
    OP_LE,   // Less or Equal

    OP_BP    // Break point.
};

struct Instruction {
    OpCode op;
    int p;
    int q;

    void show() {
        switch(op) {
            case OP_LA:
                std::cout << "LA    " << p << ", " << q << '\n';
                break;

            case OP_LV:
                std::cout << "LV    " << p << ", " << q << '\n';
                break;

            case OP_LC:
                std::cout << "LC    " << q << '\n';
                break;

            case OP_LI:
                std::cout << "LI\n";
                break;

            case OP_INT:
                std::cout << "INT   " << q << '\n';
                break;

            case OP_DCT:
                std::cout << "DCT   " << q << '\n';
                break;

            case OP_J:
                std::cout << "J    " << q << '\n';
                break;

            case OP_FJ:
                std::cout << "FJ    " << q << '\n';
                break;

            case OP_HL:
                std::cout << "HLT\n";
                break;

            case OP_ST:
                std::cout << "ST\n";
                break;

            case OP_CALL:
                std::cout << "CALL  " << p << ", " << q << '\n';
                break;

            case OP_EP:
                std::cout << "EP\n"; 
                break;

            case OP_EF:
                std::cout << "EF\n"; 
                break;

            case OP_RC:
                break;

            case OP_RI:
                break;

            case OP_WRC:
                break;

            case OP_WRI:
                break;

            case OP_WLN:
                break;

            case OP_ADD:
                std::cout << "ADD\n"; 
                break;

            case OP_SUB:
                std::cout << "SUB\n"; 
                break;
                
            case OP_MUL:
                std::cout << "MUL\n"; 
                break;
                
            case OP_DIV:
                std::cout << "DIV\n"; 
                break;
                
            case OP_NEG:
                std::cout << "NEG\n"; 
                break;
                
            case OP_ODD:
                std::cout << "ODD\n"; 
                break;
                
            case OP_MOD:
                std::cout << "MOD\n"; 
                break;
                
            case OP_CV:
                std::cout << "CV\n"; 
                break;
                
            case OP_EQ:
                std::cout << "EQ\n"; 
                break;
                
            case OP_NE:
                std::cout << "NE\n"; 
                break;
                
            case OP_GT:
                std::cout << "GT\n"; 
                break;
                
            case OP_LT:
                std::cout << "LT\n"; 
                break;
                
            case OP_GE:
                std::cout << "GE\n"; 
                break;
                
            case OP_LE:
                std::cout << "LE\n"; 
                break;
                
            case OP_BP:
                std::cout << "BP\n"; 
                break;

        }
    }
};

#endif
