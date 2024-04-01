#ifdef _WIN32_WINNT
  #include <winsock2.h>
  #include <windows.h>
  #include <thread>
#endif

#include <thread>

#include <string>
#include <ncurses.h>

#include "communication/Client.h"
#include "communication/Socket.h"

#include "clients/Link.h"
#include "clients/Translator.h"

#include "clients/terminal/Display.h"

#include "data/Map.h"
#include "data/Block.h"

void listener(void * param) {
  Link * link = (Link *) param;
  while(!link->isClosed()) {
    link->listen();
  }
}

int32_t main(int32_t argc, char ** argv) {
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
  int16_t * default_foreground = (int16_t *) new int16_t(0);
  int16_t * default_background = (int16_t *) new int16_t(0);
  pair_content(0, default_foreground, default_background);
  init_pair(BLUE, COLOR_BLUE, *default_background);
  init_pair(GREEN, COLOR_GREEN, *default_background);
  init_pair(YELLOW, COLOR_YELLOW, *default_background);
  init_pair(RED, COLOR_RED, *default_background);
  init_pair(BACK_RED, *default_foreground, COLOR_RED);
  delete default_background;
  delete default_foreground;
  #ifdef _WIN32_WINNT
    HANDLE thread;
  #else
    std::thread thread;
  #endif
  Socket s = Socket();
  // std::string IP = "84.97.162.152";
  // std::string IP = "192.168.168.164";
  // std::string IP = "192.168.1.83";
  std::string IP = "127.0.0.1";
  s.connect(IP, 45678);
  Link * link;
  Translator * t;
  if (argc == 2) {
    try {
      link = new Link(s, argv[1]);
      link->initialize("tester", "admin");
    } catch (CloseException &e) {
      endwin();
      s.close();
      delete link;
      return EXIT_FAILURE;
    }
    #ifdef _WIN32_WINNT
      thread = (HANDLE) _beginthreadex(NULL, 0, (_beginthreadex_proc_type) listener, (void *) link, 0, NULL);
    #else
      thread = std::thread(listener, (void *) link);
    #endif
    while(!link->isStarted()) {
      usleep(1);
    }
    t = link->getTranslator();
    std::vector<std::string> choices;
    choices = Display::selectChoices(link->getStartingAttributes(), link->getStartingWays(), link->getWaysIncompatibilities(), t);
    try {
      link->sendChoices(choices[0], choices[1], choices[2], choices[3], choices[4], choices[5], choices[6]);
      link->sendReady();
    } catch (CloseException &e) {
      endwin();
      s.close();
      delete link;
      return EXIT_FAILURE;
    }
  }
  else if (argc == 3) {
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
      link = new Link(s, argv[1]);
      #ifdef _WIN32_WINNT
        HANDLE thread = (HANDLE) _beginthreadex(NULL, 0, (_beginthreadex_proc_type) listener, (void *) link, 0, NULL);
      #else
        std::thread thread = std::thread(listener, (void *) link);
      #endif
      while(!link->isStarted()) {
        usleep(1);
      }
      t = link->getTranslator();
    } catch (CloseException &e) {
      endwin();
      s.close();
      delete link;
      return EXIT_FAILURE;
    }
  }
  int32_t separator = (float) LINES * 3 / 5;
  float ratio = 2.25;
  WINDOW * mapScreen = subwin(stdscr, separator, COLS, 0, 0);
  WINDOW * statsScreen = subwin(stdscr, LINES - separator, ratio * (LINES - separator), separator, 0);
  WINDOW * displayScreen = subwin(stdscr, LINES - separator, std::ceil((float) COLS - 2. * ratio * (float) (LINES - separator)), separator, ratio * (LINES - separator));
  WINDOW * targetScreen = subwin(stdscr, LINES - separator, ratio * (LINES - separator), separator, std::ceil((float) COLS - ratio * (float) (LINES - separator)));
  std::string to_print = t->getStandardName("WAITING FOR OTHER PLAYERS...");
  mvwprintw(stdscr, LINES / 2, COLS / 2 - to_print.length() / 2, to_print.c_str());
  wrefresh(stdscr);
  clear();
  box(mapScreen, ACS_VLINE, ACS_HLINE);
  box(statsScreen, ACS_VLINE, ACS_HLINE);
  box(displayScreen, ACS_VLINE, ACS_HLINE);
  box(targetScreen, ACS_VLINE, ACS_HLINE);
  try {
    Display::commandLoop(link, mapScreen, statsScreen, displayScreen, targetScreen, t);
  } catch (CloseException &e) {

  }
  wclear(mapScreen);
  wclear(statsScreen);
  wclear(displayScreen);
  wclear(targetScreen);
  delwin(mapScreen);
  delwin(statsScreen);
  delwin(displayScreen);
  delwin(targetScreen);
  endwin();
  s.close();
  delete link;
  return EXIT_SUCCESS;
}
