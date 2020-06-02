//
// Created by dingmac on 2020/3/19.
//

#ifndef TEST1_GRAPHITEMS_H
#define TEST1_GRAPHITEMS_H
#define MAXNUM 10

///邻接矩阵
typedef char VertexType;
typedef int EdgeType;
typedef struct MatrixGrapth {
    VertexType vertexs[MAXNUM];//一维数组存储顶点
    EdgeType edges[MAXNUM][MAXNUM];//二维数组存储邻接矩阵
    int verNum,arcNum;
}MatrixGrapth;

//邻接表法
//弧结构
typedef struct ArcNode{
    int adjvex;//顶点的下标，0，1，2等；邻接顶点
    int weight;//权重
    struct ArcNode * nextarc;
} ArcNode;

//顶点结点
typedef struct VertexNode{
    int vertex;//顶点内容，假设为A，B，C这样的
    ArcNode *firstarc;//第一个弧
}VertexNode, VertexArr[MAXNUM];

// Adjacency 邻接表
typedef struct {
    VertexArr vertices;//顶点
    int vertexNum, arcNum;//顶点和边的个数
}ALGraph;

////////////////十字链表法//////////////
typedef struct DArcNode{
    int tailvex,headvex;//头index
    struct DArcNode * hlink, *tlink;
    int info;//权重
}DArcNode;

typedef struct DVertexNode {
    int vertex;//顶点的标志
    DArcNode *fisrthlink, *firsttlink;//第一个对应的头弧和尾弧的结点
}DVertexNode;

typedef struct DGrapth{
    DVertexNode vertexs[MAXNUM];
    int vertexNum,arcNum;
}DGrapth;

//////////邻接多重表////////////
typedef struct MultiArcNode{
    int ivex,jvex;//边两边的结点的下标
    int weight;//权重
    struct MultiArcNode *ilink, *jlink;//边两顶点的下一个弧的指针
}MultiArcNode;

typedef struct MultiVertex{
    int vertex;//顶点的值
    MultiArcNode *link;//边表的指针
}MultiVertex;

typedef struct MulitGraph{
  MultiVertex vertexs[MAXNUM];
  int vertexNum,arcNum;
};
#endif //TEST1_GRAPHITEMS_H
