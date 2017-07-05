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
    internEval(session, instr);
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

DataBean newDataBean_va(const DataType type, va_list av) {
    DataBean db;
    db.type = type;
    switch(type) {
        case DT_REAL:
            db.data.dbl = va_arg(av, double);
            break;
        case DT_INT:
            db.data.itg = va_arg(av, int);
            break;
        case DT_BOOL:
            //db.data.bln = (bool) va_arg(av, bool); //warn: '_Bool' is promoted to 'int' when passed through '...'
            db.data.bln = (bool) va_arg(av, int);    //if this code is reached, the program will abort
            break;
        case DT_STRING:
            db.data.str = va_arg(av, char*);
            break;
        case DT_NONE:
            db.data.none = 0;
            break;
        default:
            fputs("~~erreur: bean type non reconnu !", stderr);
            exit(EXIT_FAILURE);
            break;
    }
    return db;
}

DataBean newDataBean(const DataType type, ...) {
    va_list av;
    va_start(av, 1);
    DataBean tmp = newDataBean_va(type, av);
    va_end(av);
    return tmp;
}

void printDataBean(const DataBean *db) {
    switch(db->type) {
        case DT_REAL:
            printf("%lf", db->data.dbl);
            break;
        case DT_INT:
            printf("%i", db->data.itg);
            break;
        case DT_BOOL:
            printf("%s", db->data.bln ? "true" : "false");
            break;
        case DT_STRING:
            fputs(db->data.str, stdout);
            break;
        default:
            fputs("#type inconnu#", stdout);
            break;
    }
}

void freeInternDataBean(/*const*/ DataBean *bean)  {
    if(bean->type == DT_STRING) {
        free(bean->data.str);
        bean->data.str = NULL;
    }
}
