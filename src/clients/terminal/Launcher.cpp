#include <string>
#include <ncurses.h>

#include "communication/Client.h"
#include "communication/Socket.h"

#include "clients/Link.h"

#include "clients/terminal/Display.h"

#include "data/Map.h"
#include "data/Tile.h"

void communicate(Link * link, WINDOW * screen) {
  while(true) {
    MapDisplay * display = link->receiveMap();
    if(display != nullptr) {
      Display::displayMap(display, screen);
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
    bool done = false;
    int type;
    int orientation = NO_ORIENTATION;
    ProjectileDisplay * projectile = nullptr;
    Skill * skill = nullptr;
    CharacterDisplay * target = nullptr;
    Item * item = nullptr;
    Weapon * weapon = nullptr;
    while(!done) {
      char keyPressed = getch();
      switch(keyPressed) {
        case '5':
          type = REST;
          orientation;
          done = true;
          break;
        case '1':
          type = MOVE;
          orientation = SOUTH_WEST;
          done = true;
          break;
        case '2':
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
          type = MOVE;
          orientation = WEST;
          done = true;
          break;
        case '6':
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
        default:
          ;
      }
    }
    link->sendAction(type, orientation, projectile, skill, target, item, weapon);
  }
}

int main(int argc, char ** argv) {
  initscr();
  cbreak();
  noecho();
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
  std::vector<std::string> choices = Display::selectChoices(link->getStartingAttributes(), link->getStartingWays());
  link->sendChoices(choices[0], choices[1], choices[2], choices[3], choices[4], choices[5], choices[6]);
  clear();
  int separator = (float) LINES / 1.5;
  WINDOW * mapScreen = subwin(stdscr, separator, COLS, 0, 0);
  WINDOW * otherScreen = subwin(stdscr, LINES - separator, COLS, separator, 0);
  box(mapScreen, ACS_VLINE, ACS_HLINE);
  box(otherScreen, ACS_VLINE, ACS_HLINE);
  wrefresh(mapScreen);
  communicate(link, mapScreen);
  endwin();
  s.close();
}
