#include "BPlusTreeIndex.h"
using namespace BPTree;

void BPTreeIndex::insert(pair<int, int> *data)
{

}

list<pair<int, int>>* BPTreeIndex::search(const int data)
{
    return recursive_search(this, data);
}

/**
 * @brief 
 * Recurses through tree to find a node with the needed value
 * as well as all the duplicates found
 * @param node Node to begin search
 * @param data Data entry to search
 * @return list<pair<int, int>>* List of <record id, data value>
 */
list<pair<int, int>>* BPTreeIndex::recursive_search(BPTreeIndex *node, const int& data)
{
    // Base case if the tree index is a leaf then we can
    // obtain all the values from the node
    if(node->_isLeaf)
    {
        auto data_entries = new list<pair<int,int>>();
        bool duplicatesExists = true;
        auto current = node;
        
        // Duplicates can exist and thus the easiest way to obtain them is to
        // search first for the left most leaf then traverse to the right
        // on the leaves and fill a list to return.
        while(duplicatesExists)
        {
            for(auto it = node->_data->begin(); it != node->_data->end(); it++)
            {
                if(it->second == data)
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
            if(current == NULL) duplicatesExists = false;
       }
       return data_entries;
    }
    
    // Search for the index which the data is less than the value
    // i.e. intervals -> [4, 6, 8] then 2 will be < intervals[0]
    // and 5 < intervals[1] but > intervals[0] in this for loop
    // Note: 4 will not be < intervals[0]
    // but between [intervals[0], intervals[1])
    int interval;
    for(auto i = 0; i < node->_intervals->size(); i++)
    {
        if(data < node->_intervals->at(i))
        {
            interval = i;
            break;
        }
    }
    // Recurse down to find the value needed based on index found
    return recursive_search(node->_children->at(interval), data);
}

void bulk_load(list<pair<int, int>> data)
{

}