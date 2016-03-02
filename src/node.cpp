#include "node.h"
#include <algorithm>
#include <sstream>
#include <string>

Node::Node(int _id){
    id=_id;
}

//creates an edge between this node and node v
//returns true if successfull, returns false if the edge already exists.
bool Node::addEdge(Node * v){
    //first, check if this edge already exists
    auto it = find(adjacencyList.begin(), adjacencyList.end(), v);
    if(it != adjacencyList.end()){
        //the edge between this and node v already exists. do not make duplicate edges
        return false;
    }else{
        // add the edge between this and node v
        v->adjacencyList.push_back(this);
        adjacencyList.push_back(v);
        return true;
    }
}

std::string Node::toString(){
    std::stringstream ss;
    for(int i=0;i<adjacencyList.size();i++){
        ss << adjacencyList[i]->id << ", " ;
    }
    ss << std::endl;
    return ss.str();
}