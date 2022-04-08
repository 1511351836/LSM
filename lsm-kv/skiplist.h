//
// Created by 董云鹏 on 2022/4/5.
//

#ifndef LSM_KV_SKIPLIST_H
#define LSM_KV_SKIPLIST_H
#pragma once
#include <string>
#include <vector>
#include <climits>
#include <time.h>


#define MAX_LEVEL 8
using namespace std;
enum SKNodeType
{
    HEAD = 1,
    NORMAL,
    NIL
};

struct SKNode
{
    int key;
    string s;
    SKNodeType type;
    std::vector<SKNode *> forwards;
    SKNode(int _key, string _s, SKNodeType _type)
            : key(_key), s(_s), type(_type)
    {
        for (int i = 0; i < MAX_LEVEL; ++i)
        {
            forwards.push_back(nullptr);
        }
    }
};

class SkipList
{
private:
    SKNode *head;
    SKNode *nil;
    unsigned long long s = 1;
    double my_rand();
    int randomLevel();
    //int size; //这个size是指存了多少个键值对
    //int level;
    int space; //space是指存的键值对占了多少字节
public:
    SkipList()
    {
        space=10272;
        head = new SKNode(0, "0", SKNodeType::HEAD);
        nil = new SKNode(INT_MAX, "0", SKNodeType::NIL);
        for (int i = 0; i < MAX_LEVEL; ++i)
        {
            head->forwards[i] = nil;
        }
    }
    void put(uint64_t key, const std::string &s);
    std::string get(uint64_t key);
    bool remove(uint64_t key);
    int reset();
    ~SkipList()
    {
        SKNode *n1 = head;
        SKNode *n2;
        while (n1)
        {
            n2 = n1->forwards[0];
            delete n1;
            n1 = n2;
        }
    }
};



#endif //LSM_KV_SKIPLIST_H
