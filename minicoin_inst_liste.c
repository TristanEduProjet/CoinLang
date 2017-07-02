#include "minicoin_inst_intern.h"
#include <stdio.h>
#include <iso646.h>
#include <klist.h>

#define __list_free(x)
typedef Instr* Instr_p;
KLIST_INIT(312, Instr_p, __list_free)

struct InstrList {
    struct Instr;
    //List *list;
    klist_t(312) *lst;
};

void printInstrList(const Instr *instr, const unsigned int nbsp) {
    printMarge(nbsp); puts("<Liste>");
    const /*List*/klist_t(312) *lst = ((InstrList*) instr)->lst;
    kliter_t(312) *p;
    //Instr *i;
    for(p = kl_begin(/*list->*/lst); p != kl_end(/*list->*/lst); p = kl_next(p))
        internPrint(kl_val(p), nbsp+1);
}

bool verifInstrList(const Instr *instr) {
    CheckInstrType(instr, IT_LIST);
    bool res = false;
    const /*List*/klist_t(312) *lst = ((InstrList*) instr)->lst;
    kliter_t(312) *p;
    //Instr *i;
    for(p = kl_begin(lst); (not res) and (p != kl_end(lst)); p = kl_next(p))
        res or_eq internVerif(kl_val(p));
    return res;
}

DataBean evalInstrList(const SessionEval *session, const Instr *instr) {
    const /*List*/klist_t(312) *lst = ((InstrList*) instr)->lst;
    kliter_t(312) *p;
    for(p = kl_begin(lst); p != kl_end(lst); p = kl_next(p))
        internEval(session, kl_val(p));
    return noResult;
}

void freeInstrList(/*const*/ Instr *instr) {
    const InstrList *list = (InstrList*) instr;
    kliter_t(312) *p;
    for(p = kl_begin(list->lst); p != kl_end(list->lst); p = kl_next(p))
        internFree(kl_val(p));
    kl_destroy(312, list->lst);
    free(/*instr*/ list);
}

InstrList* newInstrList() {
    MallocVerif(InstrList, list);
    list->eval = evalInstrList;
    list->print = printInstrList;
    list->free = freeInstrList;
    list->check = verifInstrList;
    list->type = IT_LIST;
    //list->list = List_new();
    list->lst = kl_init(312);
    if(list->lst == NULL) {
        fputs("Erreur malloc(InstrList)", stderr);
        exit(EXIT_FAILURE);
    } else
        return list;
}

//InstrList* newInstrListFrom(const unsigned int nb, ...);

void addInstrList(const InstrList *lst, const Instr *instr) {
    //List_insertEnd(lst, instr);
    *kl_pushp(312, lst->lst) = instr;
}
