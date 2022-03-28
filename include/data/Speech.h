#ifndef _SPEECH_H_
#define _SPEECH_H_

#include <string>

#include "Values.h"

#include "utils/FileOpener.h"

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
};

#endif // _SPEECH_H_
