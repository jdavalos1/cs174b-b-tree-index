#include <iostream>
#include <vector>
#include <list>
using namespace std;

namespace BPTree {
    class BPTreeIndex
    {
        private:
            int _fanout;
            bool _isLeaf;
            list<pair<int, int>> *_data;
            vector<int>* _intervals;
            BPTreeIndex *parent;
            vector<BPTreeIndex*> *_children;
            BPTreeIndex *_neighbor;
            // Helper functions
            list<pair<int, int>>* recursive_search(BPTreeIndex*, const int&);
        
        public:
            BPTreeIndex(pair<int, int>*, bool, int, float);
            void insert(pair<int, int>*);
            list<pair<int, int>>* search(const int);
            void bulk_load(list<pair<int, int>>);
    };
}
