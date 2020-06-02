//
// Created by dingmac on 2020/3/13.
//

#ifndef TEST1_MYLINKLIST_H
#define TEST1_MYLINKLIST_H

#include <stdlib.h>
#include <stdbool.h>

#define ElemType int
typedef struct MyLinkListNode{
    ElemType data;
    struct MyLinkListNode *pre,*next;
} MyLinkListNode,*MyLinkList;

//单链表头插法插入数据
bool List_HeadInsert(MyLinkList *L, ElemType elem) {
    MyLinkList ele = (MyLinkList)malloc(sizeof(MyLinkListNode));
    ele->data = elem;
    ele->next = (*L)->next;
    (*L)->next = ele;
    return false;
}

#endif //TEST1_MYLINKLIST_H
