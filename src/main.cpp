#include <iostream>
#include "graph.h"
#include "profile.h"
#include "search.h"

using namespace std;

int main(int argc, const char * argv[]) {
	Graph* g = new Graph();
	int playerSize = 20;
	int graphConnectivityFactor = 3 ;
	int resourceSize = 10;
	//g->generateRandomGraph(playerSize, graphConnectivityFactor);
	g->generateErdosRenyiRandomGraph(playerSize, 0.4);
    Search* search = new Search(playerSize, resourceSize, g);
    //search->run();
    search->run();
    delete search;
    delete g;
    
    system("PAUSE");
    return 0;
}
