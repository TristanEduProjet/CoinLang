#include "minicoin_inst_intern.h"
#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>

struct InstrLogic {
    struct Instr;
    LogicType ltype;
    Instr *i1, *i2;
};

void printInstrLogic(const Instr *instr, const unsigned int nbsp) {
    const InstrLogic *lgc = (InstrLogic*) instr;
    printMarge(nbsp); putchar('[');
    switch(lgc->ltype) {
        case LT_AND: fputs("and", stdout); break;
        case LT_NOT: fputs("not", stdout); break;
        case LT_OR:  fputs("or", stdout); break;
        case LT_XOR: fputs("xor", stdout); break;
        default: //ne devrais jamais arriver
            fputs("Opérande non reconnue ...\n", stderr);
            exit(EXIT_FAILURE);
    }
    putchar(']'); putchar('\n');
    internPrint(lgc->i1, nbsp+1);
    if(lgc->ltype != LT_NOT) internPrint(lgc->i2, nbsp+1);
}

bool verifInstrLogic(const Instr *instr) {
    CheckInstrType(instr, IT_EXPR);
    return false;
}

#define _case_(lt, op) case lt: return newDataBean(DT_BOOL, internEval(session, lgc->i1).data.bln op internEval(session, lgc->i2).data.bln)
DataBean evalInstrLogic(const SessionEval *session, const Instr *instr) {
    const InstrLogic *lgc = (InstrLogic*) instr;
    switch(lgc->ltype) {
        case LT_NOT:
            return newDataBean(DT_BOOL, not internEval(session, lgc->i1).data.bln);
        _case_(LT_AND, and);
        _case_(LT_OR, or);
        _case_(LT_XOR, xor);
        default:
            fputs("Type manquant ...\n", stderr);
            exit(EXIT_FAILURE);
            return noResult; //pour type fonctio verif du compilateur
    }
}
#undef _case_

void freeInstrLogic(/*const*/ Instr *instr) {
    const InstrLogic *lgc = (InstrLogic*) instr;
    internFree(lgc->i1);
    internFree(lgc->i2);
    free(instr);
}

InstrLogic* newInstrLogic(const LogicType oper, const Instr *bi1, const Instr *bi2) {
    MallocVerif(InstrLogic, vbool);
    SetInstrBase(vbool, IT_EXPR, DT_BOOL, evalInstrLogic, freeInstrLogic, printInstrLogic, verifInstrLogic);
    vbool->ltype = oper;
    vbool->i1 = bi1;
    vbool->i2 = bi2;
    return vbool;
}
