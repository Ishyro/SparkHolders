#ifndef _LOGGER_H_
#define _LOGGER_H_

#ifdef LOG

#include <ostream>
#include <fstream>
#include <mutex>

#include "Values.h"

namespace Logger {
  void init_logger(std::string path);
  void logger(std::string level, std::string msg);
  void close_logger();
};
#define LOGGER_TRACE(str) Logger::logger("TRACE", str)
#define LOGGER_INFO(str) Logger::logger("INFO", str)
#define LOGGER_DEBUG(str) Logger::logger("DEBUG", str)
#define LOGGER_WARNING(str) Logger::logger("WARNING", str)
#define LOGGER_ERROR(str) Logger::logger("ERROR", str)
#define LOGGER_FATAL(str) Logger::logger("FATAL", str)
#else
#define LOGGER_TRACE(str) do {} while(0)
#define LOGGER_INFO(str) do {} while(0)
#define LOGGER_DEBUG(str) do {} while(0)
#define LOGGER_WARNING(str) do {} while(0)
#define LOGGER_ERROR(str) do {} while(0)
#define LOGGER_FATAL(str) do {} while(0)
#endif

#endif // _LOGGER_H_
