#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>
#include <unistd.h> //process.h
#include <argtable3.h>
#include "minicoin_inst.h"

#ifndef VERSION
#define VERSION {unknow} //define in Makefile
#endif // VERSION

extern int yyparse();
extern FILE *yyin;

typedef struct Param {
    char *file_input;
    bool verbose;
} Param;

static inline Param parse_args(const int argc, const char *argv[]);
static inline Instr* parse_yy(const Param params);
static inline void exec_prog(const Instr *root);
static inline void free_params(const Param param);

int main(const int argc, const char *argv[]) {
    printf("Process PID %d\n", getpid());
    const Param params = parse_args(argc, argv);
    const Instr *root = parse_yy(params);
    if(root != NULL) {
        printf("\n>< Instructions :\n");
        printInstr(root);
        exec_prog(root);
        freeInstr(&root);
        free_params(params);
        return EXIT_SUCCESS;
    } else
        return EXIT_FAILURE;
}

/*
 * http://www.argtable.org/tutorial/
 * https://linux.die.net/man/3/argtable
 */
static inline Param parse_args(const int argc, const char *argv[]) {
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

static inline Instr* parse_yy(const Param params) {
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

static inline void exec_prog(const Instr *root) {
    printf("\n>- Preparation du programme :\n");
    if(not verifInstr(root)) {
        fprintf(stderr, "Erreur v�rification interne\n");
        exit(EXIT_FAILURE);
    } else {
        SessionEval *se = evalInit();
        if(se == NULL) {
            fprintf(stderr, "Erreur initialisation interne\n");
            exit(EXIT_FAILURE);
        } else {
            printf("\n>< Execution du programme :\n");
            evalInstr(se, root);
            printf("\n>< Dump etat fin programme :\n");
            evalPrint(se);
            printf("\n>- Nettoyage ...\n");
            evalFree(&se);
        }
    }
}

static inline void free_params(const Param param) {
    free(param.file_input);
}
