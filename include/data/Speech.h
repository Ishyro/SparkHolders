#ifndef _SPEECH_H_
#define _SPEECH_H_

#include <string>
#include "Values.h"

class Speech {
  public:
    const std::string name;
    const std::string content;
    Speech(std::string content, int type):name("Unnexpected") {
      switch(type) {
        case WARNING_LINE:
          this->content = "Warning: " + content;
          break;
        case ERROR_LINE:
          this->content = "Error: " + content;
          break;
        default:
          return;
      }
    }
    Speech(std::string name);
};

#endif // _SPEECH_H_
