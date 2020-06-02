// 顺序表相关
// Created by dingmac on 2020/3/12.
//

#ifndef TEST1_MYLIST_H
#define TEST1_MYLIST_H

#include <stdbool.h>
#define ElemType int

typedef struct SqListNode {
    ElemType data[100];
    int length;
} SqListNode, * SqList;

// 删除Element
bool DeleteElement(SqList L,int i, int *e)
{
    if (i < 1 || i> L->length) {
        return false;
    }
    *e = L->data[i-1];//保存数据
    for(int m = i; m < L->length; m++){
        L->data[m-1] = L->data[m];
    }
    L->length--;
    return true;
};
#endif //TEST1_MYLIST_H
