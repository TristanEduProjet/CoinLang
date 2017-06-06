#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "minicoin_tree.h"

int printDepth = 0;
int funcDepth = 0;

double evalExpr(const Node *node) {
	switch(node->type) {
        case NTEMPTY:  return 0.0;
        case NTNUM: return node->val;

        case NTPLUS: return evalExpr(node->children[0]) + evalExpr(node->children[1]);
        case NTMIN:  return evalExpr(node->children[0]) - evalExpr(node->children[1]);
        case NTMULT: return evalExpr(node->children[0]) * evalExpr(node->children[1]);
        case NTDIV:  return evalExpr(node->children[0]) / evalExpr(node->children[1]);
        case NTPOW:  return pow(evalExpr(node->children[0]), evalExpr(node->children[1]));

        default:
            fprintf(stderr, "Error in evalExpr ... Wrong node type: %s\n", node2String(node));
            exit(EXIT_FAILURE);
	};
}

void evalInst(const Node* node) {
	switch(node->type) {
        case NTEMPTY: break;
        case NTINSTLIST:
            evalInst(node->children[0]);
            evalInst(node->children[1]);
            break;
        case NTNUM:
        case NTPLUS:
        case NTMIN:
        case NTMULT:
        case NTDIV:
        case NTPOW:
            printf("%f\n", evalExpr(node));
            break;
        default:
            fprintf(stderr, "Error in evalInst ... Wrong node type: %s\n", node2String(node));
            exit(EXIT_FAILURE);
	};
}

inline void eval(const Node *node) {
	evalInst(node);
}
