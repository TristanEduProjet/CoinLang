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


#ifdef __cplusplus
}
#endif
#endif // MINICOIN_INST_H_INCLUDED
