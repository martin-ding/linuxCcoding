// 创建stack的方法，使用链表完成
// Created by dingmac on 2020/2/29.
//
#ifndef TEST1_MYSTACK_H
#define TEST1_MYSTACK_H
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "items.h"

typedef BTree DataType;
typedef struct StackNode {
    DataType data;
    struct StackNode *next;//指向后面一个元素
}StackNode, *PStackNode;

typedef struct MyStack{
    PStackNode top;
    int count;
}Mystack, *PMyStack;

// 初始化stack
void initStack(PMyStack p)
{
    p->top = NULL;
    p->count = 0;
}

// top和bottom指向同一个的时候为空，buttom不存储数据
bool isEmptyStack(PMyStack p)
{
    if (p->top == NULL) {
        return true;
    } else {
        return false;
    }
}

// 入的数据是char 类型
bool pushMyStack(PMyStack p, DataType a)
{
    PStackNode s = (PStackNode)malloc(sizeof(StackNode));
    // 这边分配失败会有报错
    s->data = a;
    s->next = p->top;
    p->top = s;
    return true;
}

bool popMyStack(PMyStack p, DataType *a)
{
    if(p->top == NULL) return false;
    *a = p->top->data;
    PStackNode m = p->top;
    p->top = p->top->next;
    free(m);
    return true;
}

bool getTopMyStack(PMyStack p, DataType *a)
{
    if(p->top == NULL) return false;
    *a = p->top->data;
    return true;
}

void view(PStackNode p)
{
    printf("%d ",p->data);
}

#endif //TEST1_MYSTACK_H