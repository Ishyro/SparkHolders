#include "src/communications/SpeechManager.h"

std::list<Speech *> buffer;
std::mutex mtx;

static void SpeechManager::add(Speech * speech) {
  std::lock_guard<std::mutex> lck (mtx);
  buffer.push_back(speech);
}
static Speech * SpeechManager::get() {
  std::lock_guard<std::mutex> lck (mtx);
  Speech * speech = buffer.front();
  buffer.pop_front();
  return speech;
}
