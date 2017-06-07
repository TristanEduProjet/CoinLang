#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iso646.h>
#include "minicoin_tree.h"

# define PRINTTAB 2

Node* createNode(const int type) {
	Node* newnode = (Node *) malloc(sizeof(Node));
	newnode->type = type;
	newnode->children = NULL;
	return newnode;
}

Node* nodeChildren(Node* father, const Node *child1, const Node *child2) {
	father->children = (Node **) malloc(sizeof(Node*) * 2);
	father->children[0] = (Node*) child1;
	father->children[1] = (Node*) child2;
	return father;
}

const char* node2String(const Node *node) {
	char *res;
	switch ( node->type ) {
        case NTEMPTY:    return "NTEMPTY";
        case NTINSTLIST: return "NTINSTLIST";

        case NTNUM:
            res = (char *)malloc(sizeof(char) * 32);
            sprintf(res, "NTNUM -> %f", node->val);
            return res;

        case NTPLUS:  return "NTPLUS";
        case NTMIN:   return "NTMIN";
        case NTMULT:  return "NTMULT";
        case NTDIV:   return "NTDIV";
        case NTPOW:   return "NTPOW";

        default: return "UNK";
	};
}

const char* makeSpaces(const int depth, const int fdepth) {
	const int nbspaces = depth * PRINTTAB;
	char *spaces = (char*)malloc(sizeof(char) * nbspaces);
	if (depth == fdepth)
		memset(spaces, ' ', nbspaces);
	else {
		const int midspaces = fdepth * PRINTTAB;
		const int endline = (depth - fdepth) * PRINTTAB - 1;
		memset(spaces, ' ', midspaces);
		spaces[midspaces] = '\\';
		char *tmpline =  (char*)malloc(sizeof(char) * endline);
		memset(tmpline, '_', endline);
		strcat(spaces, tmpline);
		free(tmpline);
	}
	return spaces;
}

void printGraphRecu(const Node *node, int n) {
	int i;
	for(i=0 ; i<n ; i++)
        printf(" ");
	printf("%s\n", node2String(node));

	// Hack : No children only if null or number
	if ((node->children not_eq NULL) and (node->type not_eq NTNUM)) {
		printGraphRecu(node->children[0], n+1);
		printGraphRecu(node->children[1], n+1);
	}
}
void printGraphRec(const Node *node, const int depth, const int fdepth) {
	printf("%s%s\n", makeSpaces(depth, fdepth), node2String(node));
	// Hack : No children only if null or number
	if((node->children not_eq NULL) and (node->type not_eq NTNUM)) {
		printGraphRec(node->children[0], depth + 1, depth);
		printGraphRec(node->children[1], depth + 1, depth);
	}
}

void printGraph(const Node *root) {
	printGraphRecu(root, 0);
}
