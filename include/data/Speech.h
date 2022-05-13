#ifndef _SPEECH_H_
#define _SPEECH_H_

#include <string>

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
    bool operator == (const Speech& s) const { return name == s.name; }
    bool operator != (const Speech& s) const { return !operator==(s); }
};

#endif // _SPEECH_H_
