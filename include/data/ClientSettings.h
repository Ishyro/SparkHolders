#ifndef _CLIENT_SETTINGS_H_
#define _CLIENT_SETTINGS_H_

#include <string>

namespace ClientSettings {
  int getPort();
  std::string getLang();
  std::string getResolution();
  std::string getLang();

  void setPort(int data);
  void setLang(std::string data);
  void setResolution(std::string data);
  void setWindowType(int data);
};

#endif // _CLIENT_SETTINGS_H_
