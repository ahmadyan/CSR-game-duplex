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
    
    int buffersize;
    priority_queue<int, vector<int>, less<int> > pq;
    vector<int> buffer;
public:
    Search(int, int, Graph*);
    ~Search();
    void run();
    void push(int);
    int sample();
    int get(int);
};