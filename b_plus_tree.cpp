/*************************************************************************
	> File Name: b_plus_tree.cpp
	> Author:Liu Peng 
	> Mail: 1374294826@qq.com
	> Created Time: 2018年07月18日 星期三 11时19分56秒
 ************************************************************************/

#include<iostream>
#include<queue>
#include<fstream>
#include"b_plus_tree.h"

using namespace std;

/*
 *when flag is one, just finding
 *when flaf is two,just deleting
 */
void BPlusTree::search(Node *node, int target, int flag)
{
    int j = node->num;
    int m = 0;
  
    m = bptree_search_dichotomize(node->key, target, j) + 1;

    if(node->key[m] == target)
    {
        if(node->is_leaf == false)
        {
            Node *equal_node = node;
            road->path.push(m+1);
            node = node->child[m+1];

            while(node->is_leaf == false)
            {
                road->path.push(0);
                node = node->child[0];
            }
            t->target_node = node;
            t->pos = 0;
            if(flag == 2)
            {
                equal_node->key[m] = node->key[1];
            }
        }
        else
        {
            t->target_node = node;
            t->pos = m;
        }

        //cout << "find " << target << endl;
           
    }
    else if(node->is_leaf == false)
    {
        road->path.push(m);
        search(node->child[m], target, flag);
    }
    else
    {
        cout << "don`t find " << target << endl;
        t->target_node = NULL;
    }
}

void BPlusTree::search_insert(Node *node, int target)
{
    int j = node->num;
    int m = 0;
  
    m = bptree_search_dichotomize(node->key, target, j) + 1;

    if(node->key[m] == target)
    {
        cout << "the same key" << endl;
        t->target_node = NULL;
    }
    else if(node->is_leaf == false)
    {
        road->path.push(m);
        search_insert(node->child[m], target);
    }
    else
    {
            //cout << "find insert " << target << " location" << endl;
            t->pos = m;
            t->target_node = node;
    }
}

void BPlusTree::bptree_delete(int target)
{
    search(this->root, target, 2);
    int ppos;
    if(t->target_node == NULL)
    {
        return;
    }
        
    int pos = t->pos;
    Node *node = t->target_node;
    for(int i = pos; i < node->num -1; i++)
    {
        node->key[i] = node->key[i+1];
    }
    node->num--;

    bptree_delete_recursive(node);

    if(!road->path.empty())
    {
        bptree_empty_road();
    }
    bptree_null_t();
}

void BPlusTree::bptree_delete_recursive(Node *node)
{
    int ppos;    
    if(node->num < MIN)
    {
        if(!road->path.empty())
        {
            ppos = road->path.top();
            road->path.pop();
        }
        else
        {
            if(node->num == 0)
            {
                if(node->child[0] != NULL)
                {
                    this->root = node->child[0];
                    this->bptree_node_num--;
                    this->root->parent = NULL;
                    delete node;
                }
                else
                {
                    cout << "the tree is null" << endl;
                }
            }
            return;
        }

        if(ppos == node->parent->num && node->parent->child[ppos-1]->num <= MIN)
        {
            bptree_merge_child(node->parent, ppos - 1, node->parent->child[ppos-1], node);
        }
        else if(ppos != node->parent->num && node->parent->child[ppos+1]->num > MIN)
        {
            bptree_shift_to_left_child(node->parent, ppos, node, node->parent->child[ppos+1]);
        }
        else if(ppos > 0 && node->parent->child[ppos-1]->num > MIN)
        {
            bptree_shift_to_right_child(node->parent, ppos - 1,node->parent->child[ppos-1], node);
        }
        else
        {
            bptree_merge_child(node->parent, ppos, node, node->parent->child[ppos+1]);
        }
        bptree_delete_recursive(node->parent);
    }
}

void BPlusTree::bptree_merge_child(Node *node, int pos, Node *l, Node *r)
{
    if(l->is_leaf == true)
    {
        for(int i = l->num, j = 0; j < r->num; i++, j++)
        {
            l->key[i] = r->key[j];       
        }    
        l->num = MIN + MIN -1;
    }
    else
    {
        for(int i = l->num + 1, j = 0; j < r->num; i++, j++)
        {
            l->key[i] = r->key[j];
        }
        l->key[l->num] = node->key[pos];
        for(int i = l->num + 1, j = 0; j <= r->num; i++, j++) 
        {
            l->child[i] = r->child[j];
            r->child[j]->parent = l;
            r->child[j] = NULL;
        }  
        l->num = MIN + MIN;
    }

    for(int j = pos + 1; j < node->num; j++) 
    {
        node->key[j-1] = node->key[j];
        node->child[j] = node->child[j+1];       
    }

    node->num -= 1;

    if(l->is_leaf == true)
    {
        l->next = r->next;
        if(r->next != NULL)
        {
            r->next->pre = l;
        }
    }
    bptree_node_num--;
    delete r;
}

void BPlusTree::bptree_shift_to_left_child(Node *node, int pos, Node *l, Node *r)
{
    l->num++;

    if(r->is_leaf == false) 
    {
        l->key[l->num-1] = node->key[pos];
        node->key[pos] = r->key[0];          
    } 
    else 
    {
        l->key[l->num-1] = r->key[0];
        node->key[pos] = r->key[1];    
    }

    for(int j = 1; j < r->num; j++) 
    {
        r->key[j-1] = r->key[j];      
    }

    if(r->is_leaf == false) 
    {
        l->child[l->num] = r->child[0];
        r->child[0]->parent = l;
        r->child[0] = NULL;
        for(int j = 1; j <= r->num; j++) 
        {
            r->child[j-1] = r->child[j];   
        }   
    } 

    r->num--;
}

void BPlusTree::bptree_shift_to_right_child(Node *node, int pos, Node *l, Node *r)
{
    r->num++;

    for(int i = r->num -1; i > 0; i--) 
    {
        r->key[i] = r->key[i-1]; 
    }

    if(r->is_leaf == false)
    {
        r->key[0] = node->key[pos];
        node->key[pos] = l->key[l->num-1]; 
    }
    else
    {
        r->key[0] = l->key[l->num-1];
        node->key[pos] = l->key[l->num-1];
    }

    if(r->is_leaf == false)
    {
        for(int i = r->num; i > 0; i--)
        {
            r->child[i] = r->child[i-1];
        }
        r->child[0] = l->child[l->num];
        l->child[l->num]->parent = r;
        l->child[l->num] = NULL;
    }

    l->num--;
}

void BPlusTree::bptree_serialization()
{
    if(this->root->num == 0)
    {
        cout << "the root is NULL" << endl;
        delete this->root;
        delete this->t;
        delete this->road;
        this->bptree_node_num = 0;
        return;       
    }
    ofstream out(this->filename, ios::out|ios::binary|ios::trunc);
    if(!out.is_open())
    {
        cout << "文件创建失败" << endl;
        return;
    }

    queue<Node *> bptree;
    Node *p;
    mem_Node *mp = new mem_Node;
    bptree.push(root);

    while(!bptree.empty())
    {
        p = bptree.front();
        bptree_mem_node_new(mp, p);
        out.write((char *)mp, sizeof(mem_Node));
        bptree.pop();
        if(p->is_leaf != true)
        {
            for(int i = 0; i <= p->num; i++)
                bptree.push(p->child[i]);         
        }

        delete p;   
    }

    delete mp;
    delete this->t;
    delete this->road;
    out.close();
    this->bptree_node_num = 0;
}

void BPlusTree::bptree_deserialization()
{
    ifstream in(this->filename, ios::in|ios::binary);
    if(!in.is_open())
    {
        cout << "文件打开失败或没有该文件" << endl;
        this->bptree_node_num++;
        this->root = new Node;
        this->t = new Position;
        this->road = new Road;
        return;
    }
    if(in.eof())
    {
        cout << "文件为空" << endl;
        this->bptree_node_num++;
        this->root = new Node;
        this->t = new Position;
        this->road = new Road;
        in.close();
        return;
    }

    mem_Node *mp = new mem_Node;
    in.read((char *)mp, sizeof(mem_Node));
    this->root = bptree_node_new(mp);
    this->t = new Position;
    this->road = new Road;
    Node *p;
    Node *child;
    Node *pre = NULL;
    int i;
    int num;
    queue<Node *> bptree;
    this->bptree_node_num = 1;

    if(this->root->is_leaf == false)
    {
        bptree.push(root);   
    }

    while(!bptree.empty())
    {
        p = bptree.front();
        bptree.pop();
        i = 0;
        num = p->num;
        while(num >= 0)
        {
            in.read((char *)mp, sizeof(mem_Node));
            child = bptree_node_new(mp);
            this->bptree_node_num++;
            p->child[i] = child;
            child->parent = p;
            if(child->is_leaf == false)
            {
                bptree.push(child);     
            }
            else if(pre != NULL)
            {
                child->pre = pre;
                pre->next = child;
            }
            num--;
            i++;
            pre = child;
        }  
    }

    delete mp;
    in.close();
}

void BPlusTree::bptree_search_max_key()
{
    Node *max = root;
    while(max->is_leaf == false)
    {
        max = max->child[max->num];   
    }

    t->target_node = max;
    t->pos = max->num -1;

    return;
}

void BPlusTree::bptree_search_min_key()
{
    Node *min = root;
    while(min->is_leaf == false)
    {
        min = min->child[0];
    }

    t->target_node = min;
    t->pos = 0;
    return;
}

BPlusTree::BPlusTree()
{
    cout << "输入序列化的文件名" << endl;
    cin >> this->filename;
    this->bptree_node_num = 0;
    bptree_deserialization();
}

BPlusTree::~BPlusTree()
{
//    bptree_serialization();
}

void BPlusTree::bptree_linear_positive_print()
{
    bptree_search_min_key();
    Node *node = t->target_node;
    while(node != NULL)
    {
        for(int i = 0; i < node->num; i++)
        {
            cout << node->key[i] << "  ";
        }
        cout << endl;
        node = node->next;
    }
    bptree_null_t();
}

void BPlusTree::bptree_linear_reverse_print()
{
    bptree_search_max_key();
    Node *node = t->target_node;
    while(node != NULL)
    {
        for(int i = 0; i < node->num; i++)
        {
            cout << node->key[i] << "  ";
        }
        cout << endl;
        node = node->pre;
    }
    bptree_null_t();
}

Node *BPlusTree::bptree_node_new(mem_Node *mem_node)
{
    Node *node = new Node;
    if(node == NULL)
    {
        cout << "空间分配失败" << endl;
        return NULL;          
    }

    for(int i = 0; i < mem_node->num; i++)
    {
        node->key[i] = mem_node->key[i];   
    }

        node->num = mem_node->num;
        node->is_leaf = mem_node->is_leaf;
        
        return node;
}

void *BPlusTree::bptree_mem_node_new(mem_Node *mem_node, Node *node)
{
    if(mem_node == NULL)
    {
        cout << "空间分配失败" << endl;
        return NULL;          
    }

    for(int i = 0; i < node->num; i++)
    {
        mem_node->key[i] = node->key[i];           
    }

    mem_node->num = node->num;
    mem_node->is_leaf = node->is_leaf;
}

int BPlusTree::bptree_search_dichotomize(int *key, int target, int num)
{
    int low = 0;
    int high = num - 1;
    int mid;

    while(low <= high)
    {
        mid = (low + high) / 2;
       
        if(key[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid -1;
        }
    }

    return high;
}

void BPlusTree::bptree_insert_behind(int target)
{
    //t = search(this->root, target, 1, road);
    search_insert(this->root, target);
    Node *node; 
    if(t->target_node == NULL)
    {
        return;
    }

    node = t->target_node;
    road->path.push(t->pos);
    bptree_split_forward(node, target, NULL);

    if(!road->path.empty())
    {
        bptree_empty_road();
    }
    bptree_null_t();
}

void BPlusTree::bptree_split_forward(Node *node, int target, Node *rchild)
{
    int pos;
    if(node->num < MAX)
    {
        pos = road->path.top();
        road->path.pop();
        if(node->is_leaf == true)
        {
            for(int i = node->num - 1; i >= pos; i--)
            {
                node->key[i+1] = node->key[i];
            }
            node->key[pos] = target;
            node->child[pos+1] = rchild;
            rchild->parent = node;
        }
        else
        {
            for(int i = node->num - 1; i >= pos; i--)
            {
                node->key[i+1] = node->key[i];
                node->child[i+2] = node->child[i+1]; 
            }
            node->key[pos] = target;
            node->child[pos+1] = rchild;
            rchild->parent = node;
        }
        node->num++;
        return;
    }

    int num_split;
    num_split = (MAX + 1) / 2;

    Node *new_node = new Node;
    bptree_node_num++;
    Node *parent;
    pos = road->path.top();
    road->path.pop();

    if(node->parent == NULL)
    {
        road->path.push(0);
        Node *new_root = new Node;
        new_root->is_leaf = false;
        this->root = new_root;
        bptree_node_num++;
        parent = new_root;

        new_root->child[0] = node;
        node->parent = new_root;
    }
    else
    {
        parent = node->parent;
    }

    new_node->is_leaf = node->is_leaf;
    int i,j;

    if(node->is_leaf == true)
    {
        target = bptree_split_leaf(num_split, pos, node, target, new_node);
    }
    else
    {
        target = bptree_split_no_leaf(num_split, pos, node, target, new_node, rchild);
    }
    bptree_split_forward(parent, target, new_node);
}

int BPlusTree::bptree_split_leaf(int num_split, int pos, Node *node, int target, Node *new_node)
{
    int i,j;
    if(pos >= num_split)
    {
        node->num = num_split;

        for (i = 0, j = 0; i < pos - num_split; i++,j++)
        {
            new_node->key[i] = node->key[j+num_split];
        }

        new_node->key[i] = target;
        i++;

        for(i, j; i < MIN + 1; i++,j++)
        {
            new_node->key[i] = node->key[j+num_split];
        }

        new_node->num = MIN + 1;
        node->next = new_node;
        new_node->pre = node;
    }
    else
    {
            
        node->num = num_split;

        for(i = 0, j = 0; i < MIN + 1; i++)
        {
            new_node->key[i] = node->key[i+num_split-1];
        }

        for(i = num_split - 1; i > pos; i--)
        {
            node->key[i] = node->key[i -1];
        }

        node->key[i] = target;

        new_node->num = MIN + 1;
        node->next = new_node;
        new_node->pre = node;
    }

    return target;
}

int BPlusTree::bptree_split_no_leaf(int num_split, int pos, Node *node, int target, Node *new_node, Node *rchild)
{
    int i, j;
    if(pos >= num_split )
    {
        node->num = num_split;
        if(pos == num_split)
        {
            new_node->child[0] = rchild;
            rchild->parent = new_node;
            for(i = 0; i < MIN; i++)
            {
                new_node->key[i] = node->key[i+num_split];
                new_node->child[i+1] = node->child[i+num_split+1];
                node->child[i+num_split+1]->parent = new_node;
                node->child[i+num_split+1] = NULL;
            }
        }
        else
        {
            for(i = 0, j = 1; i < pos - num_split - 1; i++, j++)
            {
                new_node->key[i] = node->key[j+num_split];
                new_node->child[i] = node->child[j+num_split];
                node->child[j+num_split]->parent = new_node;
                node->child[j+num_split] = NULL;     
            }

            new_node->key[i] = target;
            new_node->child[i] = node->child[j+num_split];
            node->child[j+num_split]->parent = new_node;
            node->child[j+num_split] = NULL;
            new_node->child[i+1] = rchild;
            rchild->parent = new_node;
            i++;

            for(i, j; i < MIN; i++, j++)
            {
                new_node->key[i] = node->key[j+num_split];
                new_node->child[i+1] = node->child[j+num_split+1];
                node->child[j+num_split+1]->parent = new_node;
                node->child[j+num_split+1] = NULL;     
            }

            target = node->key[num_split];
        }

        new_node->num = MIN;
    }
    else
    {
        node->num = num_split;

        int now_target = target;
        target = node->key[num_split -1];

        for(i = 0; i < MIN; i++)
        {
            new_node->key[i] = node->key[i+num_split];
            new_node->child[i] = node->child[i+num_split];
            node->child[i+num_split]->parent = new_node;
            node->child[i+num_split] = NULL;
        }

        new_node->child[i] = node->child[i+num_split];
        node->child[i+num_split]->parent = new_node;
        node->child[i+num_split] = NULL;
        new_node->num = MIN;

        for(i = num_split - 1; i > pos; i--)
        {
            node->key[i] = node->key[i -1];
            node->child[i+1] = node->child[i];
        }

        node->key[i] = now_target;
        node->child[i+1] = rchild;
        rchild->parent = node;
    }
    return target;
}

void BPlusTree::bptree_empty_road()
{
    while(!road->path.empty())
    {
        road->path.pop();      
    }
}

void BPlusTree::bptree_null_t()
{
    t->target_node = NULL;
}

void BPlusTree::bptree_delete_behind(int target)
{
    search(this->root, target, 2);
    if(t->target_node == NULL)
    {
        return;
    }
        
    int pos = t->pos;
    Node *node = t->target_node;

    bptree_move_forward(node);

    if(!road->path.empty())
    {
        bptree_empty_road();
    }
    bptree_null_t();
}

void BPlusTree::bptree_move_forward(Node *node)
{
    int ppos;
    ppos = road->path.top();
    road->path.pop();

    if(node->num > MIN)
    {
        for(int i = ppos; i < node->num -1; i++)
        {
            node->key[i] = node->key[i+1];
        }
        node->num--;
        
        if(node->num == 0)
        {
            if(node->child[0] != NULL)
            {
                this->root = node->child[0];
                this->bptree_node_num--;
                this->root->parent = NULL;
                delete node;
            }
            else
            {
                cout << "the tree is null" << endl;
            }
        }
        return;
    }
    else
    {
        if(ppos == node->parent->num && node->parent->child[ppos-1]->num <= MIN)
        {
            bptree_merge_child(node->parent, ppos - 1, node->parent->child[ppos-1], node);
        }
        else if(ppos != node->parent->num && node->parent->child[ppos+1]->num > MIN)
        {
            bptree_shift_to_left_child(node->parent, ppos, node, node->parent->child[ppos+1]);
        }
        else if(ppos > 0 && node->parent->child[ppos-1]->num > MIN)
        {
            bptree_shift_to_right_child(node->parent, ppos - 1,node->parent->child[ppos-1], node);
        }
        else
        {
            bptree_merge_child(node->parent, ppos, node, node->parent->child[ppos+1]);
        }
        bptree_delete_recursive(node->parent);
    }
}
