// Returns a C++ list that sorts by key value. Keys are from 0 to num - 1
// Key values are from 1 - range
// If you want distinct key values, set distinct to true. NOTE range has to be greater than num of keys to ensure enough values for distinctness

std::list<std::pair<int, int> > generateSortedData(int num, int range, bool distinct) {
  // This is the sorted input case
  // which will use a list to keep pairs sorted on search key value
  if(distinct) {
    assert(range >= num && "Range of values must be more than key slots to ensure distinctness");
  }

  std::list<std::pair<int, int> > data;
  srand(time(NULL));
  int val = rand() % (range + 1);

  // For loop to initialize elements
  for(int i = 0; i < num; i++) {

    // If distinct then we will increment sequence instead of making random data
    if(distinct) {
      val++;
    }
    else {
      // Generate new random value 1 - range
      val = rand() % (range + 1);
    }

    data.push_back(std::pair<int, int>(i, val));
  }

  // Sorting list using list::sort method with passed in lambda sort
  // If distinct, then we are just essentially reversing list
  data.sort([](auto const& a, auto const& b) {
    return a.second > b.second;
  });

  return data;
}

// Unsorted data sequence
// Returns a C++ list. Keys are from 0 to num - 1
// Key values are from 1 - range
std::list<std::pair<int, int> > generateUnsortedData(int num, int range, bool distinct){

  std::list<std::pair<int, int> > data;
  srand(time(NULL));
  int val = rand() % (range + 1);
  
  if(distinct) {
    assert(range >= num && "Range of values must be more than key slots to ensure distinctness");

    std::unordered_set<int> distinctValues;
    int distinctCounter = 0;
    do {
      val = rand() % (range + 1);
      auto result = distinctValues.insert(val);
      if(result.second) {
        distinctCounter++;
      }

    } while(distinctCounter < num);


    int count = 0;
    for(auto const& item : distinctValues){
      data.push_back(std::pair<int, int>(count, item));
      count++;
    }

    return data;
  }


  // Non-Distinct case

  // For loop to initialize elements
  for(int i = 0; i < num; i++) {

    // Generate new random value 1 - range
    val = rand() % (range + 1);
    data.push_back(std::pair<int, int>(i, val));
  }

  return data;

}


// Helper Rotation function
/*Fuction to get gcd of a and b*/
int gcd(int a, int b) 
{ 
    if (b == 0) 
        return a; 
  
    else
        return gcd(b, a % b); 
} 
  
/*Function to left rotate arr[] of size n by d*/
void leftRotate(int* arr, int d, int n) 
{ 
    /* To handle if d >= n */
    d = d % n; 
    int g_c_d = gcd(d, n); 
    for (int i = 0; i < g_c_d; i++) { 
        /* move i-th values of blocks */
        int temp = arr[i]; 
        int j = i; 
  
        while (1) { 
            int k = j + d; 
            if (k >= n) 
                k = k - n; 
  
            if (k == i) 
                break; 
  
            arr[j] = arr[k]; 
            j = k; 
        } 
        arr[j] = temp; 
    }
 
}




// Semi-Sorted Case. Uses Vector to ensure semi-order is kept
std::list<std::pair<int, int> > generateSemiSortedData(int num, int range, bool distinct) {

  std::list<std::pair<int, int> > data;
  std::vector<int> sorted_ints;
  int val;
  int semiSortFactor = floor(num * 0.1);


  if(distinct) {
    assert(range >= num && "Range of values must be more than key slots to ensure distinctness");

    std::unordered_set<int> distinctValues;
    int distinctCounter = 0;

    // Generate distinct values
    do {
      val = rand() % (range + 1);
      auto result = distinctValues.insert(val);
      if(result.second) {
        distinctCounter++;
      }

    } while(distinctCounter < num);

    // Load into vector
    for(auto const& item : distinctValues) {
      sorted_ints.push_back(item);
    }

    // Sort the vector
    std::sort(std::begin(sorted_ints), std::end(sorted_ints));

    // Rotate the vector by 10% of sorted values
    leftRotate(sorted_ints.data(), semiSortFactor, sorted_ints.size());

    for(int i = 0; i < sorted_ints.size(); i++) {
      data.push_back(std::pair<int, int>(i, sorted_ints[i]));
    }

    return data;
  }


  // Load a vector with non-distinct values
  for(int i = 0; i < num; i++) {
    val = rand() % (range + 1);
    sorted_ints.push_back(val);
  }

  // Rotate!
  leftRotate(sorted_ints.data(), semiSortFactor, sorted_ints.size());

  // Insert into list
  for(int i = 0; i < sorted_ints.size(); i++) {
      data.push_back(std::pair<int, int>(i, sorted_ints[i]));
    }

  return data;
}
