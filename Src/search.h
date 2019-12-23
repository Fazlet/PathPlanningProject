#ifndef SEARCH_H
#define SEARCH_H
#include <chrono>
#include "environmentoptions.h"
#include "ilogger.h"
#include <math.h>
#include <limits>
#include <list>
#include "searchresult.h"
#include <unordered_map>
#include <vector>

class Search
{
    public:
        Search();
        ~Search(void);
        SearchResult startSearch(ILogger *Logger, const Map &Map, const EnvironmentOptions &options);

    protected:
        //CODE HERE

        virtual Node findMin();

        virtual void makePrimaryPath(Node currentNode);
        virtual void makeSecondaryPath();

        //Hint 1. You definetely need class variables for OPEN and CLOSE

        //Hint 2. It's a good idea to define a heuristic calculation function, that will simply return 0
        //for non-heuristic search methods like Dijkstra

        virtual double computeHeuristic(int a1, int b1, int a2, int b2, const EnvironmentOptions &options);
        
        //Hint 3. It's a good idea to define function that given a node (and other stuff needed)
        //will return it's sucessors, e.g. unordered list of nodes
        
        virtual std::list<Node> getNeighbors(Node currentNode, const Map &map, const EnvironmentOptions &options);

        //Hint 4. working with OPEN and CLOSE is the core
        //so think of the data structures that needed to be used, about the wrap-up classes (if needed)
        //Start with very simple (and ineffective) structures like list or vector and make it work first
        //and only then begin enhancement!

        struct compare
        {
            bool operator()(const Node& one, const Node& two)
            {
                return one.F < two.F;
            }
        };

        SearchResult                                       sresult; //This will store the search result
        std::list<Node>                                    lppath, hppath;
        std::unordered_map<int, Node>                      OPEN;
        std::unordered_map<int, Node>                      CLOSE;
        double                                             hweight;
        //CODE HERE to define other members of the class
};
#endif
