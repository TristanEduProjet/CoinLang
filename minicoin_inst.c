#include "minicoin_inst.h"
#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <klist.h>

#define __list_free(x)
typedef Instr* Instr_p;
KLIST_INIT(312, Instr_p, __list_free)

#define PRINTTAB 2
#define CHARTAB '|'

void printMarge(const unsigned int sublvl);
//static inline bool CheckInstrType(const Instr *instr, const InstrType type);
#define CheckInstrType(instr, ctype) if(instr->type != ctype) {fprintf(stderr, "Erreur %s(%p) n'est pas %s (%d != %d)\n", __func__, instr, #ctype, instr->type, ctype); return false;}
#define MallocVerif(type, name) type *name = malloc(sizeof(type)); if(name == NULL) { fputs("Erreur malloc(" #type ")", stderr); exit(EXIT_FAILURE); }
#define internPrint(var, nbsp) (var)->print(var, nbsp)
#define internCall(var, fn) ((var)->fn(var))
#define internFree(var) internCall(var, free)
#define internVerif(var) internCall(var, check)

struct Instr {
    InstrType type; //Type d'instruction
    DataType retour; //Type data retourn�e par instruction
    void (*eval)(const Instr*);
    void (*free)(/*const*/ Instr*);
    void (*print)(const Instr*, const unsigned int);
    bool (*check)(const Instr*); //true = erreur, false = ok
};

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

void evalInstrList(const Instr *instr) {
    /*const List *lst = ((InstrList*) instr)->list;
    Instr *i;
    for(i=List_getFisrt(lst) ; i not_eq NULL ; i=List_getNext(lst))
        evalInstr(i);*/
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

void evalInstrExpr(const Instr *instr) {
    ;
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
    return (clc->i1 not_eq NULL) or (internVerif(clc->i1)) or (clc->i2 not_eq NULL) or (internVerif(clc->i2)) or (clc->i1->type == clc->i2->type);
}

void evalInstrCalc(const Instr *instr) {
    ;
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

void evalInstrAffect(const Instr *instr) {
    ;
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


void printMarge(const unsigned int sublvl) {
    unsigned int i;
    for(i=0 ; i < sublvl ; i++) {
        putchar(' ');
        putchar(CHARTAB);
        putchar(' ');
    }
}
