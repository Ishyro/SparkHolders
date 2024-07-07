#include "data/Settings.h"

// time
#include<ctime>

namespace Settings {
  int32_t yearDuration;
  int32_t monthDuration;
  int32_t weekDuration;
  int32_t dayDuration;
  int32_t hourDuration;
  int32_t minuteDuration;
  float tickDurationInSeconds;
  int32_t startingYear;
  int32_t startingMonth;
  int32_t startingDay;
  int32_t startingHour;
  int64_t longitudeOrigin;
  int64_t latitudeOrigin;
  float secondToMeter;
  std::vector<int32_t> zenithLightPower; // week
  std::vector<int32_t> nightLightPower; // week
  bool tidalLocked;
  MathUtil::Vector3i shadowAngle;
  int32_t nightDuration;
  int32_t dawnDuration;
  int32_t daytimeDuration;
  int32_t duskDuration;
  std::vector<std::string> monthNames; // yearDuration / monthDuration
  std::vector<std::string> dayNames; // weekDuration
  std::array<int32_t, DAMAGE_TYPE_STATUS_NUMBER> statusThreshold;
  float buyingPriceModifier;
  std::chrono::milliseconds tickTimer;
  int32_t port;
  int64_t seed;
  std::string masterPassword;

  int32_t getYearDuration() { return yearDuration; } // in days
  int32_t getMonthDuration() { return monthDuration; } // in days
  int32_t getWeekDuration() { return weekDuration; } // in days
  int32_t getDayDuration() { return dayDuration; } // in hours
  int32_t getHourDuration() { return hourDuration; } // in minutes
  int32_t getMinuteDuration() { return minuteDuration; } // in seconds/tick
  int32_t getYearDurationInRound() { return getYearDuration() * getDayDurationInRound(); } // in rounds
  int32_t getMonthDurationInRound() { return getMonthDuration() * getDayDurationInRound(); } // in rounds
  int32_t getWeekDurationInRound() { return getWeekDuration() * getDayDurationInRound(); } // in rounds
  int32_t getDayDurationInRound() { return getDayDuration() * getHourDurationInRound() ; } // in rounds
  int32_t getHourDurationInRound() { return getHourDuration(); } // in rounds
  int32_t getStartingYear() { return startingYear; }
  int32_t getStartingMonth() { return startingMonth; }
  int32_t getStartingDay() { return startingDay; }
  int32_t getStartingHour() { return startingHour; }
  int64_t getLongitudeOrigin() { return longitudeOrigin; }
  int64_t getLatitudeOrigin() { return latitudeOrigin; }
  float getSecondToMeter() { return secondToMeter; }
  float getTickDurationInSeconds() { return tickDurationInSeconds; }
  std::chrono::milliseconds getTickTimer() { return tickTimer; }
  int32_t getZenithLightPower(int32_t indice) { return zenithLightPower[indice]; } // week
  int32_t getNightLightPower(int32_t indice) { return nightLightPower[indice]; } // week
  bool getTidalLocked() { return tidalLocked; }
  MathUtil::Vector3i getShadowAngle() { return shadowAngle; }
  int32_t getNightDuration() { return nightDuration; }
  int32_t getDawnDuration() { return dawnDuration; }
  int32_t getDaytimeDuration() { return daytimeDuration; }
  int32_t getDuskDuration() { return duskDuration; }
  std::string getMonthName(int32_t indice) { return monthNames[indice]; } // yearDuration / monthDuration
  std::string getDayName(int32_t indice) { return dayNames[indice]; } // weekDuration
  float getBuyingPriceModifier() { return buyingPriceModifier; }
  int32_t getPort() { return port; }
  int64_t getSeed() { return seed; }
  std::string getMasterPassword() { return masterPassword; }
  int32_t getStatusThreshold(int32_t indice) { return statusThreshold[indice]; }

  void setYearDuration(int32_t data) { yearDuration = data; } // in days
  void setMonthDuration(int32_t data) { // in days
    monthDuration = data;
    monthNames = std::vector<std::string>(yearDuration / monthDuration);
  }
  void setWeekDuration(int32_t data) { // in days
    weekDuration = data;
    zenithLightPower = std::vector<int32_t>(weekDuration);
    nightLightPower = std::vector<int32_t>(weekDuration);
    dayNames = std::vector<std::string>(weekDuration);
  } 
  void setDayDuration(int32_t data) { dayDuration = data; } // in hours
  void setHourDuration(int32_t data) { hourDuration = data; } // in minutes
  void setMinuteDuration(int32_t data) { minuteDuration = data; } // in seconds/minutes
  void setStartingYear(int32_t data) { startingYear = data; }
  void setStartingMonth(int32_t data) { startingMonth = data; }
  void setStartingDay(int32_t data) { startingDay = data; }
  void setStartingHour(int32_t data) { startingHour = data; }
  void setOrigin(int64_t data1, int64_t data2) { longitudeOrigin = data1; latitudeOrigin = data2; }
  void setSecondToMeter(float data) { secondToMeter = data; }
  void setTickDurationInSeconds(float data) {tickDurationInSeconds = data; }
  void setTickTimer(int32_t data) { tickTimer = std::chrono::milliseconds(data); }
  void setZenithLightPower(int32_t indice, int32_t data) { zenithLightPower[indice] = data; } // week
  void setNightLightPower(int32_t indice, int32_t data) { nightLightPower[indice] = data; } // week
  void setTidalLocked(bool data) { tidalLocked = data; }
  void setShadowAngle(MathUtil::Vector3i data) { shadowAngle = data; }
  void setNightDuration(int32_t data) { nightDuration = data; }
  void setDawnDuration(int32_t data) { dawnDuration = data; }
  void setDaytimeDuration(int32_t data) { daytimeDuration = data; }
  void setDuskDuration(int32_t data) { duskDuration = data; }
  void setMonthName(int32_t indice, std::string data) { monthNames[indice] = data; } // yearDuration / monthDuration
  void setDayName(int32_t indice, std::string data) { dayNames[indice] = data; } // weekDuration
  void setBuyingPriceModifier(float data) { buyingPriceModifier = data; }
  void setPort(int32_t data) { port = data; }
  void setSeed(int64_t data) { seed = data; }
  void setMasterPassword(std::string data) { masterPassword = data; }
  void setStatusThreshold(int32_t indice, int32_t data) { statusThreshold[indice] = data; }
};
