#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#ifdef __cplusplus
#include <cstdbool>
#include <cstddef>
#else
#include <stdbool.h>
#include <stddef.h>
#endif

typedef struct List List;

//#if defined(__STDC__) || defined(__cplusplus)
#ifdef __cplusplus
extern "C" {
#endif

#include "minicoin_inst.h" //après typedef sinon erreur definition

List* List_new();
void List_free(const List *lst);

bool List_isEmpty(const List *lst);
size_t List_size(const List *lst);
//void printList(/*const*/ List *lst);

Instr* List_getFisrt(/*const*/ List *lst);
Instr* List_getLast(/*const*/ List *lst);
Instr* List_getNext(/*const*/ List *lst);

void List_insertBegin(List *lst, /*const*/ Instr *instr);
void List_insertEnd(List *lst, /*const*/ Instr *instr);

#ifdef __cplusplus
}
#endif
#endif // LIST_H_INCLUDED
