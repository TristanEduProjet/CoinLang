#include "minicoin_inst_intern.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
    char *var; //!< variable key name
    bool from; //set instr ou get var
    Instr *expr; //from = false;
};

void printInstrAffect(const Instr *instr, const unsigned int nbsp) {
    const InstrAffect *aff = (InstrAffect*) instr;
    printMarge(nbsp); puts(aff->from ? "Lecture" : "Affectation");
    printMarge(nbsp+1); printf("Variable : %s\n", aff->var);
    if(not aff->from) {
        printMarge(nbsp+1); puts("Valeur :");
        internPrint(aff->expr, nbsp+2);
    }
}

bool verifInstrAffect(const Instr *instr) {
    CheckInstrType(instr, IT_AFFECT);
    const InstrAffect *affct = (InstrAffect*) instr;
    return (affct->var not_eq NULL)
        and (not affct->from ? (affct->expr not_eq NULL) and (internVerif(affct->expr)) : true);
}

DataBean evalInstrAffect(const SessionEval *session, const Instr *instr) {
    const InstrAffect *affct = (InstrAffect*) instr;
    const khash_t(vars) *hm = session->variables;
    /*const*/ khint_t k = kh_get(vars, hm, affct->var);
    bool not_new = true;
    if(k == kh_end(hm)) {
        if(affct->from) {
            fprintf(stderr, "Erreur : variable \"%s\" n'existe pas\n", affct->var);
            exit(EXIT_FAILURE);
        } else {
            int ret;
            k = kh_put(vars, hm, affct->var, &ret); // add the key
            not_new = false;
        }
    }
    if(affct->from) {
        if(/*affect->retour != kh_val(hm, k).type*/ false) {
            fprintf(stderr, "Erreur : variable \"%s\" de type %d et voulu de type %d\n", affct->var, kh_val(hm, k).type, affct->retour);
            exit(EXIT_FAILURE);
        }
        return kh_value(hm, k); // set the value of the key
    } else {
        if(not_new and (affct->expr->retour != kh_val(hm, k).type)) {
            fprintf(stderr, "Erreur : variable \"%s\" de type %d et assignement %d\n", affct->var, kh_val(hm, k).type, affct->expr->retour);
            exit(EXIT_FAILURE);
        }
        kh_value(hm, k) = internEval(session, affct->expr); // set the value of the key
        return noResult;
    }
}

void freeInstrAffect(/*const*/ Instr *instr) {
    const InstrAffect *affct = (InstrAffect*) instr;
    free(affct->var);
    if(affct->from)
        internFree(affct->expr);
    free(/*instr*/ affct);
}

InstrAffect* newInstrAffect_(const char *varname, const bool from, const Instr *value) {
    MallocVerif(InstrAffect, aff);
    SetInstrBase(aff, IT_AFFECT, from ? DT_ANY : DT_NONE, evalInstrAffect, freeInstrAffect, printInstrAffect, verifInstrAffect);
    aff->var = varname;
    aff->from = from;
    aff->expr = from ? NULL : value;
    return aff;
}
InstrAffect* newInstrAffect_Set(const char *varname, const Instr *value) {
    return newInstrAffect_(varname, false, value);
}
/*InstrAffect* newInstrAffect_From(const char *varname, const char *keyname) {
    return newInstrAffect_(varname, true, keyname);
}*/
InstrAffect* newInstrAffect_Get(const char *varname) {
    return newInstrAffect_(varname, true, NULL);
}

/*InstrAffect* newInstrAffect_GetCheck(const char *varname, const DataType type) {
}*/

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
