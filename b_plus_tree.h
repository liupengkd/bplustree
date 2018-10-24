/*************************************************************************
	> File Name: b+_tree.h
	> Author:Liu Peng 
	> Mail: 1374294826@qq.com
	> Created Time: 2018年07月11日 星期三 14时36分13秒
 ************************************************************************/

#ifndef LP_B_PLUS_TREE_H
#define LP_B_PLUS_TREE_H

#include"node.h"

class BPlusTree{
private:
    Node *root;
    int bptree_node_num;
    char filename[20];
    Position *t;
    Road *road;

public:
    BPlusTree();
    ~BPlusTree();
    void bptree_merge_child(Node *node, int pos, Node *l, Node *r);
    void bptree_search_max_key();
    void bptree_search_min_key();
    void bptree_shift_to_left_child(Node *node, int pos, Node *l, Node *r);
    void bptree_shift_to_right_child(Node *node, int pos, Node *l, Node *r);
    void bptree_delete(int target);
    void bptree_delete_recursive(Node *node);
    void bptree_serialization();
    void bptree_deserialization();
    void search(Node *node, int target, int flag);
    void search_insert(Node *node, int target);
    void bptree_linear_positive_print();
    void bptree_linear_reverse_print();
    Node *bptree_node_new(mem_Node *mem_node);
    void *bptree_mem_node_new(mem_Node *mem_node, Node *node);
    int bptree_search_dichotomize(int *key, int target, int num);
    void bptree_insert_behind(int target);
    void bptree_split_forward(Node *node, int target, Node *rchild);
    void bptree_empty_road();
    void bptree_null_t();
    int bptree_split_leaf(int num_split, int pos, Node *node, int target, Node *new_node);
    int bptree_split_no_leaf(int num_split, int pos, Node *node, int target, Node *new_node, Node *rchild);
    void bptree_delete_behind(int target);
    void bptree_move_forward(Node *node);
};
#endif
