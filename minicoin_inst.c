#include "minicoin_inst_intern.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iso646.h>

#define PRINTTAB 2
#define CHARTAB '|'


SessionEval* evalInit() {
    SessionEval *se = malloc(sizeof(SessionEval));
    if(se == NULL) {
        fputs("Erreur malloc(SessionEval)", stderr);
    } else {
        if(!initSessionVars(se))
            fputs("Erreur new HashMap vars", stderr);
        else
            return se;
    }
    return NULL;
}

void evalFree(SessionEval **ptr) {
    freeSessionVars(*ptr);
    free(*ptr);
    *ptr = NULL;
}

void evalPrint(const SessionEval *session) {
    puts("Variables :");
    printSessionVars(session);
}

void evalInstr(const SessionEval *session, const Instr *instr) {
    instr->eval(session, instr);
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


DataBean result(const DataType type, ...) {
    va_list av;
    va_start(av, 1);
    DataBean db;
    db.type = type;
    switch(type) {
        case DT_REAL:
            db.data.dbl = va_arg(av, double);
            break;
        case DT_INT:
            db.data.itg = va_arg(av, int);
            break;
        case DT_STRING:
            db.data.str = va_arg(av, char*);
            break;
        case DT_NONE:
            db.data.none = 0;
            break;
        default:
            fputs("~~erreur: bean type non reconnu !", stderr);
            break;
    }
    va_end(av);
    return db;
}

void printDataBean(const DataBean *db) {
    switch(db->type) {
        case DT_REAL:
            printf("%lf", db->data.dbl);
            break;
        case DT_INT:
            printf("%i", db->data.itg);
            break;
        case DT_STRING:
            fputs(db->data.str, stdout);
            break;
        default:
            fputs("#type inconnu#", stdout);
            break;
    }
}
