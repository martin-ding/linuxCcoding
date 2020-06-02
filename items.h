//
// Created by dingmac on 2020/3/17.
//

#ifndef TEST1_ITEMS_H
#define TEST1_ITEMS_H
typedef struct BTreeNode{
    int data;
    struct BTreeNode *lchild,*rchild;
    int ltag,rtag;
}BtreeNode,*BTree;

//双亲表示法
typedef struct CNode1 {
    int data;
    int parent;//表示双亲的数组下标
}CNode1;

//双亲表示法
typedef struct TParent{
    CNode1 nodes[10];
    int count;
};

//孩子结点
typedef struct CNode{
    int child;//记录的是孩子结点数组的下标
    struct CNode * nextchild;//指向下一个孩子
}CNode;

typedef struct TNode {
    int data;
    int parent;//可以存储双亲结点，方便寻找双亲，其实就是将双亲表示法和孩子表示法融合了一下
    CNode *childs;//指向第一个孩子
}TNode;

//孩子表示法，类似于邻接表
typedef struct TreeChildren
{
    TNode nodes[10];
    int count;//有多少个结点
}TreeChildren;

#define NUM 10
//孩子兄弟表示法
typedef struct Tree {
    int data;
    struct Tree *firstChild, *nextChild;
}Tree;

#endif //TEST1_ITEMS_H
