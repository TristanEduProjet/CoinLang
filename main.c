#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>
#include <unistd.h> //process.h
#include <stdint.h>
#include <argtable3.h>
#include "minicoin_inst.h"

#ifndef VERSION
#define VERSION {unknow} //define in Makefile
#endif // VERSION

#define PARAM_SHOW_PARSER    0b00000001
#define PARAM_SHOW_INSTRUCTS 0b00000010
#define PARAM_SHOW_DUMP_VARS 0b00000100
#define PARAM_SHOW_DUMP_SESSION (PARAM_SHOW_DUMP_VARS)
#define PARAM_SHOW_ALL (PARAM_SHOW_PARSER | PARAM_SHOW_INSTRUCTS | PARAM_SHOW_DUMP_SESSION)

#define BIT_ON(var, flag) var or_eq flag
#define BIT_OFF(var, flag) var and_eq compl(flag)
#define BIT_TOGGLE(var, flag) var xor_eq flag
#define BIT_GET(var, flag) (var & (flag))

extern int yyparse();
extern FILE *yyin;

typedef struct Param {
    char *file_input;
    uint8_t verbosity;
} Param;

static inline Param parse_args(const int argc, const char *argv[]);
static inline Instr* parse_yy(const Param params);
static inline void exec_prog(const Param params, const Instr *root);
static inline void free_params(const Param param);

int main(const int argc, const char *argv[]) {
    printf("Process PID %d\n", getpid());
    const Param params = parse_args(argc, argv);
    const Instr *root = parse_yy(params);
    if(root != NULL) {
        if(BIT_GET(params.verbosity, PARAM_SHOW_INSTRUCTS)) {
            printf("\n>< Instructions :\n");
            printInstr(root);
        }
        exec_prog(params, root);
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
    struct arg_lit *help, *version, *level, *verb, *silent;
    struct arg_str *parser, *instrs, *session;
    struct arg_file *in, *out;
    struct arg_end *end;
    struct arg_rem *sr;
    /*const*/ void *argtable[] = {
        help    = arg_lit0("h", "help", "display this help and exit"),
        version = arg_lit0(NULL, "version",  "display version info and exit"),
        verb    = arg_lit0("v", "verbose", "verbose output"),
        silent  = arg_lit0(NULL, "silent", "silent output"),
        //parser  = arg_str0(NULL, "show-parser", "{y,yes,n,no}", "show parser output"),
        instrs  = arg_str0(NULL, "show-instructs", "{y,yes,n,no}", "show program obtain"),
        session = arg_str0(NULL, "dump-session", "{y,yes,n,no}", "dump session after execution"),
        sr      = arg_rem(NULL,                                  "  (show variables and functions)"),
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
        /*parser->sval[0] =*/ instrs->sval[0] = session->sval[0] = "y"; //par defaut
        const int nerrors = arg_parse(argc, argv, argtable);
        if(help->count > 0) { //special case: '--help' takes precedence over error reporting
            printf("Usage: %s", argv[0]);
            (verb->count > 0 ? arg_print_syntaxv : arg_print_syntax)(stdout, argtable, "\n");
            printf("Full command-line of %s.\n\n", argv[0]);
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
            //TODO: vérifier avec in->extension
            params.file_input = strdup(in->basename[0]);
        }
        {
            /* verbosité */
            //*parser, *instrs, *session;
            //if(parser->count >= 1)
            /*if(parser->sval[0][0] == 'y')
                BIT_ON(params.verbosity, PARAM_SHOW_PARSER);
            else
                BIT_OFF(params.verbosity, PARAM_SHOW_PARSER);*/
            //if(instrs->count >= 1)
            if(instrs->sval[0][0] == 'y')
                BIT_ON(params.verbosity, PARAM_SHOW_INSTRUCTS);
            else
                BIT_OFF(params.verbosity, PARAM_SHOW_INSTRUCTS);
            //if(session->count >= 1)
            if(session->sval[0][0] == 'y')
                BIT_ON(params.verbosity, PARAM_SHOW_DUMP_SESSION);
            else
                BIT_OFF(params.verbosity, PARAM_SHOW_DUMP_SESSION);
            if(verb->count > 0)
                BIT_ON(params.verbosity, PARAM_SHOW_ALL);
            /*else*/ if(silent->count > 1)
                BIT_OFF(params.verbosity, PARAM_SHOW_ALL);
                //params.verbosity and_eq compl(PARAM_SHOW_ALL);
        }
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0])); //deallocate each non-null entry in argtable[]
        return params;
    }
}

static inline Instr* parse_yy(const Param params) {
    printf("\n>- Preparation du parser :\n");
    /*const*/ FILE *fp = fopen(params.file_input, "r");
    if(fp == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier à executer.\n");
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

static inline void exec_prog(const Param params, const Instr *root) {
    printf("\n>- Preparation du programme :\n");
    if(not verifInstr(root)) {
        fprintf(stderr, "Erreur vérification interne\n");
        exit(EXIT_FAILURE);
    } else {
        SessionEval *se = evalInit();
        if(se == NULL) {
            fprintf(stderr, "Erreur initialisation interne\n");
            exit(EXIT_FAILURE);
        } else {
            printf("\n>< Execution du programme :\n");
            evalInstr(se, root);
            if(BIT_GET(params.verbosity, PARAM_SHOW_DUMP_SESSION)) {
                printf("\n>< Dump etat fin programme :\n");
                evalPrint(se);
            }
            printf("\n>- Nettoyage ...\n");
            evalFree(&se);
        }
    }
}

static inline void free_params(const Param param) {
    free(param.file_input);
}
