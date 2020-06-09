#include <iostream>
#include <chrono>
#include "../datagen/datagen.h"
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
            
            for(int i = 1; i < 6; i++) bpManager->insert(new pair<int, int>(i, i));

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

            delete bpManager;
        }

        void testBulkLoad15ItemsInOrder_ReturnShouldContainValues1_15()
        {
            auto bpManager = new BPTreeManager(5,100, true);
            auto dataset = list<pair<int, int>>();
            for(int i = 0; i < 15; i++)
                dataset.push_back(make_pair(i, i));
            bpManager->bulk_load(dataset);

            auto stats = bpManager->get_tree_stats();

            cout << "Height: " << stats.first << "\nNumber of Nodes: " << stats.second << endl;
            bpManager->serialize("dbtree.db");
        }

        void testBulkLoadFromAFile_ReturnShouldContainValues1_10() {
            auto bpManager = new BPTreeManager(6,100, true, true, "../Testing/testFile.db");
            bpManager->print();
        }

        void testTreeStatsFunction_ReturnShouldHaveHeight2_Nodes10() {
            auto bpManager = new BPTreeManager(3,100, true, true, "../Testing/testFile.db");
            bpManager->print();
            std::pair<int, int> stats = bpManager->get_tree_stats();
            std::cout << "Height: " << stats.first << " Number of nodes: " << stats.second << std::endl;
        }

        /**
         * @brief The test function for the single insertion case
         * @param option - The type of data you want to generate. 0 is sorted, 1 is unsorted, 2 is semi-sorted
         */
        void testDirectInsertExperiment(int option) {
            int records = 50000;
            string dataOption;
            chrono::steady_clock::time_point start, end;
            chrono::duration<double> totalTime;
            list<pair<int, int>> data;
            auto bpManager = new BPTreeManager(6, 100000);

            // Begin testing for n = 50,000, 100,000... 1M
            for(int i = 1; i < 21; i++) {
                records  = 50000 * i;

                switch (option) {
                    // Sorted Data Case
                    case 0:
                        dataOption = "Sorted Data";
                        data = generateSortedData(records, 2000000, false);
                        break;

                    // Unsorted Case
                    case 1:
                        dataOption = "Unsorted Data";
                        data = generateUnsortedData(records, 2000000, false);
                        break;

                    // Semi-Sorted Case
                    case 2:
                        dataOption = "Semi-Sorted Data";
                        data = generateSemiSortedData(records, 2000000, false);
                        break;
                }

                // Beginning counting how long it takes to insert all records into tree
                start = std::chrono::steady_clock::now();
                for(auto const& item : data) {
                    bpManager->insert(new pair<int, int>(item.first, item.second));
                }
                end = std::chrono::steady_clock::now();
                totalTime = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

                if(_output) {
                    //pair<int, int> stats = bpManager->get_tree_stats();
                    cout << "---------- Test " << i << " ----------" << endl;
                    cout << "Single Insertion Case (" << dataOption << "): Insertion of " << records << " records into the B+ Tree took " << totalTime.count() << "s" << endl;
                    //cout << "Height after insertion: " << stats.first << "\n" << "Nodes in index: " << stats.second << endl;
                    cout << "---------- End of Test " << i << " ----------\n" << endl;
                }
            }
        }

        /**
         * @brief The test function for the single insertion case
         * @param option - The type of data you want to generate. 0 is sorted, 1 is unsorted, 2 is semi-sorted
         */
        void testBulkLoadingInsertExperiment(int option) {
            int records = 50000;
            string dataOption;
            chrono::steady_clock::time_point start, end;
            chrono::duration<double> totalTime;
            list<pair<int, int>> data;

            // Begin testing for n = 50,000, 100,000... 1M
            for (int i = 1; i < 21; i++) {
                auto bpManager = new BPTreeManager(6, 100000, true);
                records = 50000 * i;

                switch (option) {
                    // Sorted Data Case
                    case 0:
                        dataOption = "Sorted Data";
                        data = generateSortedData(records, 2000000, false);
                        break;

                        // Unsorted Case
                    case 1:
                        dataOption = "Unsorted Data";
                        data = generateUnsortedData(records, 2000000, false);
                        break;

                        // Semi-Sorted Case
                    case 2:
                        dataOption = "Semi-Sorted Data";
                        data = generateSemiSortedData(records, 2000000, false);
                        break;
                }

                // Beginning counting how long it takes to insert all records into tree
                cout << "About to bulk load with " << dataOption << endl;
                start = std::chrono::steady_clock::now();
                bpManager->bulk_load(data);
                end = std::chrono::steady_clock::now();
                std::cout << "done bulk now searching\n";
                auto v = bpManager->search(1);
                std::cout << v->empty() << std::endl;
                cout << "Finished bulk loading!" << endl;
                totalTime = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

                if(_output) {
                    //pair<int, int> stats = bpManager->get_tree_stats();
                    cout << "---------- Test " << i << " ----------" << endl;
                    cout << "Bulk Insertion Case (" << dataOption << "): Insertion of " << records << " records into the B+ Tree took " << totalTime.count() << "s" << endl;
                    //cout << "Height after insertion: " << stats.first << "\n" << "Nodes in index: " << stats.second << endl;
                    cout << "---------- End of Test " << i << " ----------\n" << endl;
                }
            }

        }


};  