#ifdef LOG

#include <chrono>
#include <ctime> 

#include "util/Logger.h"

namespace Logger {
  std::ofstream log;
  std::mutex mutex;
  void init_logger(std::string path) { const std::lock_guard<std::mutex> guard(mutex); log.open(path); }
  void logger(std::string level, std::string msg) {
    const std::lock_guard<std::mutex> guard(mutex);
    std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string date = std::ctime(&current_time);
    date.erase(date.find('\n', 0), 1);
    log << level << " - " << date << " - " << msg << std::endl;
  }
  void close_logger() { log.close(); }
};

#endif
