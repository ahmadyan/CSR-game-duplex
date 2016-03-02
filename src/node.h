#pragma once

#include <string>
#include<vector>

class Node{
public:
    Node(int);
    int id;
    int color;
    std::vector<Node*> adjacencyList;
    bool addEdge(Node*);
    std::string toString();
};