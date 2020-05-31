#include "BPlusTreeNode.h"
using namespace BPTree;

void BPTreeManager::insert(pair<int, int>* record)
{
    if(root == NULL)
    {
        root = new BPTreeNode(true, fanout);
    }
    root->recursive_insert(record);
}

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

void BPTreeNode::recursive_insert(const pair<int, int>* data_record)
{
    if(this->_isLeaf)
    {
        if(this->_data->size() != this->_fanout-1)
        {
//            for(auto it = this->_data->begin(); it != this->_data->end(); it++)
//            {
//                if(it->first < data_record->first)
//                    this->
//            }
        }
    }
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