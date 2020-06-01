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
    return root == NULL ? NULL : root->recursive_search(data);
}

void BPTreeManager::insert(pair<int, int>* record)
{
    if(root == NULL)
    {
        root = new BPTreeNode(true, fanout);
    }
    BPTreeNode* newRoot = root->recursive_insert(record);
    
    if(newRoot != NULL) root = newRoot;
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
    return this->recursive_search(searchKey);
}


BPTreeNode* BPTreeNode::recursive_insert(const pair<int, int>* data_record)
{
    if(this->_isLeaf)
    {
        for(auto it = this->_data->begin(); it != this->_data->end(); it++)
        {
            if(it->first < data_record->first)
            {
                this->_data->insert(it, *data_record);
                break;
            }
        }
        if(this->_data->size() >= this->_fanout-1)
        {
            return recursive_split();
        }
    }
    else
    {
        int interval;

        for(auto i = 0; i < this->_intervals->size(); i++)
        {
            if(data_record->first < this->_intervals->at(i))
            {
                interval = i;
                break;
            }
        }
        return this->_children->at(interval)->recursive_insert(data_record);
    }
}

BPTreeNode* BPTreeNode::recursive_split()
{
    // A leaf node has been found which must be split
    if(this->_isLeaf)
    {
        // Split the list into two based equally (or almost equally) distributed
        // Using the splice functionn the list is split in 2 into a second list
        // and then into a new leaf
        int splitPos = this->_data->size() / 2;
        auto secondList = new list<pair<int, int>>();
        auto og_it = this->_data->begin();
        advance(og_it, splitPos);
        secondList->splice(secondList->begin(), *secondList, og_it, this->_data->end());

        // Create a new leaf and then attach the new leaf as the neighbor of the 
        auto newLeaf = new BPTreeNode(true, this->_fanout, secondList, this->_parent, this->_neighbor);
        this->_neighbor = newLeaf;

        // This case only happens at the root if the root is a leaf
        if(this->_parent == NULL)
        {
            auto intervals = new vector<int>(secondList->front().first);
            return new BPTreeNode(false, this->_fanout, new vector<BPTreeNode*>(this, newLeaf), intervals);
        }
        
        // Parent exists so we can call a recursive split but before
        // add the new child to the list parent list which will then
        // be checked higher up
        this->_parent->add_child(secondList->front().first, newLeaf);
        return this->_parent->recursive_split();
    }
    else
    {
        if(this->_parent == NULL)
        {
            // Find position of split and it value
            auto splitPos = this->_intervals->size() / 2;
            auto parentInterval = this->_intervals->at(splitPos);
            // Create interval and children set for the new node
            auto secondIntervalSet = new vector<int>(this->_intervals->begin() + splitPos + 1, this->_intervals->end());
            auto secondChildrenSet = new vector<BPTreeNode*>(this->_children->begin() + splitPos, this->_children->end());
            // Create the new node
            auto newNode = new BPTreeNode(false, this->_fanout, secondChildrenSet, secondIntervalSet);
            // remove the value from the first node since they belong to the second set
            this->_intervals->erase(this->_intervals->begin() + splitPos, this->_intervals->end());
            this->_children->erase(this->_children->begin() + (splitPos+1), this->_children->end());
            // create new node as the parent (root)
            auto newParent = new BPTreeNode(false, this->_fanout,
                                            new vector<BPTreeNode*>(this, newNode), new vector<int>(parentInterval));
        }
    }
    
}

void BPTreeNode::add_child(int leftmostValue, BPTreeNode* child)
{
    // Locate where the new value can be added to the intervals
    // and the children nodes (i.e. if intervals can be added to x
    // then it's children will be added between intervals[x] and intervals[x+1])
    // and children will be added to children[x+1])
    int i;
    for(i = 0; i < this->_intervals->size(); i++)
    {
        if(leftmostValue < this->_intervals->at(i)) break;
    }
    this->_intervals->insert(this->_intervals->begin() + i, leftmostValue);
    this->_children->insert(this->_children->begin() + (i + 1), child);
}