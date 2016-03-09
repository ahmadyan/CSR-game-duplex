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
    buffersize=50;
    buffer = Container(pq);
}

Search::~Search(){
}

void Search::run(){
    Profile* p = new Profile(playerSize, resourceSize);
    p->generateRandomProfile();
    int c0 = p->cost(g->nodes[g->nodes.size() - 1]);
    p->updateRadius(g);
    p->updateSaturation(g);
    cout << " ##############" << endl ;
    
    cout << "cost=" << c0 << endl;
    cout << g->toString() << endl;
    cout << "-------------------" << endl;
    for (int i = 0; i < playerSize; i++){
        cout << "***" << p->updateRadius(g->nodes[i]) << endl;
    }
    cout << "*******************" << endl;
    for (int i = 0; i < playerSize; i++){
        cout << p->radius[i] << endl;
    }
    cout << "*******************" << endl;
    for (int i = 0; i < playerSize; i++){
        cout << p->objective(g->nodes[i]) << endl;
    }
    cout << "*******************" << endl;
    cout << p->objectiveSum(g) << endl;
    int max = -1;
    for (int i = 0; i < 100; i++){
        p->flip(g, g->nodes[i%playerSize]);
        p->updateRadius(g);
        cout << " unsaturated neighbors = " << p->updateSaturation(g) << " " << "cost=" ;
        //p->generateRandomProfile();
        //p->updateRadius(g);
        int o = p->objectiveSum(g);
        if (o>max) max = o ;
        cout << o << endl;
        //p->updateSaturation(g);
    }
    cout << "Maximum Objective= " << max << endl;
    
    for(int i=0;i<10000;i++){
        push(rand()%1000);
    }
    
    for(int i=0;i<20;i++)
        cout << sample() << endl ;
}

void Search::push(int new_elem){
    if(pq.size()<buffersize){
        pq.push(new_elem);
    }else{
        auto max = pq.top();
        if(new_elem<max){
            pq.pop();
            pq.push(new_elem);
        }
    }
    buffer = Container(pq);
}

int Search::get(int i){
    return buffer[i];
}

int Search::sample(){
    return buffer[rand()%buffer.size()];
}
