#define MINICOIN_INST_C
#include "minicoin_inst.h"
#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>

#define PRINTTAB 2
#define CHARTAB '|'


void evalInstr(const Instr *instr) {
    instr->eval(instr);
}

void printInstr(const Instr *instr) {
    internPrint(instr, 0);
}

bool verifInstr(const Instr *instr /*, char *buf_msg*/) {
    return instr->check(instr);
}

void freeInstr(Instr **instr) {
    internFree(*instr);
    *instr = NULL;
}


void printMarge(const unsigned int sublvl) {
    unsigned int i;
    for(i=0 ; i < sublvl ; i++) {
        putchar(' ');
        putchar(CHARTAB);
        putchar(' ');
    }
}
