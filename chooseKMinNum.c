#define MAXVALUE 1000
#include <stdio.h>
//#include "MyStack.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef char DataType;

typedef struct StackNode {
    DataType data;
    struct StackNode *next;//指向后面一个元素
}StackNode, *PMyStack;

// 初始化stack
PMyStack initStack()
{
    PMyStack p = (PMyStack) malloc (sizeof(StackNode));
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

void findTwoMin(int* data, int *min1, int *min2, int length);
int findSomeMin(int *arr, int arrlen, int *out, int outlen);

int isRightBracket(char *brackets, int len);

int main(int argc, char *argv[])
{
//    int data[100];
    int n = 8;
    int i=0;
//    for(; i<n; i++){
//        scanf("%d", &data[i]);
//    }
////    int min1, min2;
////    findTwoMin(data, &min1, &min2, n);
////    printf("min1 = %d\n", min1);
////    printf("min2 = %d\n", min2);
//
//    int arr[10] = {0};
//    findSomeMin(data, n, arr, 4);
//    for ( i = 0; i < 4; ++i) {
//        printf("min%d = %d\n", i, arr[i]);
//    }
//    return 0;

    char str[10] = {0};
    for ( i = 0; i < n; i++) {
        scanf("%c", &str[i]);
    }
    for (i = 0; i < n; i++) {
        printf("%c ", str[i]);
    }
    printf("%d--------\n", strlen(str));
    int a = isRightBracket(str, strlen(str));
    printf("%d", a);
}

void findTwoMin(int* data, int *min1, int *min2, int length){
    int temp;
    *min1 = 9999;
    *min2 = 9999;
    int i;
    for( i=0; i<length; i++){
        temp = data[i];
        if(temp<*min2){
            if(temp > *min1){
                *min2 = temp;
            }else{
                *min2  = *min1;
                *min1 = temp;
            }
        }
    }
}

/**
 * @param arr int 数组
 * @param arrlen 数组长度
 * @param out  输出的数组指针
 * @param outlen 输出数组的长度
 * @return 0 失败 1 成功
 */
int findSomeMin(int *arr, int arrlen, int *out, int outlen) {
    if (outlen > arrlen) return 0;
    if (outlen == arrlen)  {
        out = arr;
        return 1;
    }
    int i, temp;
    //初始化
    for ( i = 0; i < outlen; i++) {
        out[i] = MAXVALUE;
    }
    for (i = 0; i < arrlen; i++) {
        temp = arr[i];
        //其中out[0]是最大的数
        if (temp < out[0]) {
            int j = outlen-1;
            for(; j >= 0; j--) {
                if(temp < out[j]) {
                    int m = out[j];
                    out[j] = temp;
                    temp = m;
                }
            }
        }
    }
    return 1;
}

// 1正确 0 错误
int isRightBracket(char *brackets, int len) {
    PMyStack mystack = initStack();
    int i;
    char g;
    for (i = 0; i < len; ++i) {
        int flag = 0;//表示匹配成功
        printf("%c", brackets[i]);
        switch (brackets[i]) {
            case '(':
            case '{':
            case '[':
                pushMyStack(mystack, brackets[i]);
                break;
            case ')':
            case '}':
            case ']':
                popMyStack(mystack, &g);//弹出
                if (brackets[i] == ')' && g == '(') {
                    break;
                }
                if (brackets[i] == '}' && g == '{') {
                    break;
                }
                if (brackets[i] == ']' && g == '[') {
                    break;
                }
                flag = 1;
                break;
            default: flag = 1;
                break;
        }
        if (flag == 1) {
            printf("%d", flag);
            return 0;
        }
    }
    bool a = getTopMyStack(mystack, &g);
    if (!a) {return 1;} else {
        return 0;
    }
}