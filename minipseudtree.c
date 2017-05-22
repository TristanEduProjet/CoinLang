#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "minipseudtree.h"

# define PRINTTAB 2

Node* createNode(int type) {
	Node* newnode = (Node *) malloc(sizeof(Node));
	newnode->type = type;
	newnode->children = NULL;
	return newnode;
}

Node* nodeChildren(Node* father, Node *child1, Node *child2) { 
	father->children = (Node **) malloc(sizeof(Node*) * 2);
	father->children[0] = child1;
	father->children[1] = child2;
	return father;
}

const char* node2String(Node *node) {	
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

const char* makeSpaces(int depth, int fdepth) {
	int nbspaces = depth * PRINTTAB;	
	char *spaces = (char *)malloc(sizeof(char) * nbspaces);
	if (depth == fdepth)		
		memset(spaces, ' ', nbspaces);
	else {
		int midspaces = fdepth * PRINTTAB;
		int endline = (depth - fdepth) * PRINTTAB - 1;
		memset(spaces, ' ', midspaces);
		spaces[midspaces] = '\\';
		char *tmpline =  (char *)malloc(sizeof(char) * endline);
		memset(tmpline, '_', endline);
		strcat(spaces, tmpline);
		free(tmpline);
	}
	return spaces;
}

void printGraphRecu(Node *node, int n) {
	
	int i;
	for ( i=0;i<n;i++)printf(" ");
	printf("%s\n", node2String(node));
	
	// Hack : No children only if null or number 
	if ((node->children != NULL) && 
		(node->type != NTNUM)  ) {
		printGraphRecu(node->children[0], n+1);
		printGraphRecu(node->children[1], n+1);
	}
}
void printGraphRec(Node *node, int depth, int fdepth) {
	printf("%s%s\n", makeSpaces(depth, fdepth), node2String(node));
	
	// Hack : No children only if null or number 
	if ((node->children != NULL) && 
		(node->type != NTNUM)  ) {
		printGraphRec(node->children[0], depth + 1, depth);
		printGraphRec(node->children[1], depth + 1, depth);
	}
}

void printGraph(Node * root) {
	printGraphRecu(root, 0);
}

