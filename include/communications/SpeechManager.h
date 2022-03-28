#ifndef _SPEECH_MANAGER_H_
#define _SPEECH_MANAGER_H_

#include <list>
#include <mutex>

#include "data/Speech.h"

class SpeechManager {
  public:
    static void add(const Speech * speech);
    static const Speech * get();
};

#endif // _SPEECH_MANAGER_H_
