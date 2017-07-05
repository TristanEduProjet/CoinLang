#ifndef MINICOIN_INST_H_INCLUDED
#define MINICOIN_INST_H_INCLUDED

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum InstrType {
    IT_UNKNOWN = 0,
    IT_LIST,
    IT_AFFECT,
    IT_EXPR,
    IT_CALC,
    IT_LOGIC,
    IT_COMP,
    IT_TEST,
    IT_LOOP
} InstrType;

typedef struct Instr Instr;
typedef struct SessionEval SessionEval;

SessionEval* evalInit();
void evalFree(SessionEval **ptr);
void evalPrint(const SessionEval *session);

void evalInstr(const SessionEval *session, const Instr *instr);
void printInstr(const Instr *instr);
void freeInstr(Instr **instr);
bool verifInstr(const Instr *instr);


typedef struct InstrList InstrList;

InstrList* newInstrList();
//InstrList* newInstrListFrom(const unsigned int nb, ...);
void addInstrList(const InstrList *lst, const Instr *instr);


typedef enum DataType {
    DT_STRING = 0b00000001,
    DT_REAL   = 0b00000010,
    DT_INT    = 0b00000100,
    DT_BOOL   = 0b00001000
    #ifdef MINICOIN_INST_INTERN_H_INCLUDED
    , DT_NONE = 0b00000000
    , DT_ANY  = 0b11111111
    #endif //MINICOIN_INST_INTERN_H_INCLUDED
} DataType;
#define checkDataType(to_check, type) ((to_check) & (type))

typedef struct InstrExpr InstrExpr;

InstrExpr* newInstrExpr(const DataType type, ...);


typedef enum OperType {
    OP_PLUS = '+',
    OP_MIN = '-',
    OP_MULT = '*',
    OP_DIV = '/',
    OP_POW = '^'
} OperType;

typedef struct InstrCalc InstrCalc;

InstrCalc* newInstrCalc(const OperType type, const Instr *i1, const Instr *i2);


typedef enum LogicType {
    LT_NOT,
    LT_AND,
    LT_OR,
    LT_XOR
} LogicType;

typedef struct InstrLogic InstrLogic;

InstrLogic* newInstrLogic(const LogicType oper, const Instr *bi1, const Instr *bi2);


typedef enum ComparType {
    CT_EQ,
    CT_NEQ,
    CT_GT,
    CT_EGT,
    CT_LT,
    CT_ELT
} ComparType;

typedef struct InstrCompar InstrCompar;

InstrCompar* newInstrCompar(const ComparType oper, const Instr *i1, const Instr *i2);


typedef struct InstrAffect InstrAffect;

InstrAffect* newInstrAffect_Set(const char *varname, const Instr *value);
InstrAffect* newInstrAffect_Get(const char *varname);
//InstrAffect* newInstrAffect_GetCheck(const char *varname, const DataType type);


typedef struct InstrTest InstrTest;

InstrTest* newInstrTest(const Instr *cond, const Instr *yes, const Instr *no);
InstrTest* newInstrTestTern(const Instr *cond, const Instr *yes, const Instr *no);


typedef struct InstrLoop InstrLoop;

InstrLoop* newInstrLoop(const Instr *cond, const Instr *act);

#ifdef __cplusplus
}
#endif
#endif // MINICOIN_INST_H_INCLUDED
