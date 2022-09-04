#include "communication/SpeechManager.h"

std::list<Speech *> buffer;

void SpeechManager::add(Speech * speech) {
  buffer.push_back(speech);
}

std::list<Speech *> SpeechManager::get() {
  return buffer;
}

void SpeechManager::clear() {
  buffer.clear();
}
