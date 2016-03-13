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
    int total_iteration = 10000;
    int warmupPhase=total_iteration/20;
    for (int i = 0; i < total_iteration; i++){
        buffersize = 20 + log(i);
        Profile* p;
        if(i<warmupPhase){
            p = new Profile(playerSize, resourceSize);
            p->generateRandomProfile();
        }else{
            Profile* q = sample();
            p = new Profile(q);
            for(int j=0;j<1+0.1*total_iteration/i; j++){
                //p->generateRandomProfile();
                p->flip(g, g->nodes[rand()%playerSize]);
            }
        }

        //int c0 = p->computeCost(g->nodes[g->nodes.size() - 1]);
        p->updateRadius(g);
        int s = p->updateSaturation(g);
        int o = p->objectiveSum(g);

        if (o>max) max = o ;
        db.push_back(p);
        push(p);
        //cout << i << ", " << s << ", " << o<< endl ;
        //p->flip(g, g->nodes[i%playerSize]);
        //p->updateRadius(g);
        //cout << " unsaturated neighbors = " << p->updateSaturation(g) << " " << "cost=" ;
        if(o==playerSize*resourceSize){
            cout << " Found equilibrium point after " << i << " iterations" << endl ;
            cout << " Maximum Objective= " << max << endl;
            cout << " Buffer ----------- " << endl ;

            //for(int i=0;i<buffersize;i++)
            //    cout << get(i)->cost << endl ;
            iterations=i;
            return;
        }
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
    return buffer[rand()%buffer.size()];
}

int Search::getStat(){
    return iterations;
}