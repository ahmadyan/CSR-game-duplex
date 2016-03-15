#include <random>
#include <iostream>
#include <time.h>
#include <array>
#include <cmath>
#include <string>
#include <sstream>
#include <cstdint>
#include "graph.h"

Graph::Graph(){
}

Graph::~Graph(){
    for(int i=0;i<nodes.size();i++){
        delete nodes[i];
    }
}

//incrementally constructs the graph, vertex by vertex
//randomly connects  vertex to at least p other vertices
void Graph::generateRandomGraph(int size, int p){
    std::mt19937 rng;
    std::array<int, 624> seed_data;
    std::random_device r;
    std::generate_n(seed_data.data(), seed_data.size(), std::ref(r));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    rng.seed(seq);
    std::uniform_int_distribution<uint32_t> uint_dist;
    
    for(int i=0;i<size;i++){
        Node* v = new Node(i);
        nodes.push_back(v);
    }
    for(int i=0;i<size;i++){
        if(i>0) nodes[i]->addEdge(nodes[i-1]);  //to ensure connectedness
        for(int j=0;j<p-1;j++){
            auto u = uint_dist(rng)%size;
            nodes[i]->addEdge(nodes[u]);
        }
    }
}

// Generates a random graph of size "size"
// The p is the probabiliy that there exists an edge between two vertices i and j
// Furthermore, we ensure that the graph is connected by connecting every node i to i+1 (without losing any generality)
void Graph::generateErdosRenyiRandomGraph(int size, double p){
    //step 0: setup random number generator
    std::mt19937 rng;
    std::array<int, 624> seed_data;
    std::random_device r;
    std::generate_n(seed_data.data(), seed_data.size(), std::ref(r));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    rng.seed(seq);
    std::uniform_real_distribution<> dis(0, 1);

    //step 1: generate n nodes
    for(int i=0;i<size;i++){
        Node* v = new Node(i);
        nodes.push_back(v);
    }
    
    //step 1: make sure the graph is connected
    for(int i=0;i<size-1;i++){
        nodes[i]->addEdge(nodes[i+1]);
    }
    
    //step 2: randomly generate edges
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(i!=j){
                double prob = dis(rng);
                if(prob<p){
                    nodes[i]->addEdge(nodes[j]);
                }
            }
        }
    }
}

std::string Graph::toString(){
    std::stringstream ss;
    for(int i=0;i<nodes.size();i++){
        ss << i << ":" << nodes[i]->toString() << std::endl ;

    }
    return ss.str();
}