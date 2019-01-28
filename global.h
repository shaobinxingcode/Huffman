#include <iostream>
#define code_Max 256
using namespace std;

int *countArray = (int*)malloc(sizeof(int)*256);

typedef struct hfmTreeNode{
	int symbol;
	struct hfmTreeNode *left;
	struct hfmTreeNode *right;
} hfmTreeNode, *phTreeNode;

typedef struct hHfmTreeNode{
	hfmTreeNode* rootNode;
} hHfmTreeNode;

typedef struct queueNode{
	phTreeNode ptr;
	int count;
	struct queueNode *next;
} queueNode, *ptrQueue;

typedef struct hQueueNode{
	int size;
	ptrQueue first;
} hQueueNode;

typedef struct tableNode{
	int symbol;
	char* code;
	struct tableNode *next;
} tableNode;

typedef struct hdTableNode{
	tableNode *first;
	tableNode *last;
} hdTableNode;


void initQueue(hQueueNode** hQueue){
    *hQueue=(hQueueNode*)malloc(sizeof(hQueueNode));
    (*hQueue)->size = 0;
    (*hQueue)->first = NULL;
}

void addQueueNode(hQueueNode **hQueue, hfmTreeNode *hNode, int count){
	queueNode *qNode = NULL;
	if((*hQueue)->size == 0){
		qNode = (queueNode*)malloc(sizeof(queueNode));
        (*hQueue)->first = qNode;
        qNode->count = count;
        qNode->ptr = hNode;
        qNode->next = NULL;
        (*hQueue)->size ++;
	}
	else{
		if((*hQueue)->first->count > count){
			qNode = (queueNode*)malloc(sizeof(queueNode));
			qNode->next = (*hQueue)->first;
			(*hQueue)->first = qNode;
			qNode->count = count;
			qNode->ptr = hNode;
			(*hQueue)->size ++;
		}
		else {
			queueNode* p = (*hQueue)->first;
			qNode = (queueNode*)malloc(sizeof(queueNode));
			qNode->count = count;
			qNode->ptr = hNode;
			(*hQueue)->size ++;

			while(p->next != NULL && count >= p->next->count)
				p = p->next;
			qNode->next = p->next;
			p->next = qNode;
		}
	}
}

hfmTreeNode* getHfmTreeNode(hQueueNode* hQueue){
	hfmTreeNode* getNode;
	getNode = hQueue->first->ptr;
	hQueue->first = hQueue->first->next;
	hQueue->size --;
	return getNode;
}

hHfmTreeNode* crtHfmTree(hQueueNode** hQueue){
	int count = 0;
	hfmTreeNode *left, *right;

	while((*hQueue)->size > 1){
		count = (*hQueue)->first->count + (*hQueue)->first->next->count;
		right = getHfmTreeNode(*hQueue);
		left = getHfmTreeNode(*hQueue);

		hfmTreeNode *newNode = (hfmTreeNode*)malloc(sizeof(hfmTreeNode));

		newNode->left = left;
		newNode->right = right;

		addQueueNode(hQueue,newNode,count);
	}

	hHfmTreeNode* tree = (hHfmTreeNode*)malloc(sizeof(hHfmTreeNode));
	tree->rootNode = getHfmTreeNode(*hQueue);
	return tree;
}

void traverseTree(hdTableNode** table, hfmTreeNode* tree, char* code, int k){
    if(tree->left==NULL && tree->right==NULL)  
    {
        code[k]='\0';  
        tableNode *tNode=(tableNode*)malloc(sizeof(tableNode)); 
        tNode->code=(char*)malloc(sizeof(char)*256+1);
        strcpy(tNode->code,code);
        tNode->symbol=tree->symbol;
        tNode->next=NULL;
 
        if((*table)->first==NULL)   
        {
            (*table)->first=tNode;
            (*table)->last=tNode;
        }
        else
        {
            (*table)->last->next=tNode;
            (*table)->last=tNode;
        }
    }
 
    if(tree->left!=NULL)    
    {
        code[k]='0';
        traverseTree(table,tree->left, code, k+1);
    }
 
    if(tree->right!=NULL)   
    {
        code[k]='1';
        traverseTree(table, tree->right, code, k+1);
    }	
}

hdTableNode*  crtTable(hHfmTreeNode* hfmTree){
	hdTableNode* hdTable = (hdTableNode*)malloc(sizeof(hdTableNode));
	hdTable->first = NULL;
	hdTable->last = NULL;
	char code[code_Max];
	int k = 0;

	traverseTree(&hdTable, hfmTree->rootNode, code, k);
	return hdTable;
}
