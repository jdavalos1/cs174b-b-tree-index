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
            // Private helper functions
            BPTreeNode* recursive_split();
            
            
        public:
            /**
             * @brief Construct a new BPTreeNode object. This constructor can be used
             * generically
             * 
             * @param isLeaf Determine if node is a leaf or inner node.
             * @param fanout Fanout for each node.
             */
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

            /**
             * @brief Construct a new BPTreeNode object
             * 
             * @param isLeaf Determine if node is a leaf
             * @param fanout Fanout of node
             * @param children List of children of node
             * @param intervals List of intervals used to traverse tree
             */
            BPTreeNode(bool isLeaf, int fanout, vector<BPTreeNode*> *children, vector<int> *intervals) : _isLeaf(isLeaf), _fanout(fanout), _intervals(intervals)
            {
                _children = children;
                // All pages are linked together thus we can iterate through the neighbors
                // on the size of the intervals + 1 in order to add the children
                // to the new non leaf node (typically that's what we want in this case)
                // based on the intervals vector
            }
            /**
             * @brief Construct a new BPTreeNode object used only by recursive split to create a split
             * page on the tree
             * 
             * @param isLeaf Determine if the value is a leaf
             * @param fanout Number of intervals contained in the node.
             * Fanout-1 determines the number of data entries per node
             * @param data_records List of data_records kept in page
             * @param parent Parent of the leaf
             */
            BPTreeNode(bool isLeaf, int fanout, list<pair<int, int>>* data_records, BPTreeNode* parent, BPTreeNode* neighbor) :
            _isLeaf(isLeaf), _fanout(fanout), _data(data_records), _parent(parent), _neighbor(neighbor), _intervals(NULL) {}

            ~BPTreeNode();
            list<pair<int, int>>* recursive_search(const int&);
            BPTreeNode* recursive_insert(const pair<int, int>*);

            // Public helper functions
            void add_child(int, BPTreeNode*);
            void persist(string);
    };
    
    class BPTreeManager
    {
        private:
            unsigned int _write_queue_size;
            unsigned int _current_queue = 0;
            unsigned int _fanout;
            string _file_name;
            BPTreeNode *_root;
        public:
            BPTreeManager(unsigned int fanout = 5, unsigned int write_queue_size = 4, string fileName = "dbtree.db") : _write_queue_size(write_queue_size), _fanout(fanout), _file_name(fileName) 
            {
                _root = new BPTreeNode(true, fanout);
            }
            list<pair<int, int>>* search(const int);
            void insert(pair<int, int>*);
            void serialize(string);
    };
}
