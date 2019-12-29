#ifndef _SPEECH_H_
#define _SPEECH_H_

#include <string>

class Speech {
  public:
    const std::string name;
    const std::string content;
    Speech(std::string content, int type) {
      switch(type) {
        case UNEXPECTED:
        case ERROR:
      }
    }
    Speech(std::string name) {
      // TODO XML_READER
    }
};

#endif // _SPEECH_H_
