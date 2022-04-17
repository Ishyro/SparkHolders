#include <string>
#include <ncurses.h>

#include "communication/Client.h"
#include "communication/Socket.h"

#include "clients/Link.h"

#include "data/Map.h"
#include "data/Tile.h"

#define TEAM_COLOR 1
#define ALLY_COLOR 2
#define NEUTRAL_COLOR 3
#define ENNEMY_COLOR 4

void displayMap(MapDisplay * display, WINDOW * screen) {
  int cols = 0;
  int lines = 0;
  getmaxyx(screen, lines, cols);
  mvwprintw(screen, 1, cols / 2 - display->name.length() / 2, display->name.c_str());
  for(int y = display->sizeY - 1; y >= 0 ; y--) {
    for(int x = 0; x < display->sizeX; x++) {
      std::string to_print = ".\0"; // middle dot - ·
      if(display->tiles[x][y]->untraversable) {
        to_print = "#\0";
      }
      if(display->tiles[x][y]->name == "mist") { // unseen
        to_print = " \0";
      }
      mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY -1 - y, x + cols / 2 - display->sizeX / 2, to_print.c_str());
    }
  }
  for(CharacterDisplay * character : display->characters) {
    std::string to_print;
    char truc = character->name.at(0);
    to_print = truc;
    wattron(screen, COLOR_PAIR(TEAM_COLOR));
    mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY -1 - character->y, character->x + cols / 2 - display->sizeX / 2, to_print.c_str());
    wattroff(screen, COLOR_PAIR(TEAM_COLOR));
  }
  for(ProjectileDisplay * projectile : display->projectiles) {
    std::string to_print = "~\0";
    wattron(screen, COLOR_PAIR(ENNEMY_COLOR));
    mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY -1 - projectile->y, projectile->x + cols / 2 - display->sizeX / 2, to_print.c_str());
    wattroff(screen, COLOR_PAIR(ENNEMY_COLOR));
  }
  wrefresh(screen);
}

void displayTileMap(MapDisplay * display, WINDOW * screen) {
  int cols = 0;
  int lines = 0;
  getmaxyx(screen, lines, cols);
  mvwprintw(screen, 1, cols / 2 - display->name.length() / 2, display->name.c_str());
  for(int y = display->sizeY - 1; y >= 0 ; y--) {
    for(int x = 0; x < display->sizeX; x++) {
      char * to_print = new char [2];
      to_print[1] = '\0';
      to_print[0] = display->tiles[x][y]->name.at(0);
      mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY -1 - y, x + cols / 2 - display->sizeX / 2, to_print);
      delete to_print;
    }
  }
  wrefresh(screen);
}

void displayLightMap(MapDisplay * display, WINDOW * screen) {
  int cols = 0;
  int lines = 0;
  getmaxyx(screen, lines, cols);
  mvwprintw(screen, 1, cols / 2 - display->name.length() / 2, display->name.c_str());
  for(int y = display->sizeY - 1; y >= 0 ; y--) {
    for(int x = 0; x < display->sizeX; x++) {
      char * to_print = new char [2];
      to_print[1] = '\0';
      to_print[0] = std::to_string(display->tiles[x][y]->light).at(0);
      mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY -1 - y, x + cols / 2 - display->sizeX / 2, to_print);
      delete to_print;
    }
  }
  wrefresh(screen);
}

void communicate(Link * link, WINDOW * screen) {
  while(true) {
    MapDisplay * display = link->receiveMap();
    if(display != nullptr) {
      displayMap(display, screen);
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
    link->sendAction(nullptr);
  }
}

int main(int argc, char ** argv) {
  initscr();
  curs_set(0);
  start_color();
  use_default_colors();
  short int * default_foreground = (short int *) new int(0);
  short int * default_background = (short int *) new int(0);
  pair_content(0, default_foreground, default_background);
  init_pair(TEAM_COLOR, COLOR_BLUE, *default_background);
  init_pair(ALLY_COLOR, COLOR_GREEN, *default_background);
  init_pair(NEUTRAL_COLOR, COLOR_YELLOW, *default_background);
  init_pair(ENNEMY_COLOR, COLOR_RED, *default_background);
  WINDOW * mapScreen = subwin(stdscr, LINES / 2, COLS, 0, 0);
  WINDOW * otherScreen = subwin(stdscr, LINES / 2, COLS, LINES / 2, 0);
  box(mapScreen, ACS_VLINE, ACS_HLINE);
  box(otherScreen, ACS_VLINE, ACS_HLINE);
  Socket s = Socket();
  s.connect("127.0.0.1", 45678);
  Link * link = new Link(s, nullptr);
  link->sendChoices("Betatesteur", "Vanguard", "human", "countryside", "tagran", "atheist", "soldier");
  communicate(link, mapScreen);
  endwin();
  s.close();
}
