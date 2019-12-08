#ifndef _SPEECH_MANAGER_H_
#define _SPEECH_MANAGER_H_

#include <list>

#include "src/data/Speech.h"

class SpeechManager {
  public:
    void add(Speech * speech);
    Speech * get();
  private:
    std::list<Speech *> buffer;
};

#endif // _SPEECH_MANAGER_H_
