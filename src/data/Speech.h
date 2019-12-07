#ifndef _SPEECH_H_
#define _SPEECH_H_

#include <string>

class Speech {
  public:
    const std::string name;
    const std::string content;
    Speech(std::string name);
};

#endif // _SPEECH_H_
