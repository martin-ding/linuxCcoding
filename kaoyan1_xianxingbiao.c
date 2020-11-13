#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 10

//单链表
typedef struct {
    int age;
    char name[20];
} Std;

typedef struct {
    Std data[M];
    int length;
} Sqlist;

//双链表
typedef struct DNode{
    int data;
    struct DNode *prior, *next;
}DNode, *DLinklist;

int initList(Sqlist *list);

int listInsert(Sqlist *list,int len, Std std);
DLinklist initDLinkList();
void scanList(DLinklist L);

/*****栈******/
typedef struct {
    int data[M];
    int top;
} SqStack;

void initStack(SqStack *S);
int pop(SqStack *S);
void push(SqStack *S, int m);

int main() {
    // 单链表
    //Sqlist *list = (Sqlist *) malloc (sizeof(Sqlist));
//    Sqlist list;
//    initList(&list);
//    Std std1 = {12, "zhangsan"};
//    Std std2 = {18, "lisi"};
//    Std std3 = {21, "xiaoming "};
//    listInsert(&list, 0, std1);
//    listInsert(&list, 1, std2);
//    listInsert(&list, 1, std3);
//    printf("%d\n%s\n", list.data[0].age, list.data[0].name);
//    printf("%d\n%s\n", list.data[1].age, list.data[1].name);
//    printf("%d\n%s\n", list.data[2].age, list.data[2].name);
//
//    /* 我的第一个 C 程序 */
//    printf("Hello, World! \n");
//
//    return 0;

    //双链表

//    DLinklist L = initDLinkList();
//    scanList(L);

    SqStack *S = (SqStack *) malloc(sizeof(SqStack));
    initStack(S);
    push(S, 1);
    push(S, 2);
    push(S, 4);
    printf("%d ", pop(S));
    printf("%d ", pop(S));
    printf("%d ", pop(S));
}


/***********单链表方法**/
int initList(Sqlist *list)
{
    list->length = 0;
    return 1;
}

int listInsert(Sqlist *list, int len, Std std) {
    if (len < 0 || len > list->length + 1 ) {
        return 0;
    }

    if(list->length >= M) {
    return 0;
    }
    int j;
    for(j = list->length; j>=len; j--) {
        list->data[j] = list->data[j-1];
    }
    list->data[len] = std;
    list->length++;
    return 1;
}


/***双链表方法**/

DLinklist initDLinkList()
{
    DLinklist L = (DLinklist) malloc (sizeof(DNode));
    L->next = NULL;
    L->prior = NULL;
    int x;
    DNode *p;//头插法指针
    scanf ("%d", &x);
    while (x != 10) {
        p = (DNode*) malloc(sizeof(DNode));
        p->data = x;
        if (L->next != NULL) {
            L->next->prior = p;
        }
        p->next = L->next;
        p->prior = L;
        L->next = p;
        scanf("%d",&x);
    }
    return L;
}

void scanList(DLinklist L)
{
    DNode *p = L->next;
    while (p != NULL) {
        printf("%d\t",p->data);
        p = p->next;
    }
}

/*******栈****/

void initStack(SqStack *S)
{
    S->top = -1;
}

void push(SqStack *S, int m) {
    S->top++;
    S->data[S->top] = m;
}

int pop(SqStack *S) {
    return S->data[S->top--];
}

