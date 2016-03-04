#pragma once
#include <vector>
#include "node.h"

class Graph{
public:
    Graph();
    ~Graph();
    std::vector<Node*> nodes;
    void generateRandomGraph(int size, int p);
    void generateErdosRenyiRandomGraph(int size, double p);
    std::string toString();
};