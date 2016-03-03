#pragma once
#include <random>
#include <string>
#include <vector>
#include "node.h"

class Profile{
    int size;
    int resources;
    std::vector<int> allocation; //resource allocation for each nodes
    std::mt19937 rng;
public:
    Profile(int size, int resources);
    ~Profile();
    std::string toString();
    int cost(Node*);
    void generateRandomProfile();
};