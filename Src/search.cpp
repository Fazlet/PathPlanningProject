#include <queue>
#include "search.h"

Search::Search()
{
//set defaults here
}

Search::~Search() {}

SearchResult Search::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options)
{
    //need to implement

    std::chrono::time_point<std::chrono::system_clock> start, finish;
    start = std::chrono::system_clock::now();

    Node currentNode;
    currentNode.i = map.getStartI();
    currentNode.j = map.getStartJ();
    currentNode.g = 0;
    currentNode.H = computeHeuristic(currentNode.i, currentNode.j, map.getGoalI(), map.getGoalJ(), options);
    currentNode.F = currentNode.g + (hweight * currentNode.H);

    OPEN.insert({currentNode.i + (currentNode.j * map.getMapHeight()), currentNode});

    unsigned int nodesCreated = 1;

    bool pathFound = false;

    while (!(OPEN.empty())) {
        currentNode = findMin();

        CLOSE.insert({currentNode.i + (currentNode.j * map.getMapHeight()), currentNode});
        OPEN.erase(currentNode.i + (currentNode.j * map.getMapHeight()));

        if ((currentNode.i == map.getGoalI()) && (currentNode.j == map.getGoalJ())) {
            pathFound = true;
            break;
        }

        std::list<Node> neighbors = getNeighbors(currentNode, map, options);

        for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
            if (OPEN.find(it->i + (it->j * map.getMapHeight())) == OPEN.end()) {
                OPEN.insert({it->i + (it->j * map.getMapHeight()), *it});
                ++nodesCreated;
            } else if (it->g < OPEN[it->i + (it->j * map.getMapHeight())].g) {
                OPEN[it->i + (it->j * map.getMapHeight())].g = it->g;
                OPEN[it->i + (it->j * map.getMapHeight())].parent = it->parent;
            }
        }
    }

    finish = std::chrono::system_clock::now();
    sresult.time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count()) / 1000000000;

    sresult.pathfound = pathFound;
    sresult.nodescreated = nodesCreated;

    /*sresult.numberofsteps = ;
    sresult.hppath = &hppath; //Here is a constant pointer
    sresult.lppath = &lppath;*/

    return sresult;
}

double Search::computeHeuristic(int a1, int b1, int a2, int b2, const EnvironmentOptions &options)
{
    double H = 0.0;
    return H;
}

std::list<Node> Search::getNeighbors(Node currentNode, const Map &map, const EnvironmentOptions &options)
{
    std::list<Node> neighbors;
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {

        }
    }
    return neighbors;
}

Node Search::findMin()
{
    Node minNode;
    minNode = (OPEN.begin())->second;
    for (auto it = OPEN.begin(); it != OPEN.end(); ++it) {
        if (it->second.F < minNode.F) {
            minNode = it->second;
        }
    }
    return minNode;
}

void Search::makePrimaryPath(Node currentNode)
{
    //need to implement
}

void Search::makeSecondaryPath()
{
    //need to implement
}
