#ifndef _SPEECH_H_
#define _SPEECH_H_

#include <string>
#include "src/Values.h"

class Speech {
  public:
    const std::string name;
    const std::string content;
    Speech(std::string content, int type):name("Unnexpected") {
      switch(type) {
        case WARNING:
          this->content = "Warning: " + content;
          break;
        case ERROR:
          this->content = "Error: " + content;
          break;
       }
    }
    Speech(std::string name) {
      // TODO XML_READER
    }
};

#endif // _SPEECH_H_
