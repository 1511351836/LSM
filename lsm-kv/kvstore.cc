#include "kvstore.h"
#include <string>
#include <iostream>
using namespace std;
namespace fs = std::filesystem;

uint64_t KVStore::gettime()
{
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());

    long long int time = ms.count();
    return time;
}

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
    level.push_back(0);
}

KVStore::~KVStore()
{
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
    list.put(key, s);
    if (list.space() > 1048576)
    {
        if (!fs::exists("sstables/level0"))
        {
            fs::create_directories("sstables/level0");
        }
        std::ofstream outFile("newsstable.dat", std::ios::app | std::ios::binary); //创建一个新的文件，先叫newsstable
        QuadListNode *node = list.bottomFirst();
        uint64_t tmpkey = 0;
        std::string tmpval = "";
        uint64_t time = 0;
        char *charval;
        int valsize = 0;
        int tmpoffset = 0;
        while (node != list.bottomLast->succ)
        {
            //先写时间戳，再写key,再写value
            tmpkey = node->key;
            tmpval = node->value;
            valsize = tmpval.size() + 1;
            charval = (char *)tmpval.data();
            time = gettime();
            outFile.write((char *)(&time), sizeof(uint64_t));
            outFile.write((char *)(&tmpkey), sizeof(uint64_t));
            outFile.write((char *)(&charval), valsize);
            //写索引
            IndexNode tmpNode(tmpoffset, 0, level[0], time, valsize);			   //先写到level0文件目录,文件编码为level[0].不需要+1
            index[tmpkey] = tmpNode;											   //索引中自动覆盖即可
            tmpoffset = tmpoffset + sizeof(uint64_t) + sizeof(uint64_t) + valsize; //算出下一个offset
            node = node->succ;
        }
        outFile.close();
        level[0]++;	  //第0层文件目录多了1个
        list.reset(); //清空跳表

        if (level[0] < 3) //第一层不需要compaction的时候
        {
            int fileNum = level[0] - 1; //注意，文件标号是文件数目-1
            std::string sstableName = "./sstables/level0/sstable" + std::to_string(fileNum) + ".dat";
            char *charsstableName = (char *)sstableName.data();				  //转成char*才能写，我也不知道为啥
            std::rename("./sstables/level0/newsstable.dat", charsstableName); //文件从newsstable改成文件标号
        }
        else
        {
            compaction(level, index);
        }
    }
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
    return list.get(key);
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
    return list.remove(key);
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
}

/**
 * Return a list including all the key-value pair between key1 and key2.
 * keys in the list should be in an ascending order.
 * An empty string indicates not found.
 */
void KVStore::scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string> > &list)
{	
}