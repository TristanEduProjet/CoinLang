#include "minicoin_inst_intern.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iso646.h>

struct InstrTest {
    struct Instr;
    Instr *condition, *if_true, *if_false;
};

void printInstrTest(const Instr *instr, const unsigned int nbsp) {
    const InstrTest *test = (InstrTest*) instr;
    printMarge(nbsp); puts("Test");
    printMarge(nbsp+1); puts("Condition :");
    internPrint(test->condition, nbsp+2);
    printMarge(nbsp+1); puts("Alors :");
    internPrint(test->if_true, nbsp+2);
    if(test->if_false not_eq NULL) {
        printMarge(nbsp+1); puts("Sinon :");
        internPrint(test->if_false, nbsp+2);
    }
}

bool verifInstrTest(const Instr *instr) {
    CheckInstrType(instr, IT_TEST);
    const InstrTest *test = (InstrTest*) instr;
    return (test->condition not_eq NULL) and internVerif(test->condition)
        and (test->if_true) and internVerif(test->if_true)
        and ((test->if_false not_eq NULL) ? internVerif(test->if_false) : true);
}

bool verifInstrTestTern(const Instr *instr) {
    CheckInstrType(instr, IT_TEST);
    const InstrTest *test = (InstrTest*) instr;
    return (test->condition not_eq NULL) and internVerif(test->condition)
        and (test->if_true) and internVerif(test->if_true)
        and (test->if_false not_eq NULL) and internVerif(test->if_false)
        and (test->if_true not_eq NULL) and (test->if_true->retour == test->if_false->retour);
}

DataBean evalInstrTest(const SessionEval *session, const Instr *instr) {
    const InstrTest *test = (InstrTest*) instr;
    if(internEval(session, test->condition).data.bln)
        internEval(session, test->if_true);
    else if(test->if_false not_eq NULL)
        internEval(session, test->if_false);
    return noResult;
}

DataBean evalInstrTestTern(const SessionEval *session, const Instr *instr) {
    const InstrTest *test = (InstrTest*) instr;
    DataBean tmp = (internEval(session, test->condition).data.bln)
            ? internEval(session, test->if_true)
            : internEval(session, test->if_false);
    printDataBean(&tmp);
    return tmp;
}

void freeInstrTest(/*const*/ Instr *instr) {
    const InstrTest *test = (InstrTest*) instr;
    internFree(test->condition);
    internFree(test->if_true);
    if(test->if_false not_eq NULL)
        internFree(test->if_false);
    free(/*instr*/ test);
}

InstrTest* newInstrTest(const Instr *cond, const Instr *yes, const Instr *no) {
    MallocVerif(InstrTest, test);
    SetInstrBase(test, IT_TEST, DT_NONE, evalInstrTest, freeInstrTest, printInstrTest, verifInstrTest);
    test->condition = cond;
    test->if_true = yes;
    test->if_false = no;
    return test;
}

InstrTest* newInstrTestTern(const Instr *cond, const Instr *yes, const Instr *no) {
    InstrTest* test = newInstrTest(cond, yes, no);
    test->retour = yes->retour;
    test->eval = evalInstrTestTern;
    test->check = verifInstrTestTern;
    return test;
}
