#include "data/ClientSettings.h"

// time
#include<ctime>

namespace ClientSettings {
  int32_t port;
  std::string lang;
  std::string resolution;
  int32_t window_type;

  int32_t getPort() { return port; }
  std::string getLang() { return lang; }
  std::string getResolution() { return resolution; }
  int32_t getWindowType() { return window_type; }

  void setPort(int32_t data) { port = data; }
  void setLang(std::string data) { lang = data; }
  void setResolution(std::string data) { resolution = data; }
  void setWindowType(int32_t data) { window_type = data; }
};
