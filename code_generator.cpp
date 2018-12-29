#include "code_generator.h"

std::vector<Instruction> lInstructions;

void update_load_instruction() {
    lInstructions.back().op = OP_LV;
}

int getCurrentCodeAddress() {
    return lInstructions.size();
}

void update_J_label(int id, int label) {
    lInstructions[id].q = label;
}

void update_FJ_label(int id, int label) {
    lInstructions[id].q = label;
}

void set_proc_address(int *begin_addr) {
    *begin_addr = lInstructions.size() - 1;
}

void gen_LA_code(int depth, int offset) {
    Instruction i;
    i.op = OP_LA;
    i.p = depth;
    i.q = offset + FRAME_REGISTER_NUM;

    lInstructions.push_back(i);
}

void gen_LV_code(int depth, int offset) {
    Instruction i;
    i.op = OP_LV;
    i.p = depth;
    i.q = offset + FRAME_REGISTER_NUM;

    lInstructions.push_back(i);
}

void gen_LI_code() {
    Instruction i;
    i.op = OP_LI;

    lInstructions.push_back(i);
}

void gen_LC_code(int q) {
    Instruction i;
    i.op = OP_LC;
    i.q = q;

    lInstructions.push_back(i);
}

void gen_INT_code(int q) {
    Instruction i;
    i.op = OP_INT;
    i.q = q;

    lInstructions.push_back(i);
}

void gen_DCT_code(int q) {
    Instruction i;
    i.op = OP_DCT;
    i.q = q;

    lInstructions.push_back(i);
}

int gen_J_code(int label) {
    Instruction i;
    i.op = OP_J;
    i.q = label;

    lInstructions.push_back(i);
    return lInstructions.size() - 1;
}

int gen_FJ_code(int label) {
    Instruction i;
    i.op = OP_FJ;
    i.q = label;

    lInstructions.push_back(i);
    return lInstructions.size() - 1;
}

void gen_ST_code() {
    Instruction i;
    i.op = OP_ST;

    lInstructions.push_back(i);
}
void gen_CALL_code(int base, int nextIns) {
    Instruction i;
    i.op = OP_CALL;
    i.p = base;
    i.q = nextIns;

    lInstructions.push_back(i);
}

void gen_EP_code() {
    Instruction i;
    i.op = OP_EP;

    lInstructions.push_back(i);
}

void gen_EF_code() {
    Instruction i;
    i.op = OP_EF;

    lInstructions.push_back(i);
}

void gen_HLT_code() {
    Instruction i;
    i.op = OP_HL;

    lInstructions.push_back(i);
}

void gen_RC_code() {
    Instruction i;
    i.op = OP_RC;

    lInstructions.push_back(i);
}

void gen_RI_code() {
    Instruction i;
    i.op = OP_RI;

    lInstructions.push_back(i);
}

void gen_WRC_code() {
    Instruction i;
    i.op = OP_WRC;

    lInstructions.push_back(i);
}

void gen_WRI_code() {
    Instruction i;
    i.op = OP_WRI;

    lInstructions.push_back(i);
}

void gen_WLN_code() {
    Instruction i;
    i.op = OP_WLN;

    lInstructions.push_back(i);
}

void gen_ADD_code() {
    Instruction i;
    i.op = OP_ADD;

    lInstructions.push_back(i);
}

void gen_SUB_code() {
    Instruction i;
    i.op = OP_SUB;

    lInstructions.push_back(i);
}

void gen_MUL_code() {
    Instruction i;
    i.op = OP_MUL;

    lInstructions.push_back(i);
}

void gen_DIV_code() {
    Instruction i;
    i.op = OP_DIV;

    lInstructions.push_back(i);
}

void gen_NEG_code() {
    Instruction i;
    i.op = OP_NEG;

    lInstructions.push_back(i);
}

void gen_CV_code() {
    Instruction i;
    i.op = OP_CV;

    lInstructions.push_back(i);
}

void gen_EQ_code() {
    Instruction i;
    i.op = OP_EQ;

    lInstructions.push_back(i);
}

void gen_NE_code() {
    Instruction i;
    i.op = OP_NE;

    lInstructions.push_back(i);
}

void gen_GT_code() {
    Instruction i;
    i.op = OP_GT;

    lInstructions.push_back(i);
}

void gen_LT_code() {
    Instruction i;
    i.op = OP_LT;

    lInstructions.push_back(i);
}

void gen_GE_code() {
    Instruction i;
    i.op = OP_GE;

    lInstructions.push_back(i);
}

void gen_LE_code() {
    Instruction i;
    i.op = OP_LE;

    lInstructions.push_back(i);
}

void gen_ODD_code() {
    Instruction i;
    i.op = OP_ODD;

    lInstructions.push_back(i);
}

void gen_MOD_code() {
    Instruction i;
    i.op = OP_MOD;

    lInstructions.push_back(i);
}
