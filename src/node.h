#pragma once

#include <string>
#include <vector>

class Node{
public:
    bool visited;   //used for BFS
    int id;    
    Node(int);
    std::vector<Node*> adjacencyList;
    bool addEdge(Node*);
    std::string toString();
};