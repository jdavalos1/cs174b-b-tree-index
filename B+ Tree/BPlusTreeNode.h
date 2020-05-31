#include <iostream>
#include <vector>
#include <list>
using namespace std;

namespace BPTree {
    class BPTreeNode
    {
        private:
            int _fanout;
            bool _isLeaf;
            list<pair<int, int>> *_data;
            vector<int>* _intervals;
            BPTreeNode *_parent;
            vector<BPTreeNode*> *_children;
            BPTreeNode *_neighbor;
            // Helper functions
            BPTreeNode* find_neighbor();
        public:
            BPTreeNode(bool isLeaf, int fanout) : _isLeaf(isLeaf), _fanout(fanout)
            { 
                if(_isLeaf)
                {
                    _data = new list<pair<int, int>>();
                    _intervals = NULL;
                }
                else
                {
                    _data = NULL;
                    _intervals = new vector<int>();
                }
                _parent = NULL;
                _children = NULL;
                _neighbor = NULL;
            }
            list<pair<int, int>>* recursive_search(const int&);
            void recursive_insert(const pair<int, int>*);
            void bulk_load(list<pair<int, int>>);
    };
    
    class BPTreeManager
    {
        private:
            int fanout;
            BPTreeNode *root;
        public:
            list<pair<int, int>>* search(const int);
            void insert(pair<int, int>*);
    };
}
