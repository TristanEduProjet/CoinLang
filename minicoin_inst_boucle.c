#include "minicoin_inst_intern.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iso646.h>
#include <limits.h>

struct InstrLoop {
    struct Instr;
    Instr *condition, *action;
};

void printInstrLoop(const Instr *instr, const unsigned int nbsp) {
    const InstrLoop *loop = (InstrLoop*) instr;
    printMarge(nbsp); puts("Boucle");
    printMarge(nbsp+1); puts("Condition :");
    internPrint(loop->condition, nbsp+2);
    printMarge(nbsp+1); puts("Action(s) :");
    internPrint(loop->action, nbsp+2);
}

bool verifInstrLoop(const Instr *instr) {
    CheckInstrType(instr, IT_LOOP);
    const InstrLoop *loop = (InstrLoop*) instr;
    return (loop->condition not_eq NULL) and internVerif(loop->condition)
        and (loop->action not_eq NULL) and internVerif(loop->action);
}

DataBean evalInstrLoop(const SessionEval *session, const Instr *instr) {
    const InstrLoop *test = (InstrLoop*) instr;
    short i = 0;
    while((i < SHRT_MAX) and internEval(session, test->condition).data.bln) {
        internEval(session, test->action);
        i++;
    }
    return noResult;
}

void freeInstrLoop(Instr *instr) {
    const InstrLoop *loop = (InstrLoop*) instr;
    internFree(loop->condition);
    internFree(loop->action);
    free(loop);
}

InstrLoop* newInstrLoop(const Instr *cond, const Instr *act) {
    MallocVerif(InstrLoop, loop);
    SetInstrBase(loop, IT_LOOP, DT_NONE, evalInstrLoop, freeInstrLoop, printInstrLoop, verifInstrLoop);
    loop->condition = cond;
    loop->action = act;
    return loop;
}
