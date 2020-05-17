#include "mission.h"
#include <iostream>

Mission::Mission()
{
    logger = nullptr;
    fileName = nullptr;
}

Mission::Mission(const char *FileName)
{
    fileName = FileName;
    logger = nullptr;
}

Mission::~Mission()
{
    if (logger)
        delete logger;
}

bool Mission::getMap()
{
    return map.getMap(fileName);
}

bool Mission::getConfig()
{
    return config.getConfig(fileName);
}

bool Mission::createLog()
{
    if (logger != nullptr) delete logger;
    logger = new XmlLogger(config.LogParams[CN_LP_LEVEL]);
    return logger->getLog(fileName, config.LogParams);
}

void Mission::createEnvironmentOptions()
{
    options.searchtype = config.SearchParams[CN_SP_ST];
    options.cutcorners = config.SearchParams[CN_SP_CC];
    options.allowsqueeze = config.SearchParams[CN_SP_AS];
    options.allowdiagonal = config.SearchParams[CN_SP_AD];
    if (config.N == 7) {
        options.metrictype = config.SearchParams[CN_SP_MT];
        options.hweight = config.SearchParams[CN_SP_HW];
        options.breakingties = config.SearchParams[CN_SP_BT];
    } else {
        options.metrictype = CN_SP_MT_EUCL;
        options.hweight = 0;
        options.breakingties = CN_SP_BT_GMAX;
    }
}

void Mission::createSearch()
{
//might be helpful in case numerous algorithms are added
}

void Mission::startSearch()
{
    sr = search.startSearch(logger, map, options);
}

void Mission::printSearchResultsToConsole()
{
    std::cout << "Path ";
    if (!sr.pathfound)
        std::cout << "NOT ";
    std::cout << "found!" << std::endl;
    std::cout << "numberofsteps=" << sr.numberofsteps << std::endl;
    std::cout << "nodescreated=" << sr.nodescreated << std::endl;
    if (sr.pathfound) {
        std::cout << "pathlength=" << sr.pathlength << std::endl;
        std::cout << "pathlength_scaled=" << sr.pathlength * map.getCellSize() << std::endl;
    }
    std::cout << "time=" << sr.time << std::endl;
}

void Mission::saveSearchResultsToLog()
{
    logger->writeToLogSummary(sr.numberofsteps, sr.nodescreated, sr.pathlength, sr.time, map.getCellSize());
    if (sr.pathfound) {
        logger->writeToLogPath(*sr.lppath);
        logger->writeToLogHPpath(*sr.hppath);
        logger->writeToLogMap(map, *sr.lppath);
    } else
        logger->writeToLogNotFound();
    logger->saveLog();
}

SearchResult Mission::getSearchResult()
{
    return sr;
}

void Mission::printPathToPDF() {
    if (sr.pathfound) {
        cairo_surface_t *surface; // Определяем поверхность для рисования
        cairo_t *cr;              // Определяем источник
        int x = map.getMapWidth();
        int y = map.getMapHeight();
        surface = cairo_pdf_surface_create("../../Examples/grid.pdf", x, y); // Создаем поверхность для рисования в виде файла PDF

        for (int i = 0; i < y; ++i) {
            cr = cairo_create(surface);
            cairo_set_source_rgb(cr, 166.0/255, 166.0/255, 166.0/255);
            cairo_set_line_width(cr, 0.0001);
            cairo_move_to(cr, 0, i);
            cairo_line_to(cr, x, i);
            cairo_stroke(cr);
            cairo_destroy(cr);
        }
        for (int i = 0; i < x; ++i) {
            cr = cairo_create(surface);
            cairo_set_source_rgb(cr, 166.0/255, 166.0/255, 166.0/255);
            cairo_set_line_width(cr, 0.0001);
            cairo_move_to(cr, i, 0);
            cairo_line_to(cr, i, y);
            cairo_stroke(cr);
            cairo_destroy(cr);
        }

        for (int x1 = 0; x1 < map.getMapWidth(); ++x1) {
            for (int y1 = 0; y1 < map.getMapHeight(); ++y1) {
                if (map.getValue(y1, x1) == 1) {
                    cr = cairo_create(surface);
                    cairo_set_source_rgb(cr, 0, 0, 0);
                    cairo_set_line_width(cr, 0.0001);
                    cairo_move_to(cr, x1, y1);
                    cairo_line_to(cr, x1, y1+1);
                    cairo_line_to(cr, x1+1, y1+1);
                    cairo_line_to(cr, x1+1, y1);
                    cairo_close_path(cr);
                    cairo_fill(cr);
                    cairo_destroy(cr);
                }
            }
        }
        std::list<Node>::const_iterator iter = sr.hppath->begin();
        std::list<Node>::const_iterator it = sr.hppath->begin();
        while (iter != --sr.hppath->end()) {
            int start_x = it->j;
            int start_y = it->i;
            ++iter;
            int finish_x = iter->j;
            int finish_y = iter->i;
            ++it;

            cr = cairo_create(surface);
            cairo_set_source_rgb(cr, 1, 0, 0);
            cairo_set_line_width(cr, 0.1);
            cairo_move_to(cr, start_x + 0.5, start_y + 0.5);
            cairo_line_to(cr, finish_x + 0.5, finish_y + 0.5);
            cairo_stroke(cr);
            cairo_destroy(cr);

            if (start_x != map.getStartJ() || start_y != map.getStartI()) {
                cr = cairo_create(surface);
                cairo_set_source_rgb(cr, 112.0/255, 128.0/255, 144.0/255);
                cairo_set_line_width(cr, 0.001);
                cairo_arc(cr, start_x + 0.5, start_y + 0.5, 0.25, 0, 2*CN_PI_CONSTANT);
                cairo_fill(cr);
                cairo_destroy(cr);
            }
        }

        cr = cairo_create(surface);
        cairo_set_source_rgb(cr, 0, 0, 1);
        cairo_set_line_width(cr, 0.001);
        cairo_arc(cr, map.getStartJ() + 0.5, map.getStartI() + 0.5, 0.25, 0, 2*CN_PI_CONSTANT);
        cairo_fill(cr);
        cairo_destroy(cr);

        cr = cairo_create(surface);
        cairo_set_source_rgb(cr, 0, 0, 1);
        cairo_set_line_width(cr, 0.001);
        cairo_arc(cr, map.getGoalJ() + 0.5, map.getGoalI() + 0.5, 0.25, 0, 2*CN_PI_CONSTANT);
        cairo_fill(cr);
        cairo_destroy(cr);

        cairo_surface_destroy(surface);
    }
}
