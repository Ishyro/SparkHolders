#include <string>

#include "communication/Client.h"
#include "communication/Socket.h"

#include "data/Attributes.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Tile.h"
#include "data/Way.h"

#include "clients/terminal/Display.h"

namespace Display {

  void displayMap(MapDisplay * display, WINDOW * screen) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    box(screen, ACS_VLINE, ACS_HLINE);
    mvwprintw(screen, 1, cols / 2 - display->name.length() / 2, display->name.c_str());
    for(int y = display->sizeY - 1; y >= 0; y--) {
      for(int x = 0; x < display->sizeX; x++) {
        std::string to_print = ".\0"; // middle dot - ·
        if(display->tiles[y][x]->untraversable) {
          to_print = "#\0";
        }
        if(display->tiles[y][x]->name == "mist") { // unseen
          to_print = "~\0";
        }
        mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY - 1 - y, x + cols / 2 - display->sizeX / 2, to_print.c_str());
      }
    }
    for(CharacterDisplay * character : display->characters) {
      std::string to_print;
      char truc = character->name.at(0);
      to_print = truc;
      int color;
      switch(character->teamRelation) {
        case SAME:
          color = BLUE;
          break;
        case ALLY:
          color = GREEN;
          break;
        case NEUTRAL:
          color = YELLOW;
          break;
        case ENEMY:
          color = RED;
          break;
        default:
          color = WHITE;
      }
      wattron(screen, COLOR_PAIR(color));
      mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY - 1 - character->y, character->x + cols / 2 - display->sizeX / 2, to_print.c_str());
      wattroff(screen, COLOR_PAIR(color));
    }
    for(ProjectileDisplay * projectile : display->projectiles) {
      std::string to_print = "\"\0";
      wattron(screen, COLOR_PAIR(RED));
      mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY - 1 - projectile->y, projectile->x + cols / 2 - display->sizeX / 2, to_print.c_str());
      wattroff(screen, COLOR_PAIR(RED));
    }
    for(Loot * loot : display->loots) {
      std::string to_print = "*\0";
      wattron(screen, COLOR_PAIR(YELLOW));
      mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY - 1 - loot->y, loot->x + cols / 2 - display->sizeX / 2, to_print.c_str());
      wattroff(screen, COLOR_PAIR(YELLOW));
    }
    wrefresh(screen);
  }

  void displayTileMap(MapDisplay * display, WINDOW * screen) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    box(screen, ACS_VLINE, ACS_HLINE);
    mvwprintw(screen, 1, cols / 2 - display->name.length() / 2, display->name.c_str());
    for(int y = display->sizeY - 1; y >= 0 ; y--) {
      for(int x = 0; x < display->sizeX; x++) {
        char * to_print = new char [2];
        to_print[1] = '\0';
        to_print[0] = display->tiles[y][x]->name.at(0);
        mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY -1 - y, x + cols / 2 - display->sizeX / 2, to_print);
        delete to_print;
      }
    }
    wrefresh(screen);
  }

  void displayLightMap(MapDisplay * display, WINDOW * screen) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    box(screen, ACS_VLINE, ACS_HLINE);
    mvwprintw(screen, 1, cols / 2 - display->name.length() / 2, display->name.c_str());
    for(int y = display->sizeY - 1; y >= 0 ; y--) {
      for(int x = 0; x < display->sizeX; x++) {
        char * to_print = new char [2];
        to_print[1] = '\0';
        to_print[0] = std::to_string(display->tiles[y][x]->light).at(0);
        mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY -1 - y, x + cols / 2 - display->sizeX / 2, to_print);
        delete to_print;
      }
    }
    wrefresh(screen);
  }

  WINDOW * displayAttributes(Attributes * attributes, int place, int color, WINDOW * screen, int offsetY, int offsetX) {
    WINDOW * attributesScreen = subwin(screen, ATTRIBUTES_HEIGHT, ATTRIBUTES_LENGTH, offsetY + 2 + ATTRIBUTES_HEIGHT * place, offsetX + 1);
    wattron(attributesScreen, COLOR_PAIR(color));
    box(attributesScreen, ACS_VLINE, ACS_HLINE);
    mvwprintw(attributesScreen, 1, ATTRIBUTES_LENGTH / 2 - attributes->name.length() / 2, attributes->name.c_str());
    mvwprintw(attributesScreen, 3, 1, (std::string("Hp: ") + std::to_string(attributes->baseHp)).c_str());
    mvwprintw(attributesScreen, 4, 1, (std::string("Mana: ") + std::to_string(attributes->baseMana)).c_str());
    mvwprintw(attributesScreen, 5, 1, (std::string("Armor: ") + std::to_string(attributes->baseArmor)).c_str());
    mvwprintw(attributesScreen, 6, 1, (std::string("Soulburn: ") + std::to_string(attributes->baseSoulBurn)).c_str());
    mvwprintw(attributesScreen, 7, 1, (std::string("Flow: ") + std::to_string(attributes->baseFlow)).c_str());
    mvwprintw(attributesScreen, 8, 1, (std::string("Vision Range: ") + std::to_string(attributes->baseVisionRange)).c_str());
    mvwprintw(attributesScreen, 9, 1, (std::string("Vision Power: ") + std::to_string(attributes->baseVisionPower)).c_str());
    mvwprintw(attributesScreen, 10, 1, (std::string("Detection Range: ") + std::to_string(attributes->baseDetectionRange)).c_str());
    wattroff(attributesScreen, COLOR_PAIR(color));
    return attributesScreen;
  }

  WINDOW * displayWay(Way * way, int place, int color, WINDOW * screen, int offsetY, int offsetX) {
    WINDOW * wayScreen = subwin(screen, WAY_HEIGHT, WAY_LENGTH, offsetY + 2 + WAY_HEIGHT * place, offsetX + 1);
    wattron(wayScreen, COLOR_PAIR(color));
    box(wayScreen, ACS_VLINE, ACS_HLINE);
    mvwprintw(wayScreen, 1, WAY_LENGTH / 2 - way->name.length() / 2, way->name.c_str());
    mvwprintw(wayScreen, 3, 1, (std::string("+Hp: ") + std::to_string(way->hpIncr)).c_str());
    mvwprintw(wayScreen, 4, 1, (std::string("+Mana: ") + std::to_string(way->manaIncr)).c_str());
    mvwprintw(wayScreen, 5, 1, (std::string("+Armor: ") + std::to_string(way->armorIncr)).c_str());
    mvwprintw(wayScreen, 6, 1, (std::string("+Soulburn: ") + std::to_string(way->soulBurnIncr)).c_str());
    mvwprintw(wayScreen, 7, 1, (std::string("+Flow: ") + std::to_string(way->flowIncr)).c_str());
    wattroff(wayScreen, COLOR_PAIR(color));
    return wayScreen;
  }

  WINDOW * displayCharacter(Attributes * attributes, Way * race, Way * origin, Way * culture, Way * religion, Way * profession, int color, WINDOW * screen, int sizeX, int offsetY, int offsetX) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    WINDOW * characterScreen = subwin(screen, ATTRIBUTES_HEIGHT - 2, sizeX, offsetY, offsetX);
    int hpIncr = 0;
    int manaIncr = 0;
    int armorIncr = 0;
    int soulBurnIncr = 0;
    int flowIncr = 0;
    if(race != nullptr) {
      hpIncr += race->hpIncr;
      manaIncr += race->manaIncr;
      armorIncr += race->armorIncr;
      soulBurnIncr += race->soulBurnIncr;
      flowIncr += race->flowIncr;
    }
    if(origin != nullptr) {
      hpIncr += origin->hpIncr;
      manaIncr += origin->manaIncr;
      armorIncr += origin->armorIncr;
      soulBurnIncr += origin->soulBurnIncr;
      flowIncr += origin->flowIncr;
    }
    if(culture != nullptr) {
      hpIncr += culture->hpIncr;
      manaIncr += culture->manaIncr;
      armorIncr += culture->armorIncr;
      soulBurnIncr += culture->soulBurnIncr;
      flowIncr += culture->flowIncr;
    }
    if(religion != nullptr) {
      hpIncr += religion->hpIncr;
      manaIncr += religion->manaIncr;
      armorIncr += religion->armorIncr;
      soulBurnIncr += religion->soulBurnIncr;
      flowIncr += religion->flowIncr;
    }
    if(profession != nullptr) {
      hpIncr += profession->hpIncr;
      manaIncr += profession->manaIncr;
      armorIncr += profession->armorIncr;
      soulBurnIncr += profession->soulBurnIncr;
      flowIncr += profession->flowIncr;
    }
    wattron(characterScreen, COLOR_PAIR(color));
    box(characterScreen, ACS_VLINE, ACS_HLINE);
    int space = cols / 2;
    if(attributes != nullptr) {
      mvwprintw(characterScreen, 1, 1, (std::string("Hp: ") + std::to_string(attributes->baseHp)).c_str());
      mvwprintw(characterScreen, 2, 1, (std::string("Mana: ") + std::to_string(attributes->baseMana)).c_str());
      mvwprintw(characterScreen, 3, 1, (std::string("Armor: ") + std::to_string(attributes->baseArmor)).c_str());
      mvwprintw(characterScreen, 4, 1, (std::string("Soulburn: ") + std::to_string(attributes->baseSoulBurn)).c_str());
      mvwprintw(characterScreen, 5, 1, (std::string("Flow: ") + std::to_string(attributes->baseFlow)).c_str());
      mvwprintw(characterScreen, 6, 1, (std::string("Vision Range: ") + std::to_string(attributes->baseVisionRange)).c_str());
      mvwprintw(characterScreen, 7, 1, (std::string("Vision Power: ") + std::to_string(attributes->baseVisionPower)).c_str());
      mvwprintw(characterScreen, 8, 1, (std::string("Detection Range: ") + std::to_string(attributes->baseDetectionRange)).c_str());
    }
    mvwprintw(characterScreen, 1, space, (std::string("+Hp: ") + std::to_string(hpIncr)).c_str());
    mvwprintw(characterScreen, 2, space, (std::string("+Mana: ") + std::to_string(manaIncr)).c_str());
    mvwprintw(characterScreen, 3, space, (std::string("+Armor: ") + std::to_string(armorIncr)).c_str());
    mvwprintw(characterScreen, 4, space, (std::string("+Soulburn: ") + std::to_string(soulBurnIncr)).c_str());
    mvwprintw(characterScreen, 5, space, (std::string("+Flow: ") + std::to_string(flowIncr)).c_str());
    wattroff(characterScreen, COLOR_PAIR(color));
    return characterScreen;
  }

  bool compatibleWays(Way * way1, Way * way2, Way * way3, Way * way4, Way * way5, std::list<std::pair<const std::string, const std::string>> waysIncompatibilities) {
    if(way2 != nullptr) {
      std::pair<const std::string, const std::string> pair1 = std::make_pair(way1->name, way2->name);
      for(std::pair<const std::string, const std::string> pair2 : waysIncompatibilities) {
        if((pair1.first == pair2.first && pair1.second == pair2.second) ||
          (pair1.first == pair2.second && pair1.second == pair2.first)) {
            return false;
        }
      }
    }
    if(way3 != nullptr) {
      std::pair<const std::string, const std::string> pair1 = std::make_pair(way1->name, way3->name);
      for(std::pair<const std::string, const std::string> pair2 : waysIncompatibilities) {
        if((pair1.first == pair2.first && pair1.second == pair2.second) ||
          (pair1.first == pair2.second && pair1.second == pair2.first)) {
            return false;
        }
      }
    }
    if(way4 != nullptr) {
      std::pair<const std::string, const std::string> pair1 = std::make_pair(way1->name, way4->name);
      for(std::pair<const std::string, const std::string> pair2 : waysIncompatibilities) {
        if((pair1.first == pair2.first && pair1.second == pair2.second) ||
          (pair1.first == pair2.second && pair1.second == pair2.first)) {
            return false;
        }
      }
    }
    if(way5 != nullptr) {
      std::pair<const std::string, const std::string> pair1 = std::make_pair(way1->name, way5->name);
      for(std::pair<const std::string, const std::string> pair2 : waysIncompatibilities) {
        if((pair1.first == pair2.first && pair1.second == pair2.second) ||
          (pair1.first == pair2.second && pair1.second == pair2.first)) {
            return false;
        }
      }
    }
    return true;
  }

  std::vector<std::string> selectChoices(std::vector<Attributes *> startingAttributes, std::vector<Way *> startingWays, std::list<std::pair<const std::string, const std::string>> waysIncompatibilities) {
    Attributes * selectedAttributes = nullptr;
    Way * selectedRace = nullptr;
    Way * selectedOrigin = nullptr;
    Way * selectedCulture = nullptr;
    Way * selectedReligion = nullptr;
    Way * selectedProfession = nullptr;
    std::vector<Way *> races = std::vector<Way *>();
    std::vector<Way *> origins = std::vector<Way *>();
    std::vector<Way *> cultures = std::vector<Way *>();
    std::vector<Way *> religions = std::vector<Way *>();
    std::vector<Way *> professions = std::vector<Way *>();
    std::string characterName = "";
    std::vector<std::string> result = std::vector<std::string>(7);
    for(Way * way : startingWays) {
      switch(way->type) {
        case RACE:
          races.push_back(way);
          break;
        case ORIGIN:
          origins.push_back(way);
          break;
        case CULTURE:
          cultures.push_back(way);
          break;
        case RELIGION:
          religions.push_back(way);
          break;
        case PROFESSION:
          professions.push_back(way);
          break;
        default:
          ;
      }
    }
    bool done = false;
    int cursorX = 0;
    int cursorY = 0;
    clear();
    WINDOW * attributesScreen = subwin(stdscr, LINES, ATTRIBUTES_LENGTH + 2, 0, 0);
    WINDOW * raceScreen = subwin(stdscr, LINES, WAY_LENGTH + 2, 0, ATTRIBUTES_LENGTH + 2);
    WINDOW * originScreen = subwin(stdscr, LINES, WAY_LENGTH + 2, 0, ATTRIBUTES_LENGTH + 2 + WAY_LENGTH + 2);
    WINDOW * cultureScreen = subwin(stdscr, LINES, WAY_LENGTH + 2, 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 2);
    WINDOW * religionScreen = subwin(stdscr, LINES, WAY_LENGTH + 2, 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 3);
    WINDOW * professionScreen = subwin(stdscr, LINES, WAY_LENGTH + 2, 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 4);
    WINDOW * characterScreen = subwin(stdscr, LINES, COLS - (ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5), 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5);
    box(attributesScreen, ACS_VLINE, ACS_HLINE);
    box(raceScreen, ACS_VLINE, ACS_HLINE);
    box(originScreen, ACS_VLINE, ACS_HLINE);
    box(cultureScreen, ACS_VLINE, ACS_HLINE);
    box(religionScreen, ACS_VLINE, ACS_HLINE);
    box(professionScreen, ACS_VLINE, ACS_HLINE);
    box(characterScreen, ACS_VLINE, ACS_HLINE);
    int lines = 0;
    int cols = 0;
    getmaxyx(attributesScreen, lines, cols);
    int numberAttributes = lines / ATTRIBUTES_HEIGHT;
    getmaxyx(raceScreen, lines, cols);
    int numberWays = lines / WAY_HEIGHT;
    std::list<WINDOW *> screens = std::list<WINDOW *>();
    int currentPannel = 0;
    int skip = 0;
    bool nameMode = false;
    while(!done) {
      int attributesCount = 0;
      int raceCount = 0;
      int originCount = 0;
      int cultureCount = 0;
      int religionCount = 0;
      int professionCount = 0;
      std::vector<Way *> availableRaces = std::vector<Way *>();
      std::vector<Way *> availableOrigins = std::vector<Way *>();
      std::vector<Way *> availableCultures = std::vector<Way *>();
      std::vector<Way *> availableReligions = std::vector<Way *>();
      std::vector<Way *> availableProfessions = std::vector<Way *>();
      std::string to_print = "CLASSE";
      mvwprintw(attributesScreen, 1, 1 + ATTRIBUTES_LENGTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = "RACE";
      mvwprintw(raceScreen, 1, 1 + WAY_LENGTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = "ORIGIN";
      mvwprintw(originScreen, 1, 1 + WAY_LENGTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = "CULTURE";
      mvwprintw(cultureScreen, 1, 1 + WAY_LENGTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = "RELIGION";
      mvwprintw(religionScreen, 1, 1 + WAY_LENGTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = "PROFESSION";
      mvwprintw(professionScreen, 1, 1 + WAY_LENGTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = "CHARACTER";
      mvwprintw(characterScreen, 1, (COLS - (ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5)) / 2 - to_print.length() / 2, to_print.c_str());
      skip = 0;
      for(Attributes * attributes : startingAttributes) {
        if(skip++ < currentPannel * numberAttributes) {
          continue;
        }
        int color = WHITE;
        if(selectedAttributes != nullptr && selectedAttributes->name == attributes->name) {
          color = GREEN;
        }
        if(cursorX == 0 && cursorY == attributesCount) {
          color = BLUE;
        }
        screens.push_back(displayAttributes(attributes, attributesCount++, color, attributesScreen, 0, 0));
      }
      wrefresh(attributesScreen);
      skip = 0;
      for(Way * way : races) {
        if(skip++ < currentPannel * numberWays) {
          continue;
        }
        int color = WHITE;
        if(selectedRace != nullptr && selectedRace->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 1 && cursorY == raceCount) {
          color = BLUE;
        }
        if(compatibleWays(way, selectedOrigin, selectedCulture, selectedReligion, selectedProfession, waysIncompatibilities)) {
          screens.push_back(displayWay(way, raceCount++, color, raceScreen, 0, ATTRIBUTES_LENGTH + 2));
          availableRaces.push_back(way);
        }
      }
      wrefresh(raceScreen);
      skip = 0;
      for(Way * way : origins) {
        if(skip++ < currentPannel * numberWays) {
          continue;
        }
        int color = WHITE;
        if(selectedOrigin != nullptr && selectedOrigin->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 2 && cursorY == originCount) {
          color = BLUE;
        }
        if(compatibleWays(way, selectedRace, selectedCulture, selectedReligion, selectedProfession, waysIncompatibilities)) {
          screens.push_back(displayWay(way, originCount++, color, originScreen, 0, ATTRIBUTES_LENGTH + 2 + WAY_LENGTH + 2));
          availableOrigins.push_back(way);
        }
      }
      wrefresh(originScreen);
      skip = 0;
      for(Way * way : cultures) {
        if(skip++ < currentPannel * numberWays) {
          continue;
        }
        int color = WHITE;
        if(selectedCulture != nullptr && selectedCulture->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 3 && cursorY == cultureCount) {
          color = BLUE;
        }
        if(compatibleWays(way, selectedRace, selectedOrigin, selectedReligion, selectedProfession, waysIncompatibilities)) {
          screens.push_back(displayWay(way, cultureCount++, color, cultureScreen, 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 2));
          availableCultures.push_back(way);
        }
      }
      wrefresh(cultureScreen);
      skip = 0;
      for(Way * way : religions) {
        if(skip++ < currentPannel * numberWays) {
          continue;
        }
        int color = WHITE;
        if(selectedReligion != nullptr && selectedReligion->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 4 && cursorY == religionCount) {
          color = BLUE;
        }
        if(compatibleWays(way, selectedRace, selectedOrigin, selectedCulture, selectedProfession, waysIncompatibilities)) {
          screens.push_back(displayWay(way, religionCount++, color, religionScreen, 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 3));
          availableReligions.push_back(way);
        }
      }
      wrefresh(religionScreen);
      skip = 0;
      for(Way * way : professions) {
        if(skip++ < currentPannel * numberWays) {
          continue;
        }
        int color = WHITE;
        if(selectedProfession != nullptr && selectedProfession->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 5 && cursorY == professionCount) {
          color = BLUE;
        }
        if(compatibleWays(way, selectedRace, selectedOrigin, selectedCulture, selectedReligion, waysIncompatibilities)) {
          screens.push_back(displayWay(way, professionCount++, color, professionScreen, 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 4));
          availableProfessions.push_back(way);
        }
      }
      wrefresh(professionScreen);
      int color = WHITE;
      if(characterName != "" && characterName.find(';') == std::string::npos && characterName.find('|') == std::string::npos && characterName.find('@') == std::string::npos
        && characterName.find('&') == std::string::npos && characterName.find('%') == std::string::npos) {
        color = GREEN;
      }
      if(cursorX == 6 && cursorY == 0) {
        color = BLUE;
      }
      WINDOW * nameScreen = subwin(characterScreen, 3, COLS - (ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5) - 2, 2, 1 + ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5);
      screens.push_back(nameScreen);
      wattron(nameScreen, COLOR_PAIR(color));
      box(nameScreen, ACS_VLINE, ACS_HLINE);
      mvwprintw(nameScreen, 1, 1, (std::string("Name: ") + characterName).c_str());
      wattroff(nameScreen, COLOR_PAIR(color));
      color = WHITE;
      if(cursorX == 6 && cursorY == 1) {
        color = BLUE;
      }
      screens.push_back(displayCharacter(selectedAttributes, selectedRace, selectedOrigin, selectedCulture, selectedReligion, selectedProfession, color, characterScreen, COLS - (ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5) - 2, 5, 1 + ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5));
      color = WHITE;
      if(characterName != "" && characterName.find(';') == std::string::npos && characterName.find('|') == std::string::npos && characterName.find('@') == std::string::npos
        && characterName.find('&') == std::string::npos && characterName.find('%') == std::string::npos && !nameMode && selectedAttributes != nullptr
        && selectedRace != nullptr && selectedOrigin != nullptr && selectedCulture != nullptr && selectedReligion != nullptr && selectedProfession != nullptr) {
        color = GREEN;
      }
      if(cursorX == 6 && cursorY == 2) {
        color = BLUE;
      }

      int width = COLS - (ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5) - 2;
      WINDOW * confirmScreen = subwin(characterScreen, 3, width, 5 + ATTRIBUTES_HEIGHT - 1, 1 + ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5);
      screens.push_back(confirmScreen);
      wattron(confirmScreen, COLOR_PAIR(color));
      box(confirmScreen, ACS_VLINE, ACS_HLINE);
      to_print = "CONFIRM CHARACTER CREATION";
      mvwprintw(confirmScreen, 1, width / 2 - to_print.length() / 2, to_print.c_str());
      wattroff(confirmScreen, COLOR_PAIR(color));
      wrefresh(characterScreen);
      bool done2 = false;
      while(!done2) {
        int keyPressed = getch();
        if(!nameMode) {
          switch(keyPressed) {
            case '4':
            case char(68): // LEFT ARROW
              cursorX = (cursorX - 1 + 7) % 7;
              if(cursorX == 0) {
                cursorY = std::min(cursorY, (int) startingAttributes.size() - 1 - currentPannel * numberAttributes);
              } else if(cursorX == 1) {
                cursorY = std::min(cursorY, (int) availableRaces.size() - 1 - currentPannel * numberWays);
              } else if(cursorX == 2) {
                cursorY = std::min(cursorY, (int) availableOrigins.size() - 1 - currentPannel *  numberWays);
              } else if(cursorX == 3) {
                cursorY = std::min(cursorY, (int) availableCultures.size() - 1 - currentPannel * numberWays);
              } else if(cursorX == 4) {
                cursorY = std::min(cursorY, (int) availableReligions.size() - 1 - currentPannel * numberWays);
              } else if(cursorX == 5) {
                cursorY = std::min(cursorY, (int) availableProfessions.size() - 1 - currentPannel * numberWays);
              } else if(cursorX == 6) {
                cursorY = std::min(cursorY, 2);
              }
              if(cursorY < 0) {
                cursorY = 0;
              }
              done2 = true;
              break;
            case '8':
            case char(65): { // UP ARROW
              cursorY--;
              if(cursorX == 0) {
                if(cursorY == -1 && currentPannel > 0) {
                  cursorY = numberAttributes -1;
                  currentPannel--;
                } else if(cursorY == -1) {
                  cursorY++;
                }
              } else if(cursorX == 1) {
                if(cursorY == -1 && currentPannel > 0) {
                  cursorY = numberWays -1;
                  currentPannel--;
                } else if(cursorY == -1) {
                  cursorY++;
                }
              } else if(cursorX == 2) {
                if(cursorY == -1 && currentPannel > 0) {
                  cursorY = numberWays -1;
                  currentPannel--;
                } else if(cursorY == -1) {
                  cursorY++;
                }
              } else if(cursorX == 3) {
                if(cursorY == -1 && currentPannel > 0) {
                  cursorY = numberWays -1;
                  currentPannel--;
                } else if(cursorY == -1) {
                  cursorY++;
                }
              } else if(cursorX == 4) {
                if(cursorY == -1 && currentPannel > 0) {
                  cursorY = numberWays -1;
                  currentPannel--;
                } else if(cursorY == -1) {
                  cursorY++;
                }
              } else if(cursorX == 5) {
                if(cursorY == -1 && currentPannel > 0) {
                  cursorY = numberWays -1;
                  currentPannel--;
                } else if(cursorY == -1) {
                  cursorY++;
                }
              } else if(cursorX == 6) {
                cursorY = (cursorY + 3) % 3;
              }
              done2 = true;
              break;
            }
            case '6':
            case char(67): // RIGHT ARROW
              cursorX = (cursorX + 1) % 7;
              if(cursorX == 0) {
                cursorY = std::min(cursorY, (int) startingAttributes.size() - 1 - currentPannel * numberAttributes);
              } else if(cursorX == 1) {
                cursorY = std::min(cursorY, (int) availableRaces.size() - 1 - currentPannel * numberWays);
              } else if(cursorX == 2) {
                cursorY = std::min(cursorY, (int) availableOrigins.size() - 1 - currentPannel * numberWays);
              } else if(cursorX == 3) {
                cursorY = std::min(cursorY, (int) availableCultures.size() - 1 - currentPannel * numberWays);
              } else if(cursorX == 4) {
                cursorY = std::min(cursorY, (int) availableReligions.size() - 1 - currentPannel * numberWays);
              } else if(cursorX == 5) {
                cursorY = std::min(cursorY, (int) availableProfessions.size() - 1 - currentPannel * numberWays);
              } else if(cursorX == 6) {
                cursorY = std::min(cursorY, 2);
              }
              if(cursorY < 0) {
                cursorY = 0;
              }
              done2 = true;
              break;
            case '2':
            case char(66): { // DOWN ARROW
              cursorY++;
              if(cursorX == 0) {
                if(cursorY == numberAttributes && startingAttributes.size() > numberAttributes) {
                  cursorY = 0;
                  currentPannel++;
                } else if(cursorY == startingAttributes.size()) {
                  cursorY--;
                }
              } else if(cursorX == 1) {
                if(cursorY == numberWays && availableRaces.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                } else if(cursorY == availableRaces.size()) {
                  cursorY--;
                }
              } else if(cursorX == 2) {
                if(cursorY == numberWays && availableOrigins.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                } else if(cursorY == availableOrigins.size()) {
                  cursorY--;
                }
              } else if(cursorX == 3) {
                if(cursorY == numberWays && availableCultures.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                } else if(cursorY == availableCultures.size()) {
                  cursorY--;
                }
              } else if(cursorX == 4) {
                if(cursorY == numberWays && availableReligions.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                } else if(cursorY == availableReligions.size()) {
                  cursorY--;
                }
              } else if(cursorX == 5) {
                if(cursorY == numberWays && availableProfessions.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                } else if(cursorY == availableProfessions.size()) {
                  cursorY--;
                }
              } else if(cursorX == 6) {
                cursorY = cursorY % 3;
              }
              done2 = true;
              break;
            }
            case char(10): { // ENTER
              if(cursorX == 0) {
                if(selectedAttributes == startingAttributes[cursorY + currentPannel * numberAttributes]) {
                  selectedAttributes = nullptr;
                } else {
                  selectedAttributes = startingAttributes[cursorY + currentPannel * numberAttributes];
                }
              } else if(cursorX == 1 && availableRaces.size() > 0) {
                if(selectedRace == availableRaces[cursorY]) {
                  selectedRace = nullptr;
                } else {
                  selectedRace = availableRaces[cursorY];
                }
              } else if(cursorX == 2 && availableOrigins.size() > 0) {
                if(selectedOrigin == availableOrigins[cursorY]) {
                  selectedOrigin = nullptr;
                } else {
                  selectedOrigin = availableOrigins[cursorY];
                }
              } else if(cursorX == 3 && availableCultures.size() > 0) {
                if(selectedCulture == availableCultures[cursorY]) {
                  selectedCulture = nullptr;
                } else {
                  selectedCulture = availableCultures[cursorY];
                }
              } else if(cursorX == 4 && availableReligions.size() > 0) {
                if(selectedReligion == availableReligions[cursorY]) {
                  selectedReligion = nullptr;
                } else {
                  selectedReligion = availableReligions[cursorY];
                }
              } else if(cursorX == 5 && availableProfessions.size() > 0) {
                if(selectedProfession == availableProfessions[cursorY]) {
                  selectedProfession = nullptr;
                } else {
                  selectedProfession = availableProfessions[cursorY];
                }
              } else if(cursorX == 6) {
                if(cursorY == 0) {
                  nameMode = true;
                } else if(cursorY == 2) {
                  if(characterName != "" && characterName.find(';') == std::string::npos && characterName.find('|') == std::string::npos && characterName.find('@') == std::string::npos
                    && characterName.find('&') == std::string::npos && characterName.find('%') == std::string::npos && !nameMode && selectedAttributes != nullptr
                    && selectedRace != nullptr && selectedOrigin != nullptr && selectedCulture != nullptr && selectedReligion != nullptr && selectedProfession != nullptr) {
                    done = true;
                  }
                }
              }
              done2 = true;
              break;
            }
            default:
              ;
          }
        } else { // nameMode is true
          switch(keyPressed) {
            case char(10): // ENTER
              nameMode = false;
              done2 = true;
              break;
            case char(127): // BACK_SPACE
              if(characterName != "") {
                characterName = characterName.substr(0, characterName.length() - 1);
              }
              done2 = true;
              break;
            default:
              characterName += keyPressed;
              done2 = true;
          }
        }
      }
      for(WINDOW * screen : screens) {
        wclear(screen);
        delwin(screen);
      }
      screens.clear();
    }
    clear();
    result[0] = characterName;
    result[1] = selectedAttributes->name;
    result[2] = selectedRace->name;
    result[3] = selectedOrigin->name;
    result[4] = selectedCulture->name;
    result[5] = selectedReligion->name;
    result[6] = selectedProfession->name;
    return result;
  }
}
