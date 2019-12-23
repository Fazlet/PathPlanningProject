#include "search.h"

Search::Search()
{
    hweight = 1;
}

Search::~Search() {}

SearchResult Search::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options)
{
    std::chrono::time_point<std::chrono::system_clock> start, finish;
    start = std::chrono::system_clock::now();

    Node currentNode;
    currentNode.parent = nullptr;
    currentNode.i = map.getStartI();
    currentNode.j = map.getStartJ();
    currentNode.g = 0;
    currentNode.H = computeHeuristic(currentNode.i, currentNode.j, map.getGoalI(), map.getGoalJ(), options);
    currentNode.F = currentNode.g + (hweight * currentNode.H);

    OPEN.insert({currentNode.i + (currentNode.j * map.getMapHeight()), currentNode});

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
                it->parent = &(CLOSE.find(currentNode.i + (map.getMapHeight() * currentNode.j))->second);
                OPEN.insert({it->i + (it->j * map.getMapHeight()), *it});
            } else if (it->g < OPEN[it->i + (it->j * map.getMapHeight())].g) {
                OPEN[it->i + (it->j * map.getMapHeight())].g = it->g;
                OPEN[it->i + (it->j * map.getMapHeight())].parent = &(CLOSE.find(currentNode.i + (map.getMapHeight() * currentNode.j))->second);
            }
        }

    }

    makePrimaryPath(currentNode);
    sresult.pathlength = currentNode.g;

    finish = std::chrono::system_clock::now();
    sresult.time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count()) / 1000000000;

    makeSecondaryPath();

    sresult.pathfound = pathFound;
    sresult.nodescreated = CLOSE.size() + OPEN.size();
    sresult.numberofsteps = CLOSE.size();

    sresult.hppath = &hppath;
    sresult.lppath = &lppath;

    return sresult;
}

double Search::computeHeuristic(int a1, int b1, int a2, int b2, const EnvironmentOptions &options)
{
    double H = 0.0;

    if (options.metrictype == 0) {
        if (abs(a1 - a2) < abs(b1 - b2)) {
            H = (sqrt(2) * abs(a1 - a2)) + (abs(b1 - b2) - abs(a1 - a2));
        } else {
            H = (sqrt(2) * abs(b1 - b2)) + (abs(a1 - a2) - abs(b1 - b2));
        }
    }

    if (options.metrictype == 1) {
        H = abs(a1 - a2) + abs(b1 - b2);
    }

    if (options.metrictype == 2) {
        H = sqrt((a1 - a2)*(a1 - a2) + (b1 - b2)*(b1 - b2));
    }

    if (options.metrictype == 3) {
        if (abs(a1 - a2) > abs(b1 - b2)) {
            H = abs(a1 - a2);
        } else {
            H = abs(b1 - b2);
        }
    }

    return H;
}

std::list<Node> Search::getNeighbors(Node currentNode, const Map &map, const EnvironmentOptions &options)
{
    std::list<Node> neighbors;
    Node neighbor;
    bool noWay = false;

    for (int down = -1; down < 2; ++down) {
        for (int right = -1; right < 2; ++right) {
            if ((down != 0) || (right != 0)) {

                if (map.CellOnGrid(currentNode.i + down, currentNode.j + right) &&
                    map.CellIsTraversable(currentNode.i + down, currentNode.j + right)) {

                    if ((down != 0) && (right != 0)) {
                        if ((map.CellIsObstacle(currentNode.i + down, currentNode.j)) &&
                            (map.CellIsObstacle(currentNode.i, currentNode.j + right)) &&
                            (!(options.allowsqueeze))) {
                            noWay = true;
                        }

                        if (!(options.allowdiagonal)) {
                            noWay = true;
                        }

                        if (((map.CellIsObstacle(currentNode.i + down, currentNode.j)) ||
                            (map.CellIsObstacle(currentNode.i, currentNode.j + right))) &&
                            (!(options.cutcorners))) {
                            noWay = true;
                        }
                    }

                    if ((!(noWay)) &&
                        (CLOSE.find((currentNode.i + down) + (map.getMapHeight() * (currentNode.j + right))) == CLOSE.end())) {
                        neighbor.i = currentNode.i + down;
                        neighbor.j = currentNode.j + right;
                        if ((down != 0) && (right != 0)) {
                            neighbor.g = currentNode.g + sqrt(2);
                        } else {
                            neighbor.g = currentNode.g + 1;
                        }
                        neighbor.H = computeHeuristic(neighbor.i, neighbor.j, map.getGoalI(), map.getGoalJ(), options);
                        neighbor.F = neighbor.g + (hweight * neighbor.H);

                        neighbors.push_front(neighbor);
                    }
                }
            }
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
    Node thisNode = currentNode;

    while (thisNode.parent) {
        lppath.push_front(thisNode);
        thisNode = *(thisNode.parent);
    }
    lppath.push_front(thisNode);
}

void Search::makeSecondaryPath()
{
    auto it = lppath.begin();
    int currentNode_i, currentNode_j, nextNode_i, nextNode_j;
    hppath.push_back(*it);

    while (it != --lppath.end()) {
        currentNode_i = it->i;
        currentNode_j = it->j;

        ++it;
        nextNode_i = it->i;
        nextNode_j = it->j;

        ++it;

        if (((it->i - nextNode_i) != (nextNode_i - currentNode_i)) || ((it->j - nextNode_j) != (nextNode_j - currentNode_j))) {
            hppath.push_back(*(--it));
        } else {
            --it;
        }
    }
}
