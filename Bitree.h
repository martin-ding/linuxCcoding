//
// Created by dingmac on 2020/3/1.
//
#ifndef TEST1_BITREE_H
#define TEST1_BITREE_H
#include "items.h"
#include "stdbool.h"

void addNode(BTree b,int m,int lorr);
void InOrder(BTree b);
void preOrder(BTree b);
void afterOrder(BTree b);
void levelTraverse(BTree b);
void example();
//生成中序线索二叉树
void InThread(BTree b, BTree * pre);
void ThreadMidBTree(BTree b);
void TraverThread(BTree b);
void TraverThread2(BTree b);
void bstDemo();
bool bstInsert(BTree *b, int k);
BTree bstSearch(BTree b,int k);
BTree bstSearch2(BTree b,int k);
void Initial(int s[]);
int Find(int s[], int m);
int Union(int s[], int Root1, int Root2);
bool bstDelete(BTree *b, int m);

#endif //TEST1_BITREE_H