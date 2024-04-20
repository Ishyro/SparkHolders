#include "data/ClientSettings.h"

// time
#include<ctime>

namespace ClientSettings {
  int32_t port;
  std::string lang;

  int32_t getPort() { return port; }
  std::string getLang() { return lang; }

  void setPort(int32_t data) { port = data; }
  void setLang(std::string data) { lang = data; }
};
