#include "datagen.h"

// Returns a C++ Map that sorts by key. Keys are from 0 to num - 1
// Key values are from 1 - 1000

std::map<int, int> generateSortedData(int num) {
  // This is the sorted input case
  // which will use a map to keep pairs sorted on search key

  std::map<int, int> data;
  srand(time(NULL));
  int val;

  // For loop to initialize elements
  for(int i = 0; i < num; i++) {
    // Generate new random value 1 - 1000
    val = rand() % 1001;
    data.insert(std::pair<int, int>(i, val));
  }

  return data;
}

// Unsorted data sequence
// Returns a C++ Map. Keys are from 0 to num - 1
// Key values are from 1 - 1000
std::unordered_map<int, int> generateUnsortedData(int num){
  std::unordered_map<int, int> data;
  srand(time(NULL));
  int val;

  // For loop to initialize elements
  for(int i = 0; i < num; i++) {
    // Generate new random value 1 - 1000
    val = rand() % 1001;
    data.insert(std::pair<int, int>(i, val));
  }

  return data;

}

// Semi-Sorted Case. Uses Vector to ensure semi-order is kept
std::vector<std::pair<int, int> > generateSemiSortedData(int num) {
  std::vector<std::pair<int, int> > data(num);
  auto myMap = generateSortedData(num);

  srand(time(NULL));
  int val;

  //Now that we have a sorted sequence, we'll use that to iterate and fill in the semi-sorted vector
  for(int i = 0; i < num; i++) {
    for(int j = 0; j < num; j++) {

      // If we meet condition |i - j| <= 0.1n then ai = kj
      if(abs(i - j) <= (0.1 * num)) {
        data[i] = std::pair(j, myMap[j]);
        
        
      }

      // Otherwise ai is just a random selection from the sorted sequence k (Since a is the input sequence)
      else {
        val = rand() % num;
        data[i] = std::pair(val, myMap[val]);
      }
    }
  }
  return data;
}
