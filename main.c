#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>
//#include <unistd.h> //getpid
#include <argtable3.h>
#include "minicoin_inst.h"
#include "unorderedmap.h"
//#include "list.h"

//#define VERSION {unknow} //define in Makefile

extern int yyparse();
extern FILE *yyin;

typedef struct Param {
    char *file_input;
    bool verbose;
} Param;

inline Param parse_args(const int argc, const char *argv[]);
inline Instr* parse_yy(const Param params);
inline void exec_prog(const Instr *root);
inline void free_params(const Param param);

int main(const int argc, const char *argv[]) {
	/*klist_t(32) *kl;
	kliter_t(32) *p;
	kl = kl_init(32);
	*kl_pushp(32, kl) = 1;
	*kl_pushp(32, kl) = 5;
	*kl_pushp(32, kl) = 10;
	//kl_shift(32, kl, 0);
	int tmp;
	kl_shift(32, kl, &tmp);
	printf("-- %d\n", tmp);
	for (p = kl_begin(kl); p != kl_end(kl); p = kl_next(p))
		printf("%d\n", kl_val(p));
	kl_destroy(32, kl);
    exit(EXIT_SUCCESS);*/
    /*List *l = List_new();
    Instr *i;
    double d = 5.0;
    for(i=List_getFisrt(l) ; i not_eq NULL ; i=List_getNext(l))
        printf("l -> %x\n", i);
    putchar('\n');
    List_insertBegin(l, (Instr*)newInstrExpr(DT_REAL, &d));
    for(i=List_getFisrt(l) ; i not_eq NULL ; i=List_getNext(l))
        printf("l -> %x\n", i);
    putchar('\n');
    List_insertEnd(l, (Instr*)newInstrExpr(DT_STRING, NULL));
    for(i=List_getFisrt(l) ; i not_eq NULL ; i=List_getNext(l))
        printf("l -> %x\n", i);
    putchar('\n');
    List_insertEnd(l, NULL);
    for(i=List_getFisrt(l) ; i not_eq NULL ; i=List_getNext(l))
        printf("l -> %x\n", i);
    putchar('\n');
    exit(EXIT_SUCCESS);*/
    printf("Process PID %d\n", getpid());
    system("pause");
    const Param params = parse_args(argc, argv);
    const Instr *root = parse_yy(params);
    if(root != NULL) {
        printf("\n>< Instructions :\n");
        printInstr(root);
        exec_prog(root);
        //TODO: free instructs + list
        free_params(params);
        return EXIT_SUCCESS;
    } else
        return EXIT_FAILURE;
}

/*
 * http://www.argtable.org/tutorial/
 * https://linux.die.net/man/3/argtable
 */
inline Param parse_args(const int argc, const char *argv[]) {
    struct arg_lit *help, *version, *level, *verb;
    struct arg_file *in, *out;
    struct arg_end *end;
    /*const*/ void *argtable[] = {
        help    = arg_lit0("h", "help", "display this help and exit"),
        version = arg_lit0(NULL, "version",  "display version info and exit"),
        verb    = arg_lit0("v", "verbose", "verbose output"),
        //level   = arg_int0(NULL, "level", "<n>", "foo value"),
        //in      = arg_filen(NULL, NULL, "<file>", 1, 100, "input files"),
        //in      = arg_file0("i", "input,in", "<src>", "input files"),
        in      = arg_file1("fi", "file,input,in", "<src>", "source file"),
        //out     = arg_file0("o", "output,out", "results", "output file"),
        end     = arg_end(20)
    };
    if(arg_nullcheck(argtable) != 0) {
        fprintf(stderr, "error: insufficient memory\n");
        exit(EXIT_FAILURE);
    } else {
        const int nerrors = arg_parse(argc, argv, argtable);
        if(help->count > 0) { //special case: '--help' takes precedence over error reporting
            printf("Usage: %s", argv[0]);
            (verb->count > 0 ? arg_print_syntaxv : arg_print_syntax)(stdout, argtable, "\n");
            printf("Demonstrate command-line parsing in argtable3.\n\n");
            #if defined(__GNUC__) || defined(__GNUG__)
              arg_print_glossary_gnu(stdout, argtable);
            #else
              arg_print_glossary(stdout, argtable, "  %-25s %s\n");
            #endif
            exit(EXIT_SUCCESS);
        } else if(version->count > 0) {
            puts(VERSION);
            exit(EXIT_SUCCESS);
        } else if(nerrors > 0) {
            arg_print_errors(stdout, end, argv[0]); //Display the error details contained in the arg_end struct.
            printf("Try '%s --help' for more information.\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        Param params;
        if(in->count >= 1) {
            //TODO: v�rifier avec in->extension
            params.file_input = strdup(in->basename[0]);
        }
        params.verbose = verb->count > 0;
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0])); //deallocate each non-null entry in argtable[]
        return params;
    }
}

inline Instr* parse_yy(const Param params) {
    printf("\n>- Preparation du parser :\n");
    /*const*/ FILE *fp = fopen(params.file_input, "r");
    if(fp == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier � executer.\n");
        exit(EXIT_FAILURE);
    } else {
        yyin = fp;
        Instr *root = NULL;
        printf("\n>< Parsing :\n");
        const int res = yyparse(&root);
        fclose(fp);
        if(res == 0)
            return root;
        else {
            fprintf(stderr, "yyparse() return code %d\n", res);
            exit(res);
        }
    }
}

inline void exec_prog(const Instr *root) {
    printf("\n>- Preparation du programme :\n");
    const HashMap *variables = HashMap_new();
    if(variables == NULL) {
        fprintf(stderr, "Erreur initialisation interne\n");
        exit(EXIT_FAILURE);
    } else {
        printf("\n>< Execution du programme :\n");
        evalInstr(root);
        HashMap_free(variables);
    }
}

inline void free_params(const Param param) {
    free(param.file_input);
}
