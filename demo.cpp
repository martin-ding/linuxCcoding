//
// Created by dingmac on 2020/11/9.
//

//#include "demo.h"

#include <iostream>
#include<cstdio>
#include <cstdlib>
using namespace std;

// BST的结点
typedef struct node
{
    int key;
    struct node *lChild, *rChild;
}Node, *BST;

// 在给定的BST插入element, 使之称为新的BST
bool BSTInsert(Node * &p, int element)
{
    if(NULL == p) // 空树
    {
        p = (Node*) malloc (sizeof(Node));
        p->key = element;
        p->lChild = p->rChild = NULL;
        return true;
    }

    if(element == p->key) // BST中不能有相等的值
        return false;

    if(element < p->key)  // 递归
        return BSTInsert(p->lChild, element);

    return BSTInsert(p->rChild, element); // 递归
}

// 建立BST
void createBST(Node * &T, int a[], int n)
{
    T = NULL;
    int i;
    for(i = 0; i < n; i++)
    {
        BSTInsert(T, a[i]);
    }
}

// BST的查找（递归）
//Node *BSTSearch1(BST T, int x) {
////    if(NULL == T)
////        return NULL;
//
////    if(x == T->key)
////        return T;
//    if (T) {
//        if (x < T->key)
//            T = BSTSearch(T->lChild, x);
//        else if (x > T->key) {
//            T = BSTSearch(T->rChild, x);
//        }
//    }
//    return T;
////    return BSTSearch(T->rChild, x);
//}

Node * BSTSearch(BST T, int x) {
    while (T && T->key != x) {
        if (T->key > x)
            T = T->lChild;
        else
            T = T->rChild;
    }
    return T;
}

// 前序遍历
void preOrder(BST T) {
    if (T != NULL) {
        printf("%d\t", T->key);
        preOrder(T->lChild);
        preOrder(T->rChild);
    }
}
BST *rightminChild(BST &T);
// 删除
int remove(BST &T, int x) {
    if (T == NULL) return -1;
    if (T->key > x) {
        return remove(T->lChild, x);
    } else if (T->key < x) {
        return remove(T->rChild, x);
    }
    BST temp = NULL;
    printf("\n");
    // 找到了
    if (T->lChild == NULL) {
        temp = T;
        T = T->rChild;//直接修改
        free(temp);
    } else if (T->rChild == NULL) {
        temp = T;
        T = T->lChild;
        free(temp);
    } else {
        // 都有孩子结点，删除左子树中的最小结点
        BST *min = rightminChild(T->rChild);
        printf("\n");
        T->key = (*min)->key;
        temp = *min;
        (*min) = (*min)->rChild;
        free(temp);
    }
    return 1;
}

// 最小结点和需要调整的结点
BST * rightminChild(BST &T) {
    if (T->lChild == NULL) {
        return &T;
    } else {//从左子树中
        return rightminChild(T->lChild);
    }
}

int main()
{
    int a[10] = {53,17,9,45,23,78,65,94,81,88};
    int n = 10;

    BST T = NULL;

    // 并非所有的a[]都能构造出BST,所以，最好对createBST的返回值进行判断
    createBST(T, a, n);
    preOrder(T);
    remove(T, 53);
    preOrder(T);
//
//    Node *p = NULL;
//
//    int x; // 待查找的x
//    for(x = -10; x < 20; x++)
//    {
//        p = BSTSearch(T, x);
//        cout << x << ":";
//        if(NULL == p)
//            cout << "no" << endl;
//        else
//            cout << "yes" << endl;
//    }

    return 0;
}