#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <queue>
#include "graph.h"
#include "profile.h"

using namespace std;

class Search{
    int playerSize;
    int resourceSize;
    Graph* g;
    int iterations;
    int buffersize;
    //priority_queue<int, vector<int>, less<int> > pq;
    //vector<int> buffer;
    priority_queue<Profile*, vector<Profile*>, profileGreaterComparator > pq;
    vector<Profile*> buffer;
    vector<Profile*> db;
public:
    
    Search(int, int, Graph*);
    ~Search();
    void run();
    void push(Profile*);
    Profile* sample();
    Profile* get(int);
    int getStat();
    string getOptimalAllocation();
};