//
// Created by dingmac on 2020/3/17.
//
#include "Bitree.h"
#include "MyStack.h"
#include "MyQueue.h"

// 新增结点
void addNode(BTree b,int m,int lorr)
{
    BtreeNode *e = (BtreeNode*)malloc(sizeof(BtreeNode));
    e->data = m;
    e->lchild = e->rchild = NULL;
    if (lorr == 1) {
        b->lchild = e;
    } else {
        b->rchild = e;
    }
}

void InOrder(BTree b)
{
    Mystack p;
    initStack(&p);
    while (b || !isEmptyStack(&p)) {
        if (b) {
            pushMyStack(&p, b);//推入栈
            b = b->lchild;
        } else {
            popMyStack(&p,&b);
            view(b);
            b = b->rchild;
        }
    }
}

void preOrder(BTree b)
{
    Mystack p;
    initStack(&p);
    while (b || !isEmptyStack(&p)) {
        if (b) {
            view(b);
            pushMyStack(&p, b);//推入栈
            b = b->lchild;
        } else {
            popMyStack(&p,&b);
            b = b->rchild;
        }
    }
}
//后续遍历
void afterOrder(BTree b)
{
    Mystack p;
    BTree r;
    initStack(&p);
    while (b || !isEmptyStack(&p)) {
        if (b) {
            pushMyStack(&p, b);//推入栈
            b = b->lchild;
        } else {
            getTopMyStack(&p,&b);
            if (b->rchild && r != b->rchild) {
                b = b->rchild;
            } else {
                popMyStack(&p,&b);
                view(b);
                r = b;
                b = NULL;
            }
        }
    }
}

//层次遍历二叉树
void levelTraverse(BTree b)
{
    MyQueue q;
    initQueue(&q);
    enQueue(&q,b);
    while (!isEmptyQueue(q)) {
        deQueue(&q, &b);
        view(b);
        if (b->lchild) {
            enQueue(&q,b->lchild);
        }
        if (b->rchild) {
            enQueue(&q,b->rchild);
        }
    }
}

//生成中序线索二叉树
void InThread(BTree b, BTree *pre) {
    if (b) {
        InThread(b->lchild, pre);
        if (b->lchild == NULL) {
            b->lchild = *pre;
            b->ltag = 1;
        }else {
            b->ltag = 0;
        }
        if (*pre != NULL) {
            if ((*pre)->rchild == NULL) {
                (*pre)->rchild = b;
                (*pre)->rtag = 1;
            } else {
                (*pre)->rtag = 0;
            }
        }
        *pre = b;
        InThread(b->rchild, pre);
    }
}

//创建中序线索二叉树
void ThreadMidBTree(BTree b) {
    BTree pre = NULL;
    InThread(b,&pre);
}

//遍历中序线索二叉树
void TraverThread(BTree b) {
    while (b) {
        while (b->ltag != 1) {b = b->lchild;}
        printf("%d ", b->data);
        while (b->rtag == 1) {
            b = b->rchild;
            printf("%d ", b->data);
        }
        b = b->rchild;
    }
}

BTree firstNode(BTree b) {
    if (b) {
        while (b->ltag != 1) { b = b->lchild; }
        return b;
    } else {
        return NULL;
    }
}

BTree nextNode(BTree b) {
    // 表示是下一个
    if (b->rtag == 1) {
        return b->rchild;
    } else {
        return firstNode(b->rchild);
    }
}

//使用另外的方式遍历
void TraverThread2(BTree b) {
    for (BTree m = firstNode(b); m != NULL ; m = nextNode(m)) {
        printf("%d ", m->data);
    }
}

void example()
{
    BtreeNode b;
    b.data = 12;
    addNode(&b,21,1);
    addNode(&b,1,2);
    addNode(b.lchild,22,1);
    addNode(b.lchild,20,2);
    addNode(b.rchild,12,1);
    addNode(b.rchild,19,2);
    addNode(b.lchild->lchild,30,2);
    levelTraverse(&b);
    puts("");
    InOrder(&b);

    //线索中序二叉树，然后遍历
}

////////////并查集////////////
#define SIZE 10
int UFSet[SIZE];

void Initial(int s[]) {
    for (int i = 0; i < SIZE; ++i) {
        s[i] = -1;//初始化为-1表示独立的森林
    }
}

// 查找m下标对应结点的集合,即集合的根结点
int Find(int s[], int m) {
    while (s[m] >= 0) {
        m = s[m];//双亲的下标
    }
    return m;
}

// 增加集合
int Union(int s[], int Root1, int Root2)
{
    s[Root2] = Root1;//root2拜Root2为师
}

///////////// 二叉检索树 //////////

// 二叉检索树，搜索
BTree bstSearch(BTree b,int k)
{
    if (b == NULL) return NULL;
    if (b->data == k) {
        return b;
    }
    if (b->data < k) {
        b = bstSearch(b->rchild, k);
    } else {
        b = bstSearch(b->lchild, k);
    }
    return b;
}

BTree bstSearch2(BTree b,int k)
{
    //如果不是空
    while (b != NULL) {//这里不能是用 while(!b)方式
        if (b->data == k) {
            return b;
        } else {
            if (b->data > k) {
                b = b->lchild;
            } else {
                b = b->rchild;
            }
        }
    }
    return b;
}

//二叉搜索树插入
bool bstInsert(BTree *b, int k)
{
    if (*b != NULL) {
        if ((*b)->data == k) {
            return true;
        } else {
            if ((*b)->data < k) {
                bstInsert(&((*b)->rchild), k);
            } else {
                bstInsert(&((*b)->lchild), k);
            }
        }
    } else {
        (*b) = (BTree)malloc(sizeof(BtreeNode));
        (*b)->data = k;
        (*b)->lchild = (*b)->rchild = NULL;
    }
    return true;
}

//删除值是m的结点
bool bstDelete(BTree *b, int m) {
    if (*b == NULL) {//空树
        return false;
    } else {
        //先找到然后删除
        if ((*b)->data == m) {
            if ((*b)->lchild == NULL && (*b)->rchild == NULL) {
                *b = NULL;
            } else if ((*b)->lchild == NULL) {
                BTree p = *b;
                *b = (*b)->rchild;
                free(p);
                p = NULL;
            } else if ((*b)->rchild == NULL) {
                BTree p = *b;
                *b = (*b)->lchild;
                free(p);
                p = NULL;
            } else {// 既有左孩子又有右孩子
                BTree L = ((*b)->rchild);
                if (L->lchild == NULL) {//如果右孩子每有左孩子，那么其右孩子就是比b大的结点
                    BTree p = *b;
                    L->lchild = (*b)->lchild;
                    *b = L;
                    free(p);
                    p = NULL;
                } else {//右孩子还有左孩子，就找到最左边的孩子
                    BTree LL = b;//指向双亲结点
                    for (; L->lchild != NULL; LL = L, L = L->lchild);
                    //L就是b右子树最小的数
                    (*b)->data = L->data;
                    LL->lchild = L->rchild;
                    free(L);
                }
            }
            return true;
        } else if ((*b)->data > m) {
            return bstDelete(&((*b)->lchild), m);
        } else {
            return bstDelete(&((*b)->rchild), m);
        }
    }
}

//二叉搜索树的实例
void bstDemo() {
    BTree b = NULL;
    bstInsert(&b,1);
    bstInsert(&b,10);
    bstInsert(&b,100);
    bstInsert(&b,3);
}

