#ifndef MINICOIN_TREE
#define MINICOIN_TREE

enum NodeType {
	NTEMPTY = 0,
	NTINSTLIST = 1,

	NTNUM   = 201,
	NTVAR = 222,

	NTPLUS  = 321,
	NTMIN   = 322,
	NTMULT  = 323,
	NTDIV   = 324,
	NTPOW   = 325,
	NTEQ = 326,
	NTAFF = 32
};

typedef struct Node {
	enum NodeType type;
	union {
		double val;
		char* var;
		struct Node** children;
	};
} Node;

Node* createNode(const int type);

Node* nodeChildren(Node* father, const Node *child1, const Node *child2);

const char* node2String(const Node *node);

inline void printGraph(const Node *root);

#endif
