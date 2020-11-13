// 创建stack的方法，使用链表完成
// Created by dingmac on 2020/2/29.
//
#ifndef TEST1_MYSTACK_H
#define TEST1_MYSTACK_H
#include <stdbool.h>
//#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef char DataType;

typedef struct StackNode {
    DataType data;
    struct StackNode *next;//指向后面一个元素
}StackNode, *PMyStack;

//typedef struct MyStack{
//    PStackNode top;
//    int count;
//}Mystack, *PMyStack;

// 初始化stack
PStackNode initStack()
{
    PMyStack p = (PMyStack) malloc (sizeof(Mystack));
    p->next = NULL;
    return p;
}

// top和bottom指向同一个的时候为空，buttom不存储数据
bool isEmptyStack(PMyStack p)
{
    if (p->next== NULL) {
        return true;
    } else {
        return false;
    }
}

// 入的数据是char 类型
bool pushMyStack(PMyStack p, DataType a)
{
    StackNode* s = (StackNode*)malloc(sizeof(StackNode));
    // 这边分配失败会有报错
    s->data = a;
    s->next = p->next;
    p->next = s;
    return true;
}

bool popMyStack(PMyStack p, DataType *a)
{
    if(p->next == NULL) return false;
    StackNode * m = p->next;
    *a = m->data;
    p->next= m ->next;
    free(m);
    return true;
}

bool getTopMyStack(PMyStack p, DataType *a)
{
    if(p->next == NULL) return false;
    *a = p->next->data;
    return true;
}

#endif //TEST1_MYSTACK_H