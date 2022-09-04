#ifndef _SPEECH_H_
#define _SPEECH_H_

#include <string>
#include <map>

#include "Values.h"

class Speech {
  public:
    const std::string name;
    const int type;
    const bool empty;
    const bool constant;
    Speech(
      std::string name,
      int type,
      bool empty,
      bool constant,
      std::map<const std::string, const std::string> options
    ):
      name(name),
      type(type),
      empty(empty),
      constant(constant),
      options(std::map<const std::string, const std::string>())
    {
      for(auto pair : options) {
        this->options.insert(pair);
      }
    }
    std::list<std::string> getOptions();
    Speech * getNext(std::string response, Database * database);
    std::string to_string();
    static Speech * from_string(std::string to_read);
    bool operator == (const Speech& s) const { return name == s.name; }
    bool operator != (const Speech& s) const { return !operator==(s); }
  private:
    // value is a Speech name
    std::map<const std::string, const std::string> options;
};

#endif // _SPEECH_H_
