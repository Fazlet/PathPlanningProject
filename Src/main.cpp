#include "mission.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Error! Pathfinding task file (XML) is not specified!\n";
        return 0;
    }

    Mission mission(argv[1]);

    std::cout << argv[1] << std::endl;
    std::cout << "Parsing the map from XML:\n";

    if (!(mission.getMap())) {
        std::cout << "Incorrect map! Program halted!\n";
    }
    else {
        std::cout << "Map OK!\nParsing configurations (algorithm, log) from XML:\n";
        if (!(mission.getConfig())) {
            std::cout << "Incorrect configurations! Program halted!\n";
        } else {
            std::cout << "Configurations OK!\nCreating log channel:\n";
            if (!(mission.createLog())) {
                std::cout << "Log chanel has not been created! Program halted!\n";
            } else {
                std::cout << "Log OK!\nStart searching the path:\n";

                mission.createEnvironmentOptions();
                mission.createSearch();
                mission.startSearch();

                std::cout << "Search is finished!\n";

                mission.printSearchResultsToConsole();
                mission.saveSearchResultsToLog();
                mission.printPathToPDF();

                std::cout << "Results are saved (if chosen) via created log channel." << std::endl;
            }
        }
    }
    return 0;
}

