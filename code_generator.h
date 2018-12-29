#ifndef code_generator_h
#define code_generator_h
#include "opcode.h"
#include <vector>

void update_load_instruction();
int getCurrentCodeAddress();
void update_J_label(int id, int label = 0);
void update_FJ_label(int id, int label = 0);
void set_proc_address(int *begin_addr);

void gen_LA_code(int depth, int offset);
void gen_LV_code(int depth, int offset);
void gen_LI_code();
void gen_LC_code(int q);

void gen_INT_code(int q); // INT q 
void gen_DCT_code(int q); // INT q

int gen_J_code(int label = 0); 
int gen_FJ_code(int label = 0); 

void gen_ST_code();
void gen_CALL_code(int base, int nextIns);
void gen_EP_code();     // Exit procedure
void gen_EF_code();     // Exit Function
void gen_HLT_code();    // End Program


void gen_RC_code();
void gen_RI_code();
void gen_WRC_code();
void gen_WRI_code();
void gen_WLN_code();

void gen_ADD_code();
void gen_SUB_code();
void gen_MUL_code();
void gen_DIV_code();
void gen_NEG_code();
void gen_CV_code();

void gen_EQ_code();
void gen_NE_code();
void gen_GT_code();
void gen_LT_code();
void gen_GE_code();
void gen_LE_code();
void gen_ODD_code();
void gen_MOD_code();

#endif
