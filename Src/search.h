#ifndef SEARCH_H
#define SEARCH_H
#include <chrono>
#include <cmath>
#include "environmentoptions.h"
#include "ilogger.h"
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
        virtual Node findMin(const EnvironmentOptions &options);
        virtual double computeHeuristic(int a1, int b1, int a2, int b2, const EnvironmentOptions &options);
        virtual std::list<Node> getNeighbors(Node currentNode, const Map &map, const EnvironmentOptions &options);

        virtual void makePrimaryPath(Node currentNode);
        virtual void makeSecondaryPath();

        virtual bool lineOfSight(int a1, int b1, int a2, int b2, const Map &map, const EnvironmentOptions);
        virtual Node changeParent(Node currentNode, Node parentNode, const Map &map, const EnvironmentOptions &options);

        SearchResult                                       sresult;
        std::list<Node>                                    lppath, hppath;
        std::unordered_map<int, Node>                      OPEN;
        std::unordered_map<int, Node>                      CLOSE;
};
#endif
