//
// Created by 董云鹏 on 2022/4/5.
//

#include "skiplist.h"
#include <iostream>
#include <stdlib.h>
#include "MemTable.h"

using namespace std;
double SkipList::my_rand()
{
    s = (16807 * s) % 2147483647ULL;
    return (s + 0.0) / 2147483647ULL;
}

int SkipList::randomLevel()
{
    int result = 1;
    while (result < MAX_LEVEL && my_rand() < 0.5)
    {
        ++result;
    }
    return result;
}

void put(uint64_t key, const std::string &s)
{
    SKNode* node = new SKNode(key,s,NORMAL);
    SKNode *h[MAX_LEVEL];
    SKNode* cur;
    SKNode* last;
    for(int i =0;i<MAX_LEVEL;i++)
    {
        h[i]=head;
    }
    cur=head;
    last = head;

    for(int i=MAX_LEVEL-1;i>=0;i--)
    {
        while(cur->forwards[i]->key != INT_MAX && key>(cur->forwards[i]->key))
        {
            cur= cur->forwards[i];
        }
        h[i] = cur;
    }
    last=cur->forwards[0];
    if(last != NULL && last->key == key)
    {
        last->val=value;
        return ;
    }

    else{
        int level=SkipList::randomLevel();
        for(int k = 0; k <level;k++)
        {
            node->forwards[k]=h[k]->forwards[k];
            h[k]->forwards[k]=node;
        }}
    return ;
}

void SkipList::Search(int key)
{
    // TODO
    SKNode * cur;
    cur=head;
    bool flag= true;
    for(int i=MAX_LEVEL-1;i>=0;i--){

        flag=true;
        if((cur->type) ==SKNodeType::HEAD && flag== true){
            cout<<i+1<<",h ";}

        while(((cur->forwards[i]->key)!=INT_MAX) && key>(cur->forwards[i]->key)){
            if((cur->type) !=SKNodeType::HEAD  && flag== true){
                cout<<i+1<<","<<cur->key<<" ";
            }

            cur =cur->forwards[i];
            cout<<i+1<<","<<cur->key<<" ";
            flag=false;
        }
        if((cur->type) !=SKNodeType::HEAD&& flag== true){
            cout<<i+1<<","<<cur->key<<" ";
        }
    }
    cur=cur->forwards[0];

    if(cur!= nullptr && cur->key ==key){
        cout<<"1,"<<cur->key<<" "<<cur->val<<endl;
    }
    else{ cout<<"1,";
        if(cur->key!=INT_MAX)cout<<cur->key;
        else cout<<"N";
        cout<<" Not Found"<<endl;
    }}

void SkipList::Delete(int key)
{
    // TODO
    SKNode *s[MAX_LEVEL];
    SKNode* current;
    SKNode* last;
    for(int i=0;i<MAX_LEVEL;i++)
    {
        s[i]=head->forwards[i];
    }
    current =head;
    last = head;
    int j=0;
    for(int i =MAX_LEVEL -1;i>=0;i--)
    {
        while(current->forwards[i]->key != INT_MAX && key>current->forwards[i]->key)//key大于下一个数据的值。转到本层下一个元素
        {
            current = current->forwards[i];
        }
        if(key==current->forwards[i]->key){
            j++;
        }
        s[i] = current;//保存每一层位置上的最后指针的前驱
    }
    last=current->forwards[0];
    if(last->key != key)
    {
        return;
    }
    for(int i = 0; i<j;i++)
    {
        s[i]->forwards[i]=s[i]->forwards[i]->forwards[i];
    }
    return;
}

void SkipList::Display()
{
    for (int i = MAX_LEVEL - 1; i >= 0; --i)
    {
        std::cout << "Level " << i + 1 << ":h";
        SKNode *node = head->forwards[i];
        while (node->type != SKNodeType::NIL)
        {
            std::cout << "-->(" << node->key << "," << node->val << ")";
            node = node->forwards[i];
        }

        std::cout << "-->N" << std::endl;
    }
}