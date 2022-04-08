#pragma once
#include <climits>
#include <filesystem>
#include <time.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <chrono>
#include <math.h>
#include "kvstore_api.h"
using namespace std;
namespace fs = std::filesystem;

class KVStore : public KVStoreAPI {
	// You can add your implementation here
private:
    SkipList list;
    vector<int> level;
    map<int, IndexNode> index; //存储数据索引在内存中
public:
    int maxfile(int level); //输入文件目录level，获得该层可以容纳的文件数量

    uint64_t gettime(); //获得时间戳

    void remdup(vector<TableNode> &vec); //vec中有相同key的node，删除早的.前提是vec有序

    void handleDel(vector<TableNode> &vec); //处理lazy的删除

    void compaction(vector<int> &level, map<int, IndexNode> &index);

	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

	void scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string> > &list) override;
};
