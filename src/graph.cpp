#include <random>
#include <time.h>
#include <array>
#include <cmath>
#include <string>
#include <sstream>
#include "graph.h"


Graph::Graph(){
}

Graph::~Graph(){
    for(int i=0;i<nodes.size();i++){
        delete nodes[i];
    }
}

//very simple random graph generator
//incrementally constructs the graph, vertex by vertex
//randomly connects the new vertex to at least p other vertices (no guarantees for the first p-vertices)
//todo: replace this with Erdos random graphs
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
        int j=0;
        while(j<std::min(i,p)){
            auto u = uint_dist(rng)%i;
            if(v->addEdge(nodes[u])){
                j++;
            }
        }
        nodes.push_back(v);
    }
}

std::string Graph::toString(){
    std::stringstream ss;
    for(int i=0;i<nodes.size();i++){
        ss << i << ":" << nodes[i]->toString() << std::endl ;

    }
    return ss.str();
}