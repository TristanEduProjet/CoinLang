#define MINICOIN_INST_C
#include "minicoin_inst.h"
#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>

struct InstrAffect {
    struct Instr;
    char *var;
    Instr *expr;
};

void printInstrAffect(const Instr *instr, const unsigned int nbsp) {
    const InstrAffect *aff = (InstrAffect*) instr;
    printMarge(nbsp); puts("Affectation");
    printMarge(nbsp+1); printf("Variable : %s\n", aff->var);
    printMarge(nbsp+1); puts("Valeur :");
    internPrint(aff->expr, nbsp+2);
}

bool verifInstrAffect(const Instr *instr) {
    CheckInstrType(instr, IT_AFFECT);
    const InstrAffect *affct = (InstrAffect*) instr;
    return (affct->var not_eq NULL) or (affct->expr not_eq NULL) or (internVerif(affct->expr));
}

IsrcResult evalInstrAffect(const Instr *instr) {
    //TODO: implements affectation var
    return noResult;
}

void freeInstrAffect(/*const*/ Instr *instr) {
    const InstrAffect *affct = (InstrAffect*) instr;
    free(affct->var);
    internFree(affct->expr);
    free(/*instr*/ affct);
}

InstrAffect* newInstrAffect(const char *varname, const Instr *value) {
    MallocVerif(InstrAffect, aff);
    aff->eval = evalInstrAffect;
    aff->print = printInstrAffect;
    aff->free = freeInstrAffect;
    aff->check = verifInstrAffect;
    aff->type = IT_AFFECT;
    aff->var = varname;
    aff->expr = value;
    return aff;
}
