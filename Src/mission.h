#ifndef MISSION_H
#define	MISSION_H

#include "map.h"
#include "config.h"
#include "search.h"
#include "ilogger.h"
#include "searchresult.h"
#include "environmentoptions.h"
#include "search.h"
#include "xmllogger.h"
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>

class Mission
{
    public:
        Mission();
        Mission (const char* fileName);
        ~Mission();

        bool getMap();
        bool getConfig();
        bool createLog();
        void createSearch();
        void createEnvironmentOptions();
        void startSearch();
        void printSearchResultsToConsole();
        void saveSearchResultsToLog();
        void printPathToPDF();
        SearchResult getSearchResult();
        
    private:
        Map                     map;
        Config                  config;
        EnvironmentOptions      options;
        Search                  search;
        ILogger*                logger;
        const char*             fileName;
        SearchResult            sr;
};

#endif

