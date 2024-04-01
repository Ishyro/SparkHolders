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
  void displayMap(StateDisplay * display, Adventure * adventure, Character * player, WINDOW * screen, Translator * t);
  void displayBlockMap(StateDisplay * display, Character * player, WINDOW * screen, Translator * t);
  void displayLightMap(StateDisplay * display, Character * player, WINDOW * screen, Translator * t);
  void displayStats(Character * player, WINDOW * screen, Translator * t);
  WINDOW * displaySkill(Skill * skill, WINDOW * screen, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, Translator * t);
  void displayTarget(CharacterDisplay * target, WINDOW * screen, Translator * t);
  WINDOW * displayItem(Item * item, WINDOW * screen, Translator * t);
  void displayCommands(WINDOW * screen, Translator * t);
  WINDOW * displayAttributes(Attributes * attributes, int32_t place, int32_t color, WINDOW * screen, int32_t offsetY, int32_t offsetX, Translator * t);
  WINDOW * displayWay(Way * way, int32_t place, int32_t color, WINDOW * screen, int32_t offsetY, int32_t offsetX, Translator * t);
  WINDOW * displayCharacter(Attributes * attributes, Way * race, Way * origin, Way * culture, Way * religion, Way * profession, int32_t color, WINDOW * screen, int32_t sizeX, int32_t offsetY, int32_t offsetX, Translator * t);
  bool compatibleWays(Way * way1, Way * way2, Way * way3, Way * way4, Way * way5, std::list<std::pair<const std::string, const std::string>> waysIncompatibilities);
  std::vector<std::string> selectChoices(std::vector<Attributes *> startingAttributes, std::vector<Way *> startingWays, std::list<std::pair<const std::string, const std::string>> waysIncompatibilities, Translator * t);
  void commandLoop(Link * link, WINDOW * mapScreen, WINDOW * statsScreen, WINDOW * displayScreen, WINDOW * targetScreen, Translator * t);
  Skill * selectSkill(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, int32_t & overcharge_power, int32_t & overcharge_duration, int32_t & overcharge_range, Translator * t);
  bool selectOvercharge(WINDOW * displayScreen, WINDOW * targetScreen, Skill * skill, Character * player, int32_t & overcharge_power, int32_t & overcharge_duration, int32_t & overcharge_range, Translator * t);
  bool selectTarget(WINDOW * mapScreen, WINDOW * targetScreen, StateDisplay * display, Character * player, int32_t range, int32_t & target_id, int32_t & target_x, int32_t & target_y, float & orientation, Translator * t);
  std::string selectItem(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, int32_t & object_type, int32_t & object_id, Translator * t);
  std::string selectAmmo(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, Translator * t);
  void sendAction(Link * link, int64_t id, int32_t type, void * arg1, void * arg2, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range);
};

#endif // _DISPLAY_H_
