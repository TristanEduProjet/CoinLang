#include "minicoin_inst_intern.h"
#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>

KHASH_MAP_INIT_STR(vars, DataBean)
/*
 * typedef struct kh_vars_s kh_vars_t
 * kh_vars_t *kh_init_vars(void)
 * void kh_destroy_vars(kh_vars_t *h)
 * void kh_clear_vars(kh_vars_t *h)
 * khint_t kh_get_vars(const kh_vars_t *h, kh_cstr_t key)
 * int kh_resize_vars(kh_vars_t *h, khint_t new_n_buckets)
 * khint_t kh_put_vars(kh_vars_t *h, kh_cstr_t key, int *ret)
 * void kh_del_vars(kh_vars_t *h, khint_t x)
 */

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

DataBean evalInstrAffect(const SessionEval *session, const Instr *instr) {
    const InstrAffect *affct = (InstrAffect*) instr;
    const khash_t(vars) *hm = session->variables;
    /*const*/ khint_t k = kh_get(vars, hm, affct->var);
    if(k == kh_end(hm)) {
        int ret;
        k = kh_put(vars, hm, affct->var, &ret); // add the key
    } else if(affct->expr->retour != kh_val(hm, k).type) {
        fprintf(stderr, "Erreur : variable \"%s\" de type %d et assignement %d\n", affct->var, kh_val(hm, k).type, affct->expr->retour);
        exit(EXIT_FAILURE);
    }
    kh_value(hm, k) = internEval(session, affct->expr); // set the value of the key
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
    SetInstrBase(aff, IT_AFFECT, DT_NONE, evalInstrAffect, freeInstrAffect, printInstrAffect, verifInstrAffect);
    aff->var = varname;
    aff->expr = value;
    return aff;
}

bool initSessionVars(/*const*/ SessionEval *session) {
    session->variables = kh_init(vars);
    return session->variables != NULL;
}

void printSessionVars(const SessionEval *session) {
    const khash_t(vars) *hm = session->variables;
    char *key;
    DataBean val;
    kh_foreach(hm, key, val, {
        putchar('\t');
        putchar('[');
        fputs(key, stdout);
        fputs("] -> ", stdout);
        printDataBean(&val);
        putchar('\n');
    })
    //khiter_t kit;
    //for (kit = kh_begin(hm); kit != kh_end(hm); ++kit) kh_key(hm, kit) kh_value(hm, kit)
    //else puts("#nothing#");
}

void freeSessionVars(SessionEval *session) {
    kh_destroy(vars, session->variables);
}
