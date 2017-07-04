#include "minicoin_inst_intern.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <iso646.h>

struct InstrCalc {
    struct Instr;
    OperType otype;
    Instr *i1, *i2;
};

void printInstrCalc(const Instr *instr, const unsigned int nbsp) {
    const InstrCalc *calc = (InstrCalc*) instr;
    printMarge(nbsp); putchar('['); putchar(calc->otype); putchar(']'); putchar('\n');
    internPrint(calc->i1, nbsp+1);
    internPrint(calc->i2, nbsp+1);
}

bool verifInstrCalc(const Instr *instr) {
    CheckInstrType(instr, IT_CALC);
    const InstrCalc *clc = (InstrCalc*) instr;
    return (clc->i1 not_eq NULL) or (internVerif(clc->i1)) or (clc->i2 not_eq NULL) or (internVerif(clc->i2)) or (clc->i1->type == clc->i2->type) or (clc->i1->retour == DT_REAL) or (clc->i2->retour == DT_REAL);
}

#define switch_i_dt(case_real, case_int) switch(calc->i1->retour) { \
                                            case DT_REAL: return newDataBean(DT_REAL, case_real); break; \
                                            case DT_INT:  return newDataBean(DT_INT, case_int); break; \
                                            default: ; /*silent warning*/ \
                                         }
DataBean evalInstrCalc(const SessionEval *session, const Instr *instr) {
    const InstrCalc *calc = (InstrCalc*) instr;
    switch(calc->otype) {
        case OP_PLUS: switch_i_dt(internEval(session, calc->i1).data.dbl + internEval(session, calc->i2).data.dbl,
                                  internEval(session, calc->i1).data.itg + internEval(session, calc->i2).data.itg);
            break;
        case OP_MIN: switch_i_dt(internEval(session, calc->i1).data.dbl - internEval(session, calc->i2).data.dbl,
                                 internEval(session, calc->i1).data.itg - internEval(session, calc->i2).data.itg);
            break;
        case OP_MULT: switch_i_dt(internEval(session, calc->i1).data.dbl * internEval(session, calc->i2).data.dbl,
                                  internEval(session, calc->i1).data.itg * internEval(session, calc->i2).data.itg);
            break;
        case OP_DIV: switch_i_dt(internEval(session, calc->i1).data.dbl / internEval(session, calc->i2).data.dbl,
                                 internEval(session, calc->i1).data.itg / internEval(session, calc->i2).data.itg);
            break;
        case OP_POW: switch_i_dt(pow(internEval(session, calc->i1).data.dbl, internEval(session, calc->i2).data.dbl),
                                 pow(internEval(session, calc->i1).data.itg, internEval(session, calc->i2).data.itg));
            break;
        default: return noResult; //n'arrivera pas
    }
    exit(EXIT_FAILURE); //mauvais type DT, ne devrais pas arriver !
    return noResult; //pour compilateur, respecttype retour
}
#undef switch_i_dt

void freeInstrCalc(/*const*/ Instr *instr) {
    const InstrCalc *calc = (InstrCalc*) instr;
    internFree(calc->i1);
    internFree(calc->i2);
    free(/*instr*/ calc);
}

InstrCalc* newInstrCalc(const OperType type, const Instr *i1, const Instr *i2) {
    MallocVerif(InstrCalc, calc);
    SetInstrBase(calc, IT_CALC, i1->retour, evalInstrCalc, freeInstrCalc, printInstrCalc, verifInstrCalc);
    calc->otype = type;
    calc->i1 = i1;
    calc->i2 = i2;
    return calc;
}


struct InstrExpr {
    struct Instr;
    DataBean val;
};

void printInstrExpr(const Instr *instr, const unsigned int nbsp) {
    const InstrExpr *expr = (InstrExpr*) instr;
    printMarge(nbsp);
    printDataBean(&(expr->val));
    putchar('\n');
}

bool verifInstrExpr(const Instr *instr) {
    CheckInstrType(instr, IT_EXPR);
    return true;
}

DataBean evalInstrExpr(const SessionEval *session, const Instr *instr) {
    const InstrExpr *expr = (InstrExpr*) instr;
    return expr->val;
}

void freeInstrExpr(/*const*/ Instr *instr) {
    const InstrExpr *expr = (InstrExpr*) instr;
    freeInternDataBean(&(expr->val));
    free(/*instr*/ expr);
}

InstrExpr* newInstrExpr(const DataType type, ...) {
    MallocVerif(InstrExpr, expr);
    SetInstrBase(expr, IT_EXPR, type, evalInstrExpr, freeInstrExpr, printInstrExpr, verifInstrExpr);
    va_list data;
    va_start(data, 1);
    expr->val = newDataBean_va(type, data);
    va_end(data);
    return expr;
}
