#include "list.h"
#include <list>
#include <cstddef>

using namespace std;
/*
 * http://www.cplusplus.com/reference/forward_list/forward_list/
 * http://fr.cppreference.com/w/cpp/container/forward_list
 * http://www.geeksforgeeks.org/forward-list-c-set-1-introduction-important-functions/
 * - * - * - * - * - * - * - * _
 * http://www.cplusplus.com/reference/list/list/
 * http://www.cplusplus.com/reference/list/list/list/
 * https://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
 * - * - * - * - * - * - * - * _
 * http://www.cplusplus.com/articles/LACRko23/
 *
 */

 typedef struct List {
     list<Instr*> instructs;
     list<Instr*>::iterator it;
 } List;

extern "C" {
    Instr* List_getFisrt(/*const*/ List *lst) {
        lst->it = lst->instructs.begin();
        return *(lst->it);
    }

    Instr* List_getLast(/*const*/ List *lst) {
        lst->it = lst->instructs.end();
        return *(lst->it);
    }

    Instr* List_getNext(/*const*/ List *lst) {
        if(lst->it != lst->instructs.end())
            return *(++(lst->it));
        else
            return NULL;
    }

    void List_insertBegin(List *lst, /*const*/ Instr *instr) {
        if(instr != NULL)
            lst->instructs.push_front(instr);
    }

    void List_insertEnd(List *lst, /*const*/ Instr *instr) {
        if(instr != NULL)
            lst->instructs.push_back(instr);
    }

    List* List_new() {
        return new List();
    }

    void List_free(const List *lst) {
        delete lst;
    }

    bool List_isEmpty(const List *lst) {
        return lst->instructs.empty();
    }

    size_t List_size(const List *lst) {
        return lst->instructs.size();
    }

    /*void printList(/ *const* / List *lst) {
        for (list<Instr*>::iterator it = lst->instructs.begin(); it != lst->instructs.end(); it++)
            printInstr(*it);
    }*/

}
