#include <iostream>
#include "../B+ Tree/BPlusTreeNode.h"

using namespace BPTree;
using namespace std;

class UnitTests
{
    private:
        int _fail_count;
        int _total_count;
        bool _output;
    public:
        UnitTests(bool output = true) : _total_count(0), _fail_count(0), _output(output) {}

        void testSingleInsertion_RootShouldOnlyContainOneValue()
        {
            auto bpManager = new BPTreeManager();

            bpManager->insert(new pair<int, int>(1,1));
            auto pages = bpManager->read_pages();

            auto topPage = pages->front();

            if(topPage.first != 1 || topPage.second != 1)
            {
                _fail_count++;

                if(_output) cout << "Single Insert Failed\n"; 

                delete pages;
                return;
            }

            if(_output) cout << "Single Insert Passed\n";
            _total_count++;
        }

        void testThreeInsert_ShouldContainThreeValues_OnReturnOf123SearchKeyAndRecordId()
        {
            auto bpManager = new BPTreeManager(3);

            bpManager->insert(new pair<int, int>(1, 1));
            bpManager->insert(new pair<int, int>(2, 2));
            bpManager->insert(new pair<int, int>(3, 3));

            auto pages = bpManager->read_pages();
            int counter = 1;
            for(auto page = pages->begin(); page != pages->end(); page++)
            {
                if(page->first != counter || page->second != counter)
                {
                    if(_output) cout << "Triple insert Failed at value " << counter << " with values <"<<  page->first << ", " << page->second << ">\n";
                    _fail_count++;
                    return;
                }
                counter++;
            }

            if(_output) cout << "Five insert passed\n";
            _total_count++;
        }

        void testThreeInputsOutOfOrder_ShouldContainThreeValues_OnReturOf123SearchKeyAndRecordId()
        {
            auto bpManager = new BPTreeManager(3);

            bpManager->insert(new pair<int, int>(1, 1));
            bpManager->insert(new pair<int, int>(3, 3));
            bpManager->insert(new pair<int, int>(2, 2));

            auto pages = bpManager->read_pages();
            int counter = 1;
            for(auto page = pages->begin(); page != pages->end(); page++)
            {
                if(page->first != counter || page->second != counter)
                {
                    if(_output) cout << "Triple insert Failed at value " << counter << " with values <"<<  page->first << ", " << page->second << ">\n";
                    _fail_count++;
                    return;
                }
                counter++;
            }

            if(_output) cout << "triple insert passed\n";
            _total_count++;
        }

        void testFiveInputsInOrder_ShouldContainValues12345ForSearchKeyAndRecordId()
        {
            auto bpManager = new BPTreeManager(3, 100);
            
            for(int i = 1; i < 5; i++) bpManager->insert(new pair<int, int>(i, i));

            auto pages = bpManager->read_pages();
            bpManager->print();
            int counter = 1;
            for(auto page = pages->begin(); page != pages->end(); page++)
            {
                if(page->first != counter || page->second != counter)
                {
                    if(_output) cout << "Five insert Failed at value " << counter << " with values <"<<  page->first << ", " << page->second << ">\n";
                    _fail_count++;
                    return;
                }
                counter++;
            }
            if(_output) cout << "five insert passed\n";
            _total_count++;
        }
};  