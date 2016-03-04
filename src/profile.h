#pragma once
#include <random>
#include <string>
#include <vector>
#include "node.h"

class Profile{
    int size;
    int resources;
	//resource allocation for each nodes, each allocation[i] can take a value between 0..resources-1,
	//indicating player i (\in [0..size-1])is holding resource j (\in [0..resources-1])
    std::vector<int> allocation; 
	//radius for each player: distance between node i and the nearest node other that i holding the same resource
	std::vector<int> radius;
    std::mt19937 rng;
public:
    Profile(int size, int resources);
    ~Profile();
    std::string toString();
    int cost(Node*);
    void generateRandomProfile();
	int updateRadius(Node* v);
};