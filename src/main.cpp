#include <iostream>
#include "graph.h"
#include "profile.h"

using namespace std;

int main(int argc, const char * argv[]) {
    Graph* g = new Graph();
    int playerSize = 10;
    int graphConnectivityFactor=2;
    int resourceSize = 3;
    //g->generateRandomGraph(playerSize, graphConnectivityFactor);
    g->generateErdosRenyiRandomGraph(playerSize, 0.1);
    Profile* p = new Profile(playerSize, resourceSize);
    p->generateRandomProfile();
    int c0 =p->cost(g->nodes[g->nodes.size()-1]);
    cout << "cost="<< c0 << endl ;
    cout << g->toString() << endl ;
    return 0;
}
