#ifndef _SPEECH_H_
#define _SPEECH_H_

#include <string>

#include "utils/String.h"

#include "Values.h"

class Speech {
  public:
    const std::string name;
    const std::string content;
    Speech(std::string content, int type):name("Unnexpected"),content(content) {}
    Speech(
      std::string name,
      std::string content
    ):
      name(name),
      content(content)
    {}
    std::string to_string();
    static Speech * from_string(std::string to_read);
    bool operator == (const Speech& s) const { return name == s.name; }
    bool operator != (const Speech& s) const { return !operator==(s); }
};

#endif // _SPEECH_H_
