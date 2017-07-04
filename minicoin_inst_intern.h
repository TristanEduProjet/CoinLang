#ifndef MINICOIN_INST_INTERN_H_INCLUDED
#define MINICOIN_INST_INTERN_H_INCLUDED

#include "minicoin_inst.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
//#include <inttypes.h>
#include <khash.h>

void printMarge(const unsigned int sublvl);
#define MallocVerif(type, name) type *name = malloc(sizeof(type)); \
                                if(name == NULL) { fputs("Erreur malloc(" #type ")", stderr); exit(EXIT_FAILURE); }

/*typedef union IsrcResult {
} IsrcResult;*/

typedef struct DataBean {
    DataType type;
    union {
        char *str;
        double dbl;
        int itg;
        uint8_t none;
    } data;
} DataBean;
void printDataBean(const DataBean *db);
#define CheckDataBeanType(databean, type) ((databean)->type == type)
//#define result(typ, res) ((IsrcResult){.typ = res})
DataBean result(const DataType type, ...);
//#define noResult (IsrcResult){.none = 0}
#define noResult result(DT_NONE, 0)

struct Instr {
    InstrType type; //Type d'instruction
    DataType retour; //Type data retournée par instruction
    DataBean (*eval)(const SessionEval*, const Instr*);
    void (*free)(/*const*/ Instr*);
    void (*print)(const Instr*, const unsigned int);
    bool (*check)(const Instr*); //true = erreur, false = ok
};
#define SetInstrBase(instr, type_, retour_, eval_, free_, print_, check_) \
            instr->type = type_; \
            instr->retour = retour_; \
            instr->eval = eval_; \
            instr->free = free_; \
            instr->print = print_; \
            instr->check = check_
//static inline bool CheckInstrType(const Instr *instr, const InstrType type);
#define CheckInstrType(instr, ctype) if(instr->type != ctype) {fprintf(stderr, "Erreur %s(%p) n'est pas %s (%d != %d)\n", __func__, instr, #ctype, instr->type, ctype); return false;}

#define internPrint(var, nbsp) (var)->print(var, nbsp)
#define internCall(var, fn) ((var)->fn(var))
#define internFree(var) internCall(var, free)
#define internVerif(var) internCall(var, check)
#define internEval(sess, var) ((var)->eval(sess, var))

typedef struct kh_vars_s kh_vars_t;
struct SessionEval {
    khash_t(vars) *variables;
};
bool initSessionVars(/*const*/ SessionEval *session);
void printSessionVars(const SessionEval *session);
void freeSessionVars(SessionEval *session);

#ifdef __cplusplus
}
#endif
#endif // MINICOIN_INST_INTERN_H_INCLUDED
