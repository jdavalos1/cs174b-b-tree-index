#include <iostream>
#include <fstream>
#include <math.h>
#include "BPlusTreeNode.h"

using namespace BPTree;


/**
 * @brief Searches for any data records with the needed
 * values.
 * 
 * @param data Data entry being searched
 * @return list<pair<int, int>>* Returns a pointer to
 * a list of id and data entries.
 */
list<pair<int, int>>* BPTreeManager::search(const int data)
{
    return this->_root == NULL ? NULL : this->_root->recursive_search(data);
}

/**
 * @brief Insert a data record into the tree
 * 
 * @param record New data record to be inserted into
 * a tree
 */
void BPTreeManager::insert(pair<int, int>* record)
{
    // If the root is null then we need to create
    // a new root
    if(this->_root == NULL)
    {
        this->_root = new BPTreeNode(true, this->_fanout);
    }
    // Call recursive insert on the new data record
    BPTreeNode* newRoot = this->_root->recursive_insert(record);
    
    // If the value returned by newRoot is not NULL 
    // then the root was split and we have a new root
    if(newRoot != NULL) this->_root = newRoot;

    this->_current_queue++;

    if(_current_queue >= _write_queue_size)
    {
        this->serialize(_file_name);
        _current_queue = 0;
    }
}

/**
 * @brief Write out the tree to a file
 * 
 * @param dbFile Database file name.
 * Default to bptree.db
 * 
 */
void BPTreeManager::serialize(string dbFile)
{
    this->_root->persist(dbFile);
}

list<pair<int, int>>* BPTreeManager::read_pages()
{
    return this->_root->obtain_all_pages();
}

void BPTreeManager::bulk_load(list<pair<int,int>> data_entries, float fill_factor)
{
    data_entries.sort([](auto const& a, auto const& b) {
        return a.first < b.first;
    });
    auto pages = list<BPTreeNode*>();
    auto page_records = new list<pair<int, int>>();
    auto counter = 0;
    for(auto data_entry : data_entries)
    {
        page_records->push_back(make_pair(data_entry.first, data_entry.second));
        if(page_records->size() >= fill_factor * (_fanout - 1))
        {
            pages.push_back(new BPTreeNode(true, _fanout, page_records, NULL, NULL));
            counter += page_records->size();
            page_records = new list<pair<int, int>>();
        }
    }
    if(counter != data_entries.size())
        pages.push_back(new BPTreeNode(true, _fanout, page_records, NULL, NULL));
    for(auto it = pages.begin(); it != pages.end(); it++)
    {
        auto next = it;
        next++;
        (*it)->add_neighbor(*next);
        auto newRoot = _root->insert_page(*it);
        if(newRoot != NULL) _root = newRoot;
    }
    pages.back()->add_neighbor(NULL);
}

/**
 * @brief 
 * Recurses through tree to find a node with the needed value
 * as well as all the duplicates found
 * @param node Node to begin search
 * @param data Data entry to search
 * @return list<pair<int, int>>* List of <record id, data value>
 */
list<pair<int, int>>* BPTreeNode::recursive_search(const int& searchKey)
{
    // Base case if the tree index is a leaf then we can
    // obtain all the values from the node
    if(this->_isLeaf)
    {
        auto data_entries = new list<pair<int,int>>();
        bool duplicatesExists = true;
        auto current = this;
        
        // Duplicates can exist and thus the easiest way to obtain them is to
        // search first for the left most leaf then traverse to the right
        // on the leaves and fill a list to return.
        while(duplicatesExists)
        {
            for(auto it = this->_data->begin(); it != this->_data->end(); it++)
            {
                if(it->first == searchKey)
                {
                    data_entries->push_back(pair<int,int>(it->first, it->second));
                }
                else
                {
                    // The last duplicate was found
                    duplicatesExists = false;
                    break;
                }
            }
            current = current->_neighbor;
            // Are there any neighboring leaves which can contain duplicates?
            if(current == NULL || data_entries->size() == 0) duplicatesExists = false;
       }
       return data_entries;
    }
    // Search for the index which the data is less than the value
    // i.e. intervals -> [4, 6, 8] then 2 will be < intervals[0]
    // and 5 < intervals[1] but > intervals[0] in this for loop
    // Note: 4 will not be < intervals[0]
    // but between [intervals[0], intervals[1])
    int interval;
    for(auto i = 0; i < this->_intervals->size(); i++)
    {
        if(searchKey < this->_intervals->at(i))
        {
            interval = i;
            break;
        }
    }
    // Recurse down to find the value needed based on index found
    return this->_children->at(interval)->recursive_search(searchKey);
}

/**
 * @brief Recursively step into the tree to insert a value.
 * If the insertion exceeds the fanout-1 space in a leaf
 * this function will recursively split the tree and return
 * a new root if needed.
 * 
 * @param data_record Data record being added to a page/leaf
 * @return BPTreeNode* A potential new root to the tree after
 * a split is completed.
 */
BPTreeNode* BPTreeNode::recursive_insert(const pair<int, int>* data_record)
{
    // Check if the node is a leaf and therefore a page to add values
    // Otherwise we continue to search
    if(this->_isLeaf)
    {
        // Find the location where the new data record can be added
        // to the page/leaf
        if(this->_data->empty())
        {
            this->_data->push_front(make_pair(data_record->first, data_record->second));
            return NULL;
        }
        auto it = this->_data->begin();
        for(; it != this->_data->end(); it++)
        {
            // Add the data in a sorted order in the page
            if(it->first >= data_record->first)
            {
                this->_data->insert(it, make_pair(data_record->first, data_record->second));
                break;
            }
        }
        if(it == this->_data->end())
        {
            this->_data->push_back(make_pair(data_record->first, data_record->second));
        }
        // Check if this caused an overflow
        // thus requring a split on the leaf to accomadate
        // for the size constraints
        if(this->_data->size() > (this->_fanout-1))
        {
            return this->recursive_split();
        }
    }
    else
    {
        // The node is not a leaf and a search must continue
        int i;
        for(i = 0; i < this->_intervals->size(); i++)
        {
            // the interval at i in order to break and add the value
            // to that location.
            if(data_record->first < this->_intervals->at(i))
            {
                // Call a recursive insert to find out where we can place
                // the data record
                return this->_children->at(i)->recursive_insert(data_record);
            }
        }
        if(i >= this->_intervals->size())
        {
            return this->_children->back()->recursive_insert(data_record);
        }
    }
    return NULL;
}

/**
 * @brief Recursively splits the tree at every node
 * until a node no longer needs to split.
 * 
 * @return BPTreeNode* Return a potential new root if the
 * root needs to be split
 */
BPTreeNode* BPTreeNode::recursive_split()
{
    // A leaf node has been found which must be split
    if(this->_isLeaf)
    {
        // Split the list into two based equally (or almost equally) distributed
        int splitPos = this->_data->size() / 2,
            og_size = this->_data->size();
        auto secondList = new list<pair<int, int>>();
        for(int i = splitPos; i < og_size; i++)
        {
            secondList->push_front(this->_data->back());
            this->_data->pop_back();
        }
        // Create a new leaf and then attach the new leaf as the neighbor of the 
        auto newLeaf = new BPTreeNode(true, this->_fanout, secondList, this->_parent, this->_neighbor);
        this->_neighbor = newLeaf;
        

        // This case only happens at the root if the root is a leaf
        if(this->_parent == NULL)
        {
            auto intervals = new vector<int>{secondList->front().first};
            auto bpnode = new BPTreeNode(false, this->_fanout, new vector<BPTreeNode*>{this, newLeaf}, intervals);
            this->_parent = bpnode;
            newLeaf->_parent = bpnode;
            return bpnode;
        }
        
        // Parent exists so we can call a recursive split but before
        // add the new child to the list parent list which will then
        // be checked higher up
        this->_parent->add_child(secondList->front().first, newLeaf);
        return this->_parent->recursive_split();
    }
    else
    {
        if(this->_intervals->size() >= this->_fanout)
        {
            // Find position of split and it value
            auto splitPos = this->_intervals->size() / 2;
            auto parentInterval = this->_intervals->at(splitPos);
            // Create interval and children set for the new node
            auto secondIntervalSet = new vector<int>(this->_intervals->begin() + splitPos + 1, this->_intervals->end());
            auto secondChildrenSet = new vector<BPTreeNode*>(this->_children->begin() + splitPos + 1, this->_children->end());
            // Create the new node
            auto newNode = new BPTreeNode(false, this->_fanout, secondChildrenSet, secondIntervalSet);
            // remove the value from the first node since they belong to the second set
            this->_intervals->erase(this->_intervals->begin() + splitPos, this->_intervals->end());
            this->_children->erase(this->_children->begin() + (splitPos+1), this->_children->end());
            if(this->_parent == NULL)
            {
                // create new node as the parent (root)
                auto bpnode = new BPTreeNode(false, this->_fanout,
                                                new vector<BPTreeNode*>{this, newNode}, new vector<int>{parentInterval});
                this->_parent = bpnode;
                newNode->_parent = bpnode;
                return bpnode;
            }
            else
            {
                // add the new interval and child to the list of nodes
                // call recursive split to see if we added too many
                // intervals to the parent node
                this->_parent->add_child(parentInterval, newNode);
                return this->_parent->recursive_split();
            }
        }
    }
    return NULL;
}

/**
 * @brief Adds a child node to the parent node calling the function
 * 
 * @param leftmostValue Newest interval value of child to add to
 * parent intervals
 * @param child Newest child to add to list of children
 */
void BPTreeNode::add_child(int leftmostValue, BPTreeNode* child)
{
    // Locate where the new value can be added to the intervals
    // and the children nodes (i.e. if intervals can be added to x
    // then it's children will be added between intervals[x] and intervals[x+1])
    // and children will be added to children[x+1])
    if(this->_intervals == NULL && !this->_isLeaf) this->_intervals = new vector<int>();
    int i;
    for(i = 0; i < this->_intervals->size(); i++)
    {
        if(leftmostValue < this->_intervals->at(i))
        {
            this->_intervals->insert(this->_intervals->begin() + i, leftmostValue);
            this->_children->insert(this->_children->begin() + i, child);
            return;
        }
    }
    child->_parent = this;
    this->_intervals->push_back(leftmostValue);
    this->_children->push_back(child);
}

/**
 * @brief Saves all pages to a file named bptree.db
 * 
 */
void BPTreeNode::persist(string dbFile)
{
    auto currentNode = this;
    // Get the leftmost node so that we traverse through
    // every page's neighbors to get the data records
    // without traversing the tree
    while(!currentNode->_isLeaf) currentNode = currentNode->_children->front();

    ofstream file(dbFile);
    if(file.is_open())
    {
        while(currentNode != NULL)
        {
            for(auto it = currentNode->_data->begin(); it != currentNode->_data->end(); it++)
            {
                file << it->first << "," << it->second << "\n";
            }
            currentNode = currentNode->_neighbor;
        }
    }
    file.close();
}

void BPTreeNode::delete_all_nodes()
{
    if(this->_isLeaf)
    {
        delete this->_data;
        this->_data = NULL;
        this->_neighbor = NULL;
        return;
    }

    for(int i = 0; i < _children->size(); i++)
    {
        _children->at(i)->delete_all_nodes();
        delete _children->at(i);
    }
    delete _intervals;
    delete _children;
}

list<pair<int, int>>* BPTreeNode::obtain_all_pages()
{
    auto it = this;

    while(!it->_isLeaf) it = it->_children->front();
    auto data_entries = new list<pair<int, int>>();

    while(it != NULL)
    {
        for(auto i = it->_data->begin(); i != it->_data->end(); i++)
        {
            data_entries->push_back(make_pair(i->first, i->second));
        }
        it = it->_neighbor;
    }

    return data_entries;
}

void BPTreeNode::print_tree()
{
    if(_isLeaf)
    {
        for(auto it = this->_data->begin(); it != this->_data->end(); it++) cout << it->first << " " << it->second << "-";
    }
    else
    {
        for(auto i = 0; i < _intervals->size(); i++) cout << _intervals->at(i) << " ";
        cout << "\n\n";
        for(auto i = 0; i < _children->size(); i++)
        { 
            _children->at(i)->print_tree();
            cout << "\n";
        }
    }
}

/**
 * @brief Uses a similar approach to print_tree to count the number of nodes and height of tree
 *
 */
int BPTreeNode::get_height() {
    auto height = 0;
    auto it = this;
    while(!it->_isLeaf)
    {
        height++;
        it = it->_children->front();
    }
    height++;
    return height;
}

int BPTreeNode::get_number_nodes()
{
    if(this->_isLeaf) return 1;
    else
    {
        auto numNodes = 0;
        for(auto it = this->_children->begin(); it != this->_children->end(); it++)
        {
            numNodes += 1 + (*it)->get_number_nodes();
        }
        return numNodes;
    }
    
}
BPTreeNode* BPTreeNode::insert_page(BPTreeNode *child)
{
    if(this->_children == NULL || this->_children->empty()) 
    {
        this->_children = new vector<BPTreeNode*>();
        this->_children->push_back(child);
        return NULL;
    }

    auto it = this;
    while(!it->_children->back()->_isLeaf) 
    {
        it = it->_children->back();
    }
    it->_intervals->push_back(it->_children->back()->_data->back().first);
    it->_children->push_back(child);
    return it->recursive_split();
}