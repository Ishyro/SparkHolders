#include <string>
#include <ncurses.h>

#include "communication/Client.h"
#include "communication/Socket.h"

#include "clients/Link.h"
#include "clients/Translator.h"

#include "clients/terminal/Display.h"

#include "data/Map.h"
#include "data/Tile.h"

void communicate(Link * link, WINDOW * mapScreen, WINDOW * statsScreen, WINDOW * displayScreen, WINDOW * commandsScreen, Translator * t) {
  while(true) {
    MapDisplay * display = link->receiveMap();
    Display::displayMap(display, link->getPlayer(), mapScreen, t);
    Display::displayStats(link->getPlayer(), statsScreen, t);
    Display::displayCommands(commandsScreen, t);
    bool done = false;
    int type;
    int orientation = NO_ORIENTATION;
    ProjectileDisplay * projectile = nullptr;
    Skill * skill = nullptr;
    CharacterDisplay * target = nullptr;
    Item * item = nullptr;
    Weapon * weapon = nullptr;
    while(!done) {
      flushinp();
      int keyPressed = getch();
      switch(keyPressed) {
        case '5':
          type = REST;
          done = true;
          break;
        case '1':
          type = MOVE;
          orientation = SOUTH_WEST;
          done = true;
          break;
        case '2':
        case KEY_DOWN:
          type = MOVE;
          orientation = SOUTH;
          done = true;
          break;
        case '3':
          type = MOVE;
          orientation = SOUTH_EAST;
          done = true;
          break;
        case '4':
        case KEY_LEFT:
          type = MOVE;
          orientation = WEST;
          done = true;
          break;
        case '6':
        case KEY_RIGHT:
          type = MOVE;
          orientation = EAST;
          done = true;
          break;
        case '7':
          type = MOVE;
          orientation = NORTH_WEST;
          done = true;
          break;
        case '8':
        case KEY_UP:
          type = MOVE;
          orientation = NORTH;
          done = true;
          break;
        case '9':
          type = MOVE;
          orientation = NORTH_EAST;
          done = true;
          break;
        case '<':
        case '>':
          type = CHANGE_MAP;
          done = true;
          break;
        case ' ':
          type = GRAB;
          done = true;
          break;
        case 'x':
        case 'X':
          type = REST;
          done = true;
          break;
        case 'c':
        case 'C':
          type = REST;
          done = true;
          break;
        default:
          ;
      }
    }
    link->sendAction(type, orientation, projectile, skill, target, item, weapon);
    for(CharacterDisplay * character : display->characters) {
      delete character;
    }
    for(ProjectileDisplay * projectile : display->projectiles) {
      delete projectile;
    }
    for(std::vector<Tile *> tiles : display->tiles) {
      for(Tile * tile : tiles) {
        delete tile;
      }
    }
    delete display;
  }
}

int main(int argc, char ** argv) {
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  nl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  curs_set(0);
  start_color();
  use_default_colors();
  short int * default_foreground = (short int *) new int(0);
  short int * default_background = (short int *) new int(0);
  pair_content(0, default_foreground, default_background);
  init_pair(BLUE, COLOR_BLUE, *default_background);
  init_pair(GREEN, COLOR_GREEN, *default_background);
  init_pair(YELLOW, COLOR_YELLOW, *default_background);
  init_pair(RED, COLOR_RED, *default_background);
  delete default_background;
  delete default_foreground;
  Socket s = Socket();
  s.connect("127.0.0.1", 45678);
  Link * link = new Link(s, nullptr);
  Translator * t;
  if (argc == 2) {
    try {
      link->loadChoices();
      t = new Translator(link->receiveTraductionPaths(), std::string(argv[1]));
    } catch (CloseException &e) {
      endwin();
      s.close();
      delete link;
      return EXIT_FAILURE;
    }
    std::vector<std::string> choices;
    choices = Display::selectChoices(link->getStartingAttributes(), link->getStartingWays(), link->getWaysIncompatibilities(), t);
    try {
      link->sendChoices(choices[0], choices[1], choices[2], choices[3], choices[4], choices[5], choices[6]);
    } catch (CloseException &e) {
      endwin();
      s.close();
      delete link;
      return EXIT_FAILURE;
    }
  } else if (argc == 3) {
    // reconnect mode
    try {
      if(s.read() != "RECONNECT") {
        endwin();
        s.close();
        delete link;
        return EXIT_FAILURE;
      }
      s.write(std::string(argv[2]));
      if(s.read() != "OK") {
        endwin();
        s.close();
        delete link;
        return EXIT_FAILURE;
      }
      t = new Translator(link->receiveTraductionPaths(), std::string(argv[1]));
    } catch (CloseException &e) {
      endwin();
      s.close();
      delete link;
      return EXIT_FAILURE;
    }
  }
  int separator = (float) LINES / 1.5;
  float ratio = 2.25;
  WINDOW * mapScreen = subwin(stdscr, separator, COLS, 0, 0);
  WINDOW * statsScreen = subwin(stdscr, LINES - separator, ratio * (LINES - separator), separator, 0);
  WINDOW * displayScreen = subwin(stdscr, LINES - separator, std::ceil((float) COLS - 2. * ratio * (float) (LINES - separator)), separator, ratio * (LINES - separator));
  WINDOW * commandsScreen = subwin(stdscr, LINES - separator, ratio * (LINES - separator), separator, std::ceil((float) COLS - ratio * (float) (LINES - separator)));
  std::string to_print = t->getStandardName("WAITING FOR OTHER PLAYERS...");
  mvwprintw(stdscr, LINES / 2, COLS / 2 - to_print.length() / 2, to_print.c_str());
  wrefresh(stdscr);
  clear();
  box(mapScreen, ACS_VLINE, ACS_HLINE);
  box(statsScreen, ACS_VLINE, ACS_HLINE);
  box(displayScreen, ACS_VLINE, ACS_HLINE);
  box(commandsScreen, ACS_VLINE, ACS_HLINE);
  try {
    communicate(link, mapScreen, statsScreen, displayScreen, commandsScreen, t);
  } catch (CloseException &e) {

  }
  wclear(mapScreen);
  wclear(statsScreen);
  wclear(displayScreen);
  wclear(commandsScreen);
  delwin(mapScreen);
  delwin(statsScreen);
  delwin(displayScreen);
  delwin(commandsScreen);
  endwin();
  s.close();
  delete link;
  return EXIT_SUCCESS;
}
