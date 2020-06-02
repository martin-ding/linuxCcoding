// 简单Queue，使用链表实现
// Created by dingmac on 2020/3/1.
//

#ifndef TEST1_MYQUEUE_H
#define TEST1_MYQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "items.h"

typedef BTree EleType;
typedef struct MyQueueNode
{
    EleType data;
    struct MyQueueNode *next;
}MyQueueNode, *QueueNode;

typedef struct MyQueue
{
    QueueNode front, rear;
}MyQueue, *Queue;

// 初始化queue
void initQueue(Queue q)//这边犯了错
{
    q->front = q->rear = (QueueNode)malloc(sizeof(MyQueueNode));// 这边犯了错，*q->rear 有问题
    q->front->next = NULL;
}

// 插入队列
bool enQueue(Queue q, EleType a)
{
    QueueNode s = (QueueNode)malloc(sizeof(MyQueueNode));
    s->data = a;
    s->next = NULL;
    q->rear->next = s;
    q->rear = s;
}

// 插入队列
bool deQueue(Queue q, EleType *a)
{
    if (q->front == q->rear) {return false;}
    QueueNode p = q->front->next;
    q->front->next = p->next;
    if (p == q->rear) {
        q->rear = q->front;
    }
    *a = p->data;
    free(p);
    p = NULL;
    return true;
}

//判断是否为空queue
bool isEmptyQueue(MyQueue q)
{
    if (q.front == q.rear) {
        return true;
    } else {
        return false;
    }
}

#endif //TEST1_MYQUEUE_H
