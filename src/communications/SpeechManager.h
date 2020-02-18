#ifndef _SPEECH_MANAGER_H_
#define _SPEECH_MANAGER_H_

#include <list>
#include <mutex>

#include "src/data/Speech.h"

class SpeechManager {
  public:
    static void add(Speech * speech);
    static Speech * get();
};

#endif // _SPEECH_MANAGER_H_
