#include "profile.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include <limits>
#include <set>
#include <list>
#include <cstdint>
#include <numeric>

Profile::Profile(int s, int r){
    size=s;
    resources=r;
    allocation = std::vector<int>(size);
	radius = std::vector<int>(size);
    saturation = std::vector<int>(size);
    std::array<int, 624> seed_data;
    std::random_device random_device;
    std::generate_n(seed_data.data(), seed_data.size(), std::ref(random_device));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    rng.seed(seq);
}

Profile::~Profile(){
}

void Profile::generateRandomProfile(){
    std::uniform_int_distribution<uint32_t> uint_dist;
    // ensures no missing resources in the profile
    for(int i=0;i<resources;i++){
        allocation[i] = i;
    }
    //fill up the rest of the profile randomly
    for(int i=resources;i<size;i++){
        allocation[i] = uint_dist(rng)%resources;
    }
    //shuffle the allocation a couple of times
    for(int i=0;i<resources*size;i++){
        std::swap(allocation[uint_dist(rng)%size], allocation[uint_dist(rng)%size]);
        std::next_permutation(allocation.begin(), allocation.end());
    }
}

std::string Profile::toString(){
    std::stringstream ss;
    for(int i=0;i<size;i++){
        ss << allocation[i] << ", " ;
    }
    ss << std::endl;
    return ss.str();
}

int Profile::updateSaturation(Graph* g){
    auto unsaturatedNeighbors=0;
    for (auto i = 0; i < size; i++){
        saturation[i] = updateSaturation(g->nodes[i]);
        if(saturation[i]<=sqrt(resources))
            unsaturatedNeighbors++;
    }
    std::cout << " UNSATURATED NEIGHBORS = " << unsaturatedNeighbors << std::endl ;
    return unsaturatedNeighbors;
}

int Profile::updateSaturation(Node* v){
    auto r = radius[v->id];
    std::list<Node*> q; //bfs queue
    std::vector<int> distance(size);
    std::vector<bool> visited(size);
    std::vector<int> visitedResources(resources);
    
    for (int i = 0; i < size; i++){
        visited[i] = false;
        distance[i] = std::numeric_limits<int>::max();
    }
    for(int i=0;i<resources;i++){
        visitedResources[i]=0;
    }
    distance[v->id] = 0;
    visited[v->id] = true;
    q.push_back(v);

    while (!q.empty()){
        auto u = q.front(); q.pop_front();
        for (int i = 0; i<u->adjacencyList.size(); i++){
            auto w = u->adjacencyList[i];
            if (!visited[w->id]){
                if (distance[w->id] > distance[u->id] + 1){
                    distance[w->id] = distance[u->id] + 1;
                }
                visited[w->id] = true;
                if(distance[w->id] < r){
                    q.push_back(w);
                    visitedResources[allocation[w->id]]=1;
                }
            }
        }
    }
    return std::accumulate(visitedResources.begin(), visitedResources.end(), 0);
}

int Profile::updateRadius(Graph* g){
    for (int i = 0; i < size; i++){
        radius[i] = updateRadius(g->nodes[i]);
    }
    return 0;
}

//Find the closest node to node v that has the same allocation as v
int Profile::updateRadius(Node* v){
	int result = size;	//ideally should be diameter of the graph+1, but size would be fine
	std::list<Node*> q; //bfs queue
	std::vector<int> distance(size);
	std::vector<bool> visited(size);
	for (int i = 0; i < size; i++){
		visited[i] = false;
		distance[i] = std::numeric_limits<int>::max();
	}
	distance[v->id] = 0;
	visited[v->id] = true;
	q.push_back(v);

	while (!q.empty()){
		auto u = q.front(); q.pop_front();
		for (int i = 0; i<u->adjacencyList.size(); i++){
			auto w = u->adjacencyList[i];
			if (!visited[w->id]){
				if (distance[w->id] > distance[u->id] + 1){
					distance[w->id] = distance[u->id] + 1;
				}
				if (allocation[w->id] == allocation[v->id]){
					if (distance[w->id] < result){
						//std::cout << w->id << " " << v->id << " " << allocation[w->id] << " " <<allocation[v->id] << " " << distance[w->id] << " " << result << std::endl;
						result = distance[w->id];
					}
				}
				visited[w->id] = true;
				q.push_back(w);
			}
		}
	}
	return result;
}

int Profile::objectiveSum(Graph* g){
	int result = 0;
	for (int i = 0; i < size; i++){
		result += objective(g->nodes[i]);
	}
	return result;
}

//the objective function computes the number of different resources in the ball of size r_i around node v
int Profile::objective(Node* v){
	int r = radius[v->id];
	std::list<Node*> q; //bfs queue
	std::vector<int> distance(size);
	std::vector<bool> visited(size);
	for (int i = 0; i < size; i++){
		visited[i] = false;
		distance[i] = std::numeric_limits<int>::max();
	}
	distance[v->id] = 0;
	visited[v->id] = true;
	q.push_back(v);
	bool ball_covered = false; 
	while(!ball_covered & !q.empty()){
		auto u = q.front(); q.pop_front();
		for (int i = 0; i<u->adjacencyList.size(); i++){
			auto w = u->adjacencyList[i];
			if (!visited[w->id]){			
				if (distance[w->id] > distance[u->id] + 1){
					distance[w->id] = distance[u->id] + 1;
				}			
				if (distance[w->id] > r){
					ball_covered = true;
				}
				visited[w->id] = true;
				q.push_back(w);
			}
		}
	}

	int ball = 0;
	for (int i = 0; i < size; i++){
		if (visited[i]){
			ball++;
		}
	}
	return ball;
}

// computes the sum cost for the player v
// To compute the sum cost, we execute breath-first search from node v
// along the path, whenever we see a resource, we update it's cost (the distance from node v)
// we continue until we have visited all resources or we finish the graph
int Profile::cost(Node* v){
    std::list<Node*> q; //bfs queue
    std::vector<bool> visited(size);
    std::vector<int> distance(size);
    std::vector<int> mininimumResourceDistance(resources);
    for(int i=0;i<size;i++){
        visited[i]=false;
        distance[i]=std::numeric_limits<int>::max();
    }
    for(int i=0;i<resources;i++){
        mininimumResourceDistance[i]=std::numeric_limits<int>::max();
    }
    distance[v->id]=0;
    visited[v->id]=true;
    mininimumResourceDistance[allocation[v->id]]=0;
    
    q.push_back(v);

    //The resources that we have not visited so far
    std::set<int> resource;
    for(int i=0;i<resources;i++){
        resource.insert(i);
    }
    resource.erase(allocation[v->id]);
    
    //BFS loop
    while(! (q.empty() || resource.empty() )){
        auto u = q.front(); q.pop_front();
        for(int i=0;i<u->adjacencyList.size();i++){
            auto w = u->adjacencyList[i];
            if(!visited[w->id]){
                if(distance[w->id] > distance[u->id]+1){
                    distance[w->id] = distance[u->id]+1;
                }
                if(mininimumResourceDistance[allocation[w->id]] > distance[w->id]){
                    mininimumResourceDistance[allocation[w->id]] = distance[w->id];
                }
                resource.erase(allocation[w->id]);
                //std::cout << "visiting " << w->id << " " << allocation[w->id]<< std::endl ;
                visited[w->id]=true;
                q.push_back(w);
            }
        }
    }
    //for(int i=0;i<mininimumResourceDistance.size();i++){
    //    std::cout << "min["<<i << "] = " << mininimumResourceDistance[i]  << std::endl ;
    //}
    int result = std::accumulate(mininimumResourceDistance.begin(), mininimumResourceDistance.end(), 0);
    return result;
}
