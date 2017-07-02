#include "minicoin_inst_intern.h"
#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <math.h>

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

DataBean evalInstrCalc(const SessionEval *session, const Instr *instr) {
    const InstrCalc *calc = (InstrCalc*) instr;
    switch(calc->otype) {
        case OP_PLUS: return result(DT_REAL, internEval(session, calc->i1).data.dbl + internEval(session, calc->i2).data.dbl);
        case OP_MIN:  return result(DT_REAL, internEval(session, calc->i1).data.dbl - internEval(session, calc->i2).data.dbl);
        case OP_MULT: return result(DT_REAL, internEval(session, calc->i1).data.dbl * internEval(session, calc->i2).data.dbl);
        case OP_DIV:  return result(DT_REAL, internEval(session, calc->i1).data.dbl / internEval(session, calc->i2).data.dbl);
        case OP_POW:  return result(DT_REAL, pow(internEval(session, calc->i1).data.dbl, internEval(session, calc->i2).data.dbl));
        default: return noResult; //n'arrivera pas
    }
}

void freeInstrCalc(/*const*/ Instr *instr) {
    const InstrCalc *calc = (InstrCalc*) instr;
    internFree(calc->i1);
    internFree(calc->i2);
    free(/*instr*/ calc);
}

InstrCalc* newInstrCalc(const OperType type, const Instr *i1, const Instr *i2) {
    MallocVerif(InstrCalc, calc);
    calc->eval = evalInstrCalc;
    calc->print = printInstrCalc;
    calc->free = freeInstrCalc;
    calc->check = verifInstrCalc;
    calc->type = IT_CALC;
    calc->otype = type;
    calc->i1 = i1;
    calc->i2 = i2;
    return calc;
}


struct InstrExpr {
    struct Instr;
    DataType dtype;
    union {
        char *str;
        double dbl;
    };
};

void printInstrExpr(const Instr *instr, const unsigned int nbsp) {
    const InstrExpr *expr = (InstrExpr*) instr;
    printMarge(nbsp);
    switch(expr->dtype) {
        case DT_REAL:
            printf("%lf\n", expr->dbl);
            break;
        case DT_STRING:
            printf("var %s\n", expr->str);
            break;
        default:
            puts("Unknown ...");
    }
}

bool verifInstrExpr(const Instr *instr) {
    CheckInstrType(instr, IT_EXPR);
    return true;
}

DataBean evalInstrExpr(const SessionEval *session, const Instr *instr) {
    const InstrExpr *expr = (InstrExpr*) instr;
    switch(expr->dtype) {
        case DT_REAL:
            return result(DT_REAL, expr->dbl);
        case DT_STRING:
            return result(DT_STRING, expr->str);
        default:
            //TODO: ajouter sécurité pour arrêter programme
            return noResult; //ne devrais jamais arriver
    }
}

void freeInstrExpr(/*const*/ Instr *instr) {
    const InstrExpr *expr = (InstrExpr*) instr;
    if(expr->dtype == DT_STRING)
        free(expr->str);
    free(/*instr*/ expr);
}

InstrExpr* newInstrExpr(const DataType type, void *data) {
    MallocVerif(InstrExpr, expr);
    expr->type = IT_EXPR;
    expr->eval = evalInstrExpr;
    expr->print = printInstrExpr;
    expr->free = freeInstrExpr;
    expr->check = verifInstrExpr;
    expr->dtype = type;
    switch(type) {
        case DT_REAL:
            expr->dbl = *((double*) data);
            break;
        case DT_STRING:
            expr->str = (char*) data;
            break;
        default:
            fputs("Type Expression non reconnue", stderr);
            exit(EXIT_FAILURE);
    }
    return expr;
}
