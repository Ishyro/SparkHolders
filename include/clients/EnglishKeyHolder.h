#ifndef _ENGLISH_KEY_HOLDER_H_
#define _ENGLISH_KEY_HOLDER_H_

#include <string>
#include <list>
#include <map>

class EnglishKeyHolder {
  public:
    EnglishKeyHolder(std::list<std::string> bases);
    const std::string getEnglishFromKey(std::string key);
  private:
    void loadValuesFromFile(const std::string fileName, std::map<const std::string, const std::string> * map);
    std::map<const std::string, const std::string> strings;
    #ifdef _WIN32_WINNT
      const std::string PATH_DELIMITER = "\\";
    #else
      const std::string PATH_DELIMITER = "/";
    #endif
};

#endif // _ENGLISH_KEY_HOLDER_H_
