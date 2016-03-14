#include <iostream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <chrono>
#include "graph.h"
#include "profile.h"
#include "search.h"

using namespace std;
using namespace  boost::accumulators;

int main(int argc, const char * argv[]) {
    accumulator_set<double, stats<tag::variance> > acc;
    accumulator_set<double, stats<tag::variance> > acc2;
    int runs=10;
	int playerSize = 100;
	int graphConnectivityFactor = 3 ;
	int resourceSize = 10;
    std::chrono::milliseconds duration;
    duration.zero();
    for(int i=0;i<runs;i++){
        Graph* g = new Graph();
        //g->generateRandomGraph(playerSize, graphConnectivityFactor);
        g->generateErdosRenyiRandomGraph(playerSize, 0.05);
        Search* search = new Search(playerSize, resourceSize, g);
        auto start = std::chrono::system_clock::now();
        search->run();
        duration += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
        acc(search->getStat());
        delete search;
        delete g;
    }
    duration /= runs;
    cout << " Average simulation time = " << duration.count() << "ms" << endl ;
    cout << " Count : " << boost::accumulators::count(acc) << endl;
    cout<< " Mean  :  "<< mean(acc) << endl;
    //cout<<"moment : "<<moment<2>(acc)<<endl;
    cout<<" deviation : "<< sqrt(variance(acc)) << endl;
    system("PAUSE");
    return 0;
}
