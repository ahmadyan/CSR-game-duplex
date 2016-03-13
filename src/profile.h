#pragma once
#include <random>
#include <iostream>
#include <string>
#include <vector>
#include "node.h"
#include "graph.h"

class Profile{
public:
    int size;
    int resources;
	//resource allocation for each nodes, each allocation[i] can take a value between 0..resources-1,
	//indicating player i (\in [0..size-1])is holding resource j (\in [0..resources-1])
    std::vector<int> allocation; 
	//radius for each player: distance between node i and the nearest node other that i holding the same resource
	std::vector<int> radius;
    //determines the number of resources around the neighborhood of size radius(i) of each node i
    std::vector<int> saturation;
    //list of all the players that have unsatisfied neighbors in this allocation
    std::vector<int> unsatisfiedPlayers;
    int cost;
    
    std::mt19937 rng;

    Profile(int size, int resources);
    Profile(Profile* copy);
    ~Profile();
    bool operator<(const Profile&) const; 
    
    std::string toString();
    int computeCost(Node*);
    void generateRandomProfile();
	int updateRadius(Graph* g);
	int updateRadius(Node* v);
	int objective(Node* v);
	int objectiveSum(Graph* g);
    
    int updateSaturation(Graph* g);
    int updateSaturation(Node* v);
    
    void flip(Graph* g, Node* v);
    int sampleUnsatisfiedPlayer();
};


class profileGreaterComparator{
    public: bool operator()(const Profile* lhs, const Profile* rhs) const{
        return lhs->cost > rhs->cost;
    }
};

class profileLesserComparator{
public: bool operator()(const Profile* lhs, const Profile* rhs) const{
    return lhs->cost < rhs->cost;
}
};