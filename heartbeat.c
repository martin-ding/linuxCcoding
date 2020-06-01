typedef struct 
{
	int data_type;//数据包的类型
	char name[10];//对应的客户端的名称
} package;

#define HEART 1

package DATA_PACK;

/*
**功能：客户端心跳包发送线程函数
**参数：线程传参（可传套接字）
**返回值：空
*/
void *send_heart(void *addr)
{
    while(1)
    {
        pd->data_type = HEART;  //HEART:数据包类型,pd为数据包结构体指针
        write(client_sockfd, pd, sizeof(DATA_PACK));
        sleep(3); //定时3秒
    }
    return NULL;
}

/***********************************************/
/***********************************************/

// 服务器端：

typefdef struct session
{
    char peerip[16];
    char name[10];
    int sockfd;
    int count;
    struct session *next;
} s_t;

/*
**功能：处理用户心跳包事件，将其会话记录中的计数器清零
**参数：套接字和数据包指针
**返回值：无
*/
void heart_handler(int sockfd, DATA_PACK *pd)
{
    s_t *cur = shead->next;  // shead为用户在线会话记录全局变量头指针
    while( NULL != cur)
    {
        if(strcmp(cur->name, pd->name) == 0)
        {
        	//最好加一个排它锁
            cur->count = 0;  //将计数器清零，表明用户名为pd->name的客户端还活着
            printf("客户端IP： %s ：用户 %s 连接正常\n", cur->peerip, pd->name);
        }
        cur = cur->next;
    }
}

/*
**功能：心跳检测线程函数
**参数：无
**返回值：无
*/
void *heart_check(void *p)
{
    printf("心跳检测线程已开启！\n");

    while(1)
    {
        check_handler(); // 心跳检测处理函数
        sleep(3); //定时3秒
    }
    return NULL;
}

/*
**功能：心跳检测处理函数
**参数：无
**返回值：无
*/
void check_handler()
{
    s_t *temp = NULL;  // 用于释放结点
    s_t **ppNode = &shead->next;

    while(NULL != (*ppNode))
    {
        if((*ppNode)->count == 5)
        {
            printf("客户端IP： %s ：用户 %s 已经掉线！！\n", (*ppNode)->peerip, (*ppNode)->name);
            close((*ppNode)->sockfd); //关闭对端套接字
            temp = *ppNode; //存储本结点地址
            *ppNode = (*ppNode)->next;   //移动指针
            free(temp);  //释放结点
            temp = NULL;
            continue;
        }
        else if((*ppNode)->count > 0)
        {
            printf("客户端IP： %s ：用户 %s 连接异常！\n", (*ppNode)->peerip, (*ppNode) -> name);
            (*ppNode)->count++;
            printf("count = %d\n", (*ppNode)->count); //查看计数器内容
            ppNode = &((*ppNode)->next); // 成员指针
            continue;
        }
        else if((*ppNode)->count == 0)
        {
            (*ppNode)->count++;
            printf("count = %d\n", (*ppNode)->count); //查看计数器内容
            ppNode = &((*ppNode)->next); // 成员指针
        }
    }
}