#include <iostream>
#include <list>
using namespace std;

namespace BPTree {

    class BPTreeIndex
    {
        private:
            pair<char, char> *_data;
            list<BPTreeIndex*> _children;
            bool _isLeaf;
            int _fanout;
            float _fill_factor;
        
        public:
            BPTreeIndex(pair<char, char>*, bool, int, float);
            void insert(pair<char, char>);
            void search(char);
            void bulk_load(list<pair<char, char>>);

    };
}
