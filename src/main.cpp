#include <iostream>
#include "graph.h"

using namespace std;

int main(int argc, const char * argv[]) {
    Graph* g = new Graph();
    g->generateRandomGraph(100, 5);
    cout << g->toString() << endl ;

}
