#include <iostream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include "graph.h"
#include "profile.h"
#include "search.h"

using namespace std;
using namespace  boost::accumulators;

int main(int argc, const char * argv[]) {
    accumulator_set<double, stats<tag::variance> > acc;
    accumulator_set<double, stats<tag::variance> > acc2;
    int runs=10;

	int playerSize = 20;
	int graphConnectivityFactor = 3 ;
	int resourceSize = 10;
    for(int i=0;i<runs;i++){
        Graph* g = new Graph();
        //g->generateRandomGraph(playerSize, graphConnectivityFactor);
        g->generateErdosRenyiRandomGraph(playerSize, 0.4);
        Search* search = new Search(playerSize, resourceSize, g);
        //search->run();
        search->run();
        acc(search->getStat());
        delete search;
        delete g;
        
    }
    
    cout << "Count : " << boost::accumulators::count(acc) << endl;
    cout<< "Mean  :  "<< mean(acc) << endl;
    //cout<<"moment : "<<moment<2>(acc)<<endl;
    cout<<"deviation : "<< sqrt(variance(acc)) << endl;

    
    system("PAUSE");
    return 0;
}
