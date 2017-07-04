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
    SetInstrBase(vbool, IT_LOGIC, DT_BOOL, evalInstrLogic, freeInstrLogic, printInstrLogic, verifInstrLogic);
    vbool->ltype = oper;
    vbool->i1 = bi1;
    vbool->i2 = bi2;
    return vbool;
}


struct InstrCompar {
    struct Instr;
    ComparType cmptype;
    Instr *ie1, *ie2;
};

#define _case_(ct, txt) case ct: fputs(txt, stdout); break
void printInstrCompar(const Instr *instr, const unsigned int nbsp) {
    const InstrCompar *cmp = (InstrCompar*) instr;
    printMarge(nbsp); putchar('[');
    switch(cmp->cmptype) {
        _case_(CT_EGT, ">=");
        _case_(CT_ELT, "<=");
        _case_(CT_EQ, "==");
        _case_(CT_GT, ">");
        _case_(CT_LT, "<");
        _case_(CT_NEQ, "!=");
        default: putchar('?');
    }
    putchar(']'); putchar('\n');
    internPrint(cmp->ie1, nbsp+1);
    internPrint(cmp->ie2, nbsp+1);
}
#undef _case_

bool verifInstrCompar(const Instr *instr) {
    CheckInstrType(instr, IT_COMP);
    const InstrCompar *cmp = (InstrCompar*) instr;
    return (cmp->ie1 not_eq NULL) and (internVerif(cmp->ie1))
        and (cmp->ie2) and (internVerif(cmp->ie2))
        and (cmp->ie1->retour == cmp->ie2->retour);
}

#define _tmp_(op, var) return newDataBean(DT_BOOL, internEval(session, cmp->ie1).data.var op internEval(session, cmp->ie2).data.var)
#define _case_(lt, op) case lt: switch(cmp->ie1->retour) { \
                                            case DT_INT:  _tmp_(op, itg); break;\
                                            case DT_REAL: _tmp_(op, dbl); break;\
                                            default: ; /*silent warning*/ \
                                        }\
                                        break
DataBean evalInstrCompar(const SessionEval *session, const Instr *instr) {
    const InstrCompar *cmp = (InstrCompar*) instr;
    switch(cmp->cmptype) {
        _case_(CT_EQ, ==);
        _case_(CT_NEQ, not_eq);
        _case_(CT_GT, >);
        _case_(CT_EGT, >=);
        _case_(CT_LT, <);
        _case_(CT_ELT, <=);
        default:
            fputs("Type nreco ...\n", stderr);
            exit(EXIT_FAILURE);
            return noResult; //pour type fonctio verif du compilateur
    }
    fputs("Type no autorise ...\n", stderr);
    exit(EXIT_FAILURE);
}
#undef _case_

void freeInstrCompar(/*const*/ Instr *instr) {
    const InstrCompar *cmp = (InstrCompar*) instr;
    internFree(cmp->ie1);
    internFree(cmp->ie2);
    free(cmp);
}

InstrCompar* newInstrCompar(const ComparType oper, const Instr *i1, const Instr *i2) {
    MallocVerif(InstrCompar, comp);
    SetInstrBase(comp, IT_COMP, DT_BOOL, evalInstrCompar, freeInstrCompar, printInstrCompar, verifInstrCompar);
    comp->cmptype = oper;
    comp->ie1 = i1;
    comp->ie2 = i2;
    return comp;
}
