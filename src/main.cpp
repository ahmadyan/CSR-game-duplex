#include <iostream>
#include "graph.h"
#include "profile.h"

using namespace std;

int main(int argc, const char * argv[]) {
	Graph* g = new Graph();
	int playerSize = 20;
	int graphConnectivityFactor = 2;
	int resourceSize = 5;
	//g->generateRandomGraph(playerSize, graphConnectivityFactor);
	g->generateErdosRenyiRandomGraph(playerSize, 0.05);
	Profile* p = new Profile(playerSize, resourceSize);
	p->generateRandomProfile();
	int c0 = p->cost(g->nodes[g->nodes.size() - 1]);
	cout << "cost=" << c0 << endl;
	cout << g->toString() << endl;
	cout << "-------------------" << endl;
	for (int i = 0; i < playerSize; i++){
		cout << "***" << p->updateRadius(g->nodes[i]) << endl;	
	}
	system("PAUSE");
    return 0;
}
