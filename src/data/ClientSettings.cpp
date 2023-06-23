#include "data/ClientSettings.h"

// time
#include<ctime>

namespace ClientSettings {
  int port;
  std::string lang;
  std::string resolution;
  int window_type;

  int getPort() { return port; }
  std::string getLang() { return lang; }
  std::string getResolution() { return resolution; }
  int getWindowType() { return window_type; }

  void setPort(int data) { port = data; }
  void setLang(std::string data) { lang = data; }
  void setResolution(std::string data) { resolution = data; }
  void setWindowType(int data) { window_type = data; }
};
