#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <ncurses.h>

#include <string>
#include <vector>
#include <list>

#include "clients/Link.h"

#include "Values.h"

#define WHITE 0
#define BLUE 1
#define GREEN 2
#define YELLOW 3
#define RED 4
#define BACK_RED 5

#define ATTRIBUTES_HEIGHT 12
#define ATTRIBUTES_WIDTH 25

#define WAY_HEIGHT 9
#define WAY_WIDTH 25

namespace Display {

  void print(WINDOW* screen, int offsetY, int offsetX, std::string to_print);
  void displayMap(MapDisplay * display, Character * player, WINDOW * screen, Translator * t);
  void displayTileMap(MapDisplay * display, WINDOW * screen, Translator * t);
  void displayLightMap(MapDisplay * display, WINDOW * screen, Translator * t);
  void displayStats(Character * player, WINDOW * screen, Translator * t);
  WINDOW * displaySkill(Skill * skill, WINDOW * screen, Translator * t);
  void displayTarget(CharacterDisplay * target, WINDOW * screen, Translator * t);
  WINDOW * displayWeapon(Weapon * weapon, WINDOW * screen, Translator * t);
  WINDOW * displayAmmo(Ammunition * ammo, WINDOW * screen, Translator * t);
  WINDOW * displayItem(Item * item, WINDOW * screen, Translator * t);
  void displayCommands(WINDOW * screen, Translator * t);
  WINDOW * displayAttributes(Attributes * attributes, int place, int color, WINDOW * screen, int offsetY, int offsetX, Translator * t);
  WINDOW * displayWay(Way * way, int place, int color, WINDOW * screen, int offsetY, int offsetX, Translator * t);
  WINDOW * displayCharacter(Attributes * attributes, Way * race, Way * origin, Way * culture, Way * religion, Way * profession, int color, WINDOW * screen, int sizeX, int offsetY, int offsetX, Translator * t);
  bool compatibleWays(Way * way1, Way * way2, Way * way3, Way * way4, Way * way5, std::list<std::pair<const std::string, const std::string>> waysIncompatibilities);
  std::vector<std::string> selectChoices(std::vector<Attributes *> startingAttributes, std::vector<Way *> startingWays, std::list<std::pair<const std::string, const std::string>> waysIncompatibilities, Translator * t);
  void commandLoop(Link * link, WINDOW * mapScreen, WINDOW * statsScreen, WINDOW * displayScreen, WINDOW * targetScreen, Translator * t);
  Skill * selectSkill(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, int & object_type, Translator * t);
  bool selectTarget(WINDOW * mapScreen, WINDOW * targetScreen, MapDisplay * display, int range, int & target_id, int & target_x, int & target_y, int & orientation, Translator * t);
  std::string selectItem(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, int & object_type, Translator * t);
  std::string selectAmmo(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, Translator * t);
};

#endif // _DISPLAY_H_
