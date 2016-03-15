#include <random>
#include <iostream>
#include <time.h>
#include <array>
#include <cmath>
#include <string>
#include <sstream>
#include "search.h"

template <class T, class S, class C>
S& Container(priority_queue<T, S, C>& q) {
    struct Queue : private priority_queue<T, S, C> {
        static S& Container(priority_queue<T, S, C>& q) {
            return q.*&Queue::c;
        }
    };
    return Queue::Container(q);
}

Search::Search(int ps, int rs, Graph* gg){
    playerSize = ps;
    resourceSize = rs;
    g=gg;
    buffersize=10;
    buffer = Container(pq);
    iterations=0;
}

Search::~Search(){
}

void Search::run(){
    int max = -1;
    double flipping_factor = 0.1;
    int error_probability = 5; // %100
    int total_iteration = 10000;
    int numberOfFlips = 0;
    int warmupPhase = playerSize; //total_iteration/20;

    for (int i = 0; i < total_iteration; i++){
        buffersize = 10;//+(int)sqrt(i);
        Profile* p;
        if(i<warmupPhase){
            p = new Profile(playerSize, resourceSize);
            p->generateRandomProfile();
        }else{
            Profile* q = sample();
            p = new Profile(q);
            //flip all of the unsatisfied nodes
            //p->flip(g, q->getUnsatisfiedPlayers());
            int denom = q->getUnsatisfiedPlayers().size()-1;
            denom = denom==0?1:denom;
            numberOfFlips = 1+rand()%(denom);
            //for(int j=0;j<1+flipping_factor*total_iteration/i; j++){
            for(int j=0;j<numberOfFlips; j++){
                //p->generateRandomProfile();
                if(rand()%100 < error_probability){
                    //let the algorithm intentionally make mistakes
                    p->flip(g, g->nodes[rand()%playerSize]);
                }else{
                    //choose the next player in a greedy fashion
                    p->flip(g, g->nodes[q->sampleUnsatisfiedPlayer()]);
                }
            }
            p->update(g);
        }
        p->updateRadius(g);
        int s = p->updateSaturation(g);
        int o = p->objectiveSum(g);
        if (o>max) max = o ;
        db.push_back(p);
        push(p);
        //cout << i << ", " << s << ", " << o << " " << numberOfFlips << endl ;
        //p->flip(g, g->nodes[i%playerSize]);
        //p->updateRadius(g);
        //cout << " unsaturated neighbors = " << p->updateSaturation(g) << " " << "cost=" ;
        if(o==playerSize*resourceSize){
            //cout << p->toString() << endl ;
            cout << " Found equilibrium point after " << i << " iterations" << endl ;
            cout << " Maximum Objective= " << max << endl;
            cout << " Buffer ----------- " << endl ;

            //for(int i=0;i<buffersize;i++)
            //    cout << get(i)->cost << endl ;

            return;
        }
        iterations=i;
    }
    cout << "Maximum Objective= " << max << endl;
}

void Search::push(Profile* new_elem){
    if(pq.size()<buffersize){
        pq.push(new_elem);
    }else{
        auto min = pq.top();
        if(new_elem->cost >min->cost){
            pq.pop();
            pq.push(new_elem);
        }
    }
    buffer = Container(pq);
}

Profile* Search::get(int i){
    return buffer[i];
}

Profile* Search::sample(){
    //Uniformly samples the priority queue
    return buffer[rand()%buffer.size()];
    
    //Use weighted sampling for priority queue
    //this will actually increases the number of iterations and slows for about 20%
    std::sort(buffer.begin(), buffer.end());
    for(int i=buffer.size()-1; i>=0; i--){
        if(rand()%10==0) return buffer[i];
    }
    return buffer[0];
}

int Search::getStat(){
    return iterations;
}

//buggy code,
//sort does not uses profile's < operator to sort according to cost
string Search::getOptimalAllocation(){
    std::sort(buffer.begin(), buffer.end());
    for(int i=0;i<buffer.size();i++){
        cout << buffer[i]->toString() << endl ;

    }
    auto best = buffer[0];
    return best->toString();
}
