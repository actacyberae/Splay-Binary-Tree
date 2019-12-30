#ifndef SBT_LIBRARY
#define SBT_LIBRARY

#include "./lib.h"

#define ER_EMPTYTREE -1
#define ER_EMPTYNODE -2
#define ER_UNKNWPRNT -3

#define TRUE 1
#define FALSE 0

typedef struct node *SBTptr;

typedef unsigned char tColor;

typedef struct node {
	int key;
	SBTptr left, right;
} SBTnode;

typedef struct SBT {
	int size;
	SBTptr root;
} SBT;

void Insert_node(SBT *pSBT, int pKey);

void tree_info(SBT *pSBT);

void Free_tree(SBT *pSBT);

void Free_node(SBT *pSBT, SBTptr pNode);

void Get_node(SBT *pSBT, int pKey, SBTptr *pNode);

#endif
