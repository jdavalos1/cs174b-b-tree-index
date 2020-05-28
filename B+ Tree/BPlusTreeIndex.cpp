#include "BPlusTreeIndex.h"
using namespace BPTree;

BPTreeIndex::BPTreeIndex(pair<char, char>* data, bool isLeaf, int fanout, float fill) : _isLeaf(isLeaf), _fanout(fanout), _fill_factor(fill), _data(data) {}

void BPTreeIndex::insert(pair<char, char> *data)
{

}

void BPTreeIndex::search(char data)
{

}

void bulk_load(list<pair<char, char> data)
{

}