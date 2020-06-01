//#include<stdlib.h>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <list>
#include <unordered_set>
#include <algorithm>
#include <time.h>


std::list<std::pair<int, int> > generateSortedData(int num, int range, bool distinct);
std::list<std::pair<int, int> > generateUnsortedData(int num, int range, bool distinct);
int gcd(int a, int b);
void leftRotate(int* arr, int d, int n);
std::list<std::pair<int, int> > generateSemiSortedData(int num, int range, bool distinct);
