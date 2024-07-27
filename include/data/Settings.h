#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <string>
#include <vector>
#include <chrono>

#include "util/MathUtil.h"

namespace Settings {
  int32_t getYearDuration(); // in days
  int32_t getMonthDuration(); // in days
  int32_t getWeekDuration(); // in days
  int32_t getDayDuration(); // in hours
  int32_t getHourDuration(); // in minutes
  int32_t getMinuteDuration(); // in seconds/tick
  int32_t getYearDurationInRound(); // in rounds
  int32_t getMonthDurationInRound(); // in rounds
  int32_t getWeekDurationInRound(); // in rounds
  int32_t getDayDurationInRound(); // in rounds
  int32_t getHourDurationInRound(); // in rounds
  float getTickDurationInSeconds();
  std::chrono::milliseconds getTickTimer();
  int32_t getStartingYear();
  int32_t getStartingMonth();
  int32_t getStartingDay();
  int32_t getStartingHour();
  int64_t getLongitudeOrigin();
  int64_t getLatitudeOrigin();
  float getSecondToMeter();
  int32_t getZenithLightPower(int32_t indice); // week
  int32_t getNightLightPower(int32_t indice); // week
  bool getTidalLocked();
  MathUtil::Vector3i getShadowAngle();
  int32_t getNightDuration();
  int32_t getDawnDuration();
  int32_t getDaytimeDuration();
  int32_t getDuskDuration();
  std::string getMonthName(int32_t indice); // yearDuration / monthDuration
  std::string getDayName(int32_t indice); // weekDuration
  float getBuyingPriceModifier();
  int32_t getPort();
  int64_t getSeed();
  std::string getMasterPassword();
  int32_t getStatusThreshold(int32_t indice);

  void setYearDuration(int32_t data); // in days
  void setMonthDuration(int32_t data); // in days
  void setWeekDuration(int32_t data); // in days
  void setDayDuration(int32_t data); // in hours
  void setHourDuration(int32_t data); // in minutes
  void setMinuteDuration(int32_t data); // in seconds
  void setTickDurationInSeconds(float data);
  void setTickTimer(int32_t data);
  void setStartingYear(int32_t data);
  void setStartingMonth(int32_t data);
  void setStartingDay(int32_t data);
  void setStartingHour(int32_t data);
  void setOrigin(int64_t data1, int64_t data2);
  void setSecondToMeter(float data);
  void setZenithLightPower(int32_t indice, int32_t data); // week, in lux
  void setNightLightPower(int32_t indice, int32_t data); // week, in lux
  void setTidalLocked(bool data);
  void setShadowAngle(MathUtil::Vector3i data);
  void setNightDuration(int32_t data);
  void setDawnDuration(int32_t data);
  void setDaytimeDuration(int32_t data);
  void setDuskDuration(int32_t data);
  void setMonthName(int32_t indice, std::string data); // yearDuration / monthDuration
  void setDayName(int32_t indice, std::string data); // weekDuration
  void setBuyingPriceModifier(float data);
  void setPort(int32_t data);
  void setSeed(int64_t data);
  void setMasterPassword(std::string data);
  void setStatusThreshold(int32_t indice, int32_t data);
};

#endif // _SETTINGS_H_
