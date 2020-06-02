//
// Created by dingmac on 2020/3/20.
// 图的一些操作
//
//#include <stdio.h>
//#include "Graph.h"
//#include <stdlib.h>
//#include "MyQueue.h"
//
////邻接表法初始化一个有向图
//void initAdjGrapt (){
//    int vertexNum, arcNum;//顶点数和弧数
//    printf("put vertexNum and adjNum:\n");
//    scanf("%d %d",&vertexNum, &arcNum);//接受输入，输入类似 4 5 表示4个顶点 5个弧
//    if (vertexNum < 1 || arcNum < 0) {
//        printf("illegal input");
//        return;
//    }
//    //创建一个边表
//    ALGraph alGraph;
//    alGraph.vertexNum = vertexNum;
//    alGraph.arcNum = arcNum;
//    //初始化
//    for (int i= 1;  i<= vertexNum; i++) {
//        alGraph.vertices[i].vertex = i;
//        alGraph.vertices[i].firstarc = NULL;
//    }
//    //等待用户输入顶点值和弧头的顶点值，以及权重
//    //1 4 9 表示从第一个顶点到第4个顶点的权重是9
//    printf("input arcRear arcHear weight value:\n");
//    for (int i = 1; i <= arcNum ; ++i) {
//        ArcNode * arcNode = (ArcNode *)malloc(sizeof(ArcNode));
//        int start,end,weight;
//        scanf("%d %d %d", &start,&end,&weight);
//        arcNode->adjvex = end;//弧头
//        arcNode->weight = weight;
//        arcNode->nextarc = alGraph.vertices[start].firstarc;
//        alGraph.vertices[start].firstarc = arcNode;
//    }
//};
//
////初始化一个邻接矩阵
//MatrixGrapth initMatrixGraph(int verNum){
//    MatrixGrapth matrixGrapth;
//    matrixGrapth.verNum = verNum;
//    for (int i = 1; i <= verNum; ++i) {
//        matrixGrapth.vertexs[i] = i;//顶点的值就是i
//    }
//    for (int i = 1; i <= verNum; ++i) {
//        for (int j = 1; j <= verNum; ++j) {
//            matrixGrapth.edges[i][j] = 0;//先设置为0
//        }
//    }
//    return matrixGrapth;
//};
//
////设置邻接矩阵的边元素
//void initMartixSetEdge(MatrixGrapth *graph,int start,int end) {
//    graph->edges[start][end] = 1;//表示有这个边
//}
//int visited[MAXNUM] = {0};//表示没有被访问
//// 宽度优先搜索
//void view (int v) {
//    printf("%d\n", v);
//}
//
//void BFS(MatrixGrapth *graph, int v) {
//    visited[v] = 1;//被访问了
//    view(v);
//    for (int i = 0; i < graph->verNum; ++i) {
//        if (graph->edges[v][i]) {//表明有这条边
//
//        }
//    }
//}

//bool visited[MAXNUM];
//void DFSTraverse(Graph G) {
//    for (int i = 0; i < G.verNum; ++i) {
//        visited[i] = false;
//    }
//    for (int j = 0; j < G.verNum; ++j) {
//        if (visited[j] == 0)//0表示没有被访问过
//            DFS(G,j);
//    }
//}
//
//void DFS(Graph G, int v)
//{
//    visit(v);
//    visited[v] = true;
//    for (int w = firstNeighbour(G,v); w >= 0; w = nextNeighbour(G,v)) {
//        if (visited[w] == 0)
//            DFS(G, w);
//    }
//}
//
//void MST_Krustal(Graph G, Edge* edges, int *parent)
//{
//    heapsort(edges);
//    Initial(parent);
//    for (int i = 0; i < G.arcnum; ++i) {
//        int a_root = Find(parent, edges[i].a);
//        int b_root = Find(parent, edges[i].b);
//        if (a_root != b_root) {
//            Union(parent, a_root, b_root);
//        }
//    }
//}
//
//void Dijkstra(Graph G, int v) {
//    int s[G.vexnum];//标志是不是已经加入进最短路径中
//    int path[G.vexnum];//标识从哪个顶点接入
//    int dist[G.vexnum];//路径标识，用于查找最短路径
//    for (int i = 0; i < G.vexnum; ++i) {
//        s[i] = 0;//表示没有加入到最短路径中
//        dist[i] = G.Edges[v][i];//最短路径集合中对其他顶点的路径长度
//        if (G.Edges[v][i] < MAX) {
//            path[i] = 0;
//        } else {
//            path[i] = -1;//表示无法到达
//        }
//    }
//    path[v] = -1;
//    s[v] = 1;// 表示在集合中包含了v点
//
//    //寻找最短路径
//    for (int l = 0; l < G.vexnum; ++l) {
//        int max =MAX;
//        int nextVex;
//        for (int j = 0; j < G.vexnum; ++j) {
//            if (s[j] == 0 && max < path[j]) {
//                max = path[j];
//                nextVex = j;
//            }
//        }
//
//        s[nextVex] = 1;//加入到最短路径集合中
//        for (int k = 0; k < G.vexnum; ++k) {
//            if (s[k] == 0 && dist[k] > dist[nextVex] + G.Edges[nextVex][k]) {
//                path[k] = nextVex;
//                dist[k] = dist[nextVex] + G.Edges[nextVex][k];
//            }
//        }
//    }
//}