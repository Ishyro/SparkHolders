#include "src/communications/SpeechManager.h"

void SpeechManager::add(Speech * speech) {buffer.push_back(speech);}
Speech * SpeechManager::get() {
  Speech * speech = buffer.front();
  buffer.pop_front();
  return speech;
}
