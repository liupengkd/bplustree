/*************************************************************************
	> File Name: main.cpp
	> Author:Liu Peng 
	> Mail: 1374294826@qq.com
	> Created Time: 2018年07月19日 星期四 20时33分28秒
 ************************************************************************/

#include<iostream>
#include<sys/time.h>
#include<unistd.h>
#include"b_plus_tree.h"

using namespace std;

int main()
{
    struct timeval start, end;
    BPlusTree *bptree = new BPlusTree();
    
    gettimeofday(&start, NULL);
    for(int count = 1; count <= 10000000; count++)
    {
        bptree->bptree_insert_behind(count);    
    }
    gettimeofday(&end, NULL);
    cout << "插入千万次数据的时间" << ((end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000 )<< endl;
    
/*    gettimeofday(&start, NULL);
    
    bptree->bptree_serialization();
    
    gettimeofday(&end, NULL);
    cout << "序列化千万数据的时间" << ((end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000 )<< endl;

    gettimeofday(&start, NULL);

    bptree->bptree_deserialization();
    
    gettimeofday(&end, NULL);
    cout << "反序列化千万数据的时间" << ((end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000 )<< endl;
  */  
    gettimeofday(&start, NULL);
    for(int count = 1; count <= 10000000; count++)
    {
        bptree->bptree_delete(count);    
    }
    gettimeofday(&end, NULL);
    cout << "删除千万次数据的时间" << ((end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000 )<< endl;
    

    bptree->bptree_serialization();
    
    delete bptree;
    return 0;
}
