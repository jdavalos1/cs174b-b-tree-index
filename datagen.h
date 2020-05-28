#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <cstdlib>

#include<stdlib.h>
#include <time.h>


std::map<int, int> generateSortedData(int num);
std::unordered_map<int, int> generateUnsortedData(int num);
std::vector<std::pair<int, int> > generateSemiSortedData(int num);