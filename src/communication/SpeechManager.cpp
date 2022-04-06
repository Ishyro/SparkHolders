#include "communication/SpeechManager.h"

std::list<const Speech *> buffer;
std::mutex mtx;

void SpeechManager::add(const Speech * speech) {
  std::lock_guard<std::mutex> lck (mtx);
  buffer.push_back(speech);
}

const Speech * SpeechManager::get() {
  std::lock_guard<std::mutex> lck (mtx);
  const Speech * speech = buffer.front();
  buffer.pop_front();
  return speech;
}
