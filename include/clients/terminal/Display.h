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

#define ATTRIBUTES_HEIGHT 13
#define ATTRIBUTES_WIDTH 25

#define WAY_HEIGHT 10
#define WAY_WIDTH 25

namespace Display {

  void print(WINDOW* screen, int32_t offsetY, int32_t offsetX, std::string to_print);
  void displayMap(StateDisplay * display, Adventure * adventure, Character * player, WINDOW * screen, EnglishKeyHolder * t);
  void displayBlockMap(StateDisplay * display, Character * player, WINDOW * screen, EnglishKeyHolder * t);
  void displayLightMap(StateDisplay * display, Character * player, WINDOW * screen, EnglishKeyHolder * t);
  void displayStats(Character * player, WINDOW * screen, EnglishKeyHolder * t);
  WINDOW * displaySkill(Skill * skill, WINDOW * screen, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, EnglishKeyHolder * t);
  void displayTarget(CharacterDisplay * target, WINDOW * screen, EnglishKeyHolder * t);
  WINDOW * displayItem(Item * item, WINDOW * screen, EnglishKeyHolder * t);
  void displayCommands(WINDOW * screen, EnglishKeyHolder * t);
  WINDOW * displayAttributes(Attributes * attributes, int32_t place, int32_t color, WINDOW * screen, int32_t offsetY, int32_t offsetX, EnglishKeyHolder * t);
  WINDOW * displayWay(Way * way, int32_t place, int32_t color, WINDOW * screen, int32_t offsetY, int32_t offsetX, EnglishKeyHolder * t);
  WINDOW * displayCharacter(Attributes * attributes, Way * race, Way * origin, Way * culture, Way * religion, Way * profession, int32_t color, WINDOW * screen, int32_t sizeX, int32_t offsetY, int32_t offsetX, EnglishKeyHolder * t);
  bool compatibleWays(Way * way1, Way * way2, Way * way3, Way * way4, Way * way5, std::list<std::pair<const std::string, const std::string>> waysIncompatibilities);
  std::vector<std::string> selectChoices(std::vector<Attributes *> startingAttributes, std::vector<Way *> startingWays, std::list<std::pair<const std::string, const std::string>> waysIncompatibilities, EnglishKeyHolder * t);
  void commandLoop(Link * link, WINDOW * mapScreen, WINDOW * statsScreen, WINDOW * displayScreen, WINDOW * targetScreen, EnglishKeyHolder * t);
  Skill * selectSkill(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, int32_t & overcharge_power, int32_t & overcharge_duration, int32_t & overcharge_range, EnglishKeyHolder * t);
  bool selectOvercharge(WINDOW * displayScreen, WINDOW * targetScreen, Skill * skill, Character * player, int32_t & overcharge_power, int32_t & overcharge_duration, int32_t & overcharge_range, EnglishKeyHolder * t);
  bool selectTarget(WINDOW * mapScreen, WINDOW * targetScreen, StateDisplay * display, Character * player, int32_t range, int32_t & target_id, int32_t & target_x, int32_t & target_y, float & orientation, EnglishKeyHolder * t);
  std::string selectItem(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, int32_t & object_type, int32_t & object_id, EnglishKeyHolder * t);
  std::string selectAmmo(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, EnglishKeyHolder * t);
  void sendAction(Link * link, int32_t type, void * arg1, void * arg2, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range);
};

#endif // _DISPLAY_H_
