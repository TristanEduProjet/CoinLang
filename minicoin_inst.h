#ifndef MINICOIN_INST_H_INCLUDED
#define MINICOIN_INST_H_INCLUDED

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum InstrType {
    IT_UNKNOWN = 0,
    IT_LIST,
    IT_CALC,
    IT_AFFECT,
    IT_EXPR
} InstrType;

typedef struct Instr Instr;

void evalInstr(const Instr *instr);
void printInstr(const Instr *instr);
void freeInstr(Instr **instr);
bool verifInstr(const Instr *instr);


typedef struct InstrList InstrList;

InstrList* newInstrList();
//InstrList* newInstrListFrom(const unsigned int nb, ...);
void addInstrList(const InstrList *lst, const Instr *instr);


typedef enum DataType {
    DT_STRING,
    DT_REAL
} DataType;

typedef struct InstrExpr InstrExpr;

InstrExpr* newInstrExpr(const DataType type, void *data);


typedef enum OperType {
    OP_PLUS = '+',
    OP_MIN = '-',
    OP_MULT = '*',
    OP_DIV = '/',
    OP_POW = '^'
} OperType;

typedef struct InstrCalc InstrCalc;

InstrCalc* newInstrCalc(const OperType type, const Instr *i1, const Instr *i2);


typedef struct InstrAffect InstrAffect;

InstrAffect* newInstrAffect(const char *varname, const Instr *value);


#ifdef MINICOIN_INST_C
#include <stdint.h>
//#include <inttypes.h>

#define result(typ, res) ((IsrcResult){.typ = res})
//#define noResult (IsrcResult){.none = 0}
#define noResult result(none, 0)

void printMarge(const unsigned int sublvl);
//static inline bool CheckInstrType(const Instr *instr, const InstrType type);
#define CheckInstrType(instr, ctype) if(instr->type != ctype) {fprintf(stderr, "Erreur %s(%p) n'est pas %s (%d != %d)\n", __func__, instr, #ctype, instr->type, ctype); return false;}
#define MallocVerif(type, name) type *name = malloc(sizeof(type)); if(name == NULL) { fputs("Erreur malloc(" #type ")", stderr); exit(EXIT_FAILURE); }
#define internPrint(var, nbsp) (var)->print(var, nbsp)
#define internCall(var, fn) ((var)->fn(var))
#define internFree(var) internCall(var, free)
#define internVerif(var) internCall(var, check)
#define internEval(var) ((var)->eval(var))

typedef union IsrcResult {
    char *str;
    double dbl;
    uint8_t none;
} IsrcResult;

struct Instr {
    InstrType type; //Type d'instruction
    DataType retour; //Type data retournée par instruction
    IsrcResult (*eval)(const Instr*);
    void (*free)(/*const*/ Instr*);
    void (*print)(const Instr*, const unsigned int);
    bool (*check)(const Instr*); //true = erreur, false = ok
};
#endif // MINICOIN_INST_C

#ifdef __cplusplus
}
#endif
#endif // MINICOIN_INST_H_INCLUDED
