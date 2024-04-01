#ifndef _CLIENT_SETTINGS_H_
#define _CLIENT_SETTINGS_H_

#include <string>

namespace ClientSettings {
  int32_t getPort();
  std::string getLang();
  std::string getResolution();
  std::string getLang();

  void setPort(int32_t data);
  void setLang(std::string data);
  void setResolution(std::string data);
  void setWindowType(int32_t data);
};

#endif // _CLIENT_SETTINGS_H_
