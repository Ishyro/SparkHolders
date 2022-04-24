#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <ncurses.h>

#include <string>
#include <vector>

#include "Values.h"

#define WHITE 0
#define BLUE 1
#define GREEN 2
#define YELLOW 3
#define RED 4

#define ATTRIBUTES_HEIGHT 11
#define ATTRIBUTES_LENGTH 25

#define WAY_HEIGHT 8
#define WAY_LENGTH 20

namespace Display {

  void displayMap(MapDisplay * display, WINDOW * screen);
  void displayTileMap(MapDisplay * display, WINDOW * screen);
  void displayLightMap(MapDisplay * display, WINDOW * screen);
  WINDOW * displayAttributes(Attributes * attributes, int place, int color, WINDOW * screen, int offsetY, int offsetX);
  WINDOW * displayWay(Way * way, int place, int color, WINDOW * screen, int offsetY, int offsetX);
  WINDOW * displayCharacter(Attributes * attributes, Way * race, Way * origin, Way * culture, Way * religion, Way * profession, int color, WINDOW * screen, int sizeX, int offsetY, int offsetX);
  bool compatibleWays(Way * way1, Way * way2, Way * way3, Way * way4, Way * way5, std::list<std::pair<const std::string, const std::string>> waysIncompatibilities);
  std::vector<std::string> selectChoices(std::vector<Attributes *> startingAttributes, std::vector<Way *> startingWays, std::list<std::pair<const std::string, const std::string>> waysIncompatibilities);

};

#endif // _DISPLAY_H_
