#ifndef _SPEECH_MANAGER_H_
#define _SPEECH_MANAGER_H_

#include <list>
#include <mutex>

#include "data/Speech.h"

class SpeechManager {
  public:
    static void add(Speech * speech);
    static std::list<Speech *> get();
    static void clear();
};

#endif // _SPEECH_MANAGER_H_
