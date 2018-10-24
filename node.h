/*************************************************************************
	> File Name: node.h
	> Author:Liu Peng 
	> Mail: 1374294826@qq.com
	> Created Time: 2018年07月11日 星期三 14时35分29秒
 ************************************************************************/

#ifndef LP_NODE_H
#define LP_NODE_H

#include<stack>
#include<cstring>

#define MAX 3 
#define MIN 1

using namespace std;

typedef struct Nodes
{
    int key[MAX];
    struct Nodes *child[MAX+1];
    int num;
    bool is_leaf;
    struct Nodes *next;
    struct Nodes *parent;
    struct Nodes *pre;
    Nodes()
    {
        memset(key, 0, sizeof(key));
        memset(child, 0, sizeof(child));
        num = 0;
        is_leaf = true;
        next = NULL;
        parent = NULL;
        pre = NULL;
    }
}Node;

typedef struct mem_Nodes
{
    int key[MAX];
    int num;
    bool is_leaf;
}mem_Node;

typedef struct Positions
{
    Node *target_node;
    int pos;
}Position;

typedef struct Roads
{
    stack<int> path;
}Road;
#endif
