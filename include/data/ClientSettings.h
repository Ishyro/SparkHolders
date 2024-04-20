#ifndef _CLIENT_SETTINGS_H_
#define _CLIENT_SETTINGS_H_

#include <string>

namespace ClientSettings {
  int32_t getPort();
  std::string getLang();

  void setPort(int32_t data);
  void setLang(std::string data);
};

#endif // _CLIENT_SETTINGS_H_
