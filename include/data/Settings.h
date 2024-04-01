#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <string>

namespace Settings {
  int32_t getLightMaxPower();
  int32_t getLighDuration();
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
  int32_t getStartingYear();
  int32_t getStartingMonth();
  int32_t getStartingWeek();
  int32_t getStartingDay();
  int32_t getStartingHour();
  float getMaxNumberOfDaysAwake();
  float getMaxNumberOfDaysFasting();
  float getStaminaRecoveryRatio();
  float getSatietyRecoveryRatio();
  int32_t getStaminaOverextendRatio();
  int32_t getSatietyOverextendRatio();
  float getBuyingPriceModifier();
  int32_t getPort();
  int32_t getSeed();
  std::string getMasterPassword();

  void setLightMaxPower(int32_t data);
  void setLighDuration(int32_t data);
  void setYearDuration(int32_t data); // in days
  void setMonthDuration(int32_t data); // in days
  void setWeekDuration(int32_t data); // in days
  void setHourDuration(int32_t data); // in minutes
  void setMinuteDuration(int32_t data); // in seconds/ticks
  void setStartingYear(int32_t data);
  void setStartingMonth(int32_t data);
  void setStartingWeek(int32_t data);
  void setStartingDay(int32_t data);
  void setStartingHour(int32_t data);
  void setMaxNumberOfDaysAwake(float data);
  void setMaxNumberOfDaysFasting(float data);
  void setStaminaRecoveryRatio(float data);
  void setSatietyRecoveryRatio(float data);
  void setStaminaOverextendRatio(int32_t data);
  void setSatietyOverextendRatio(int32_t data);
  void setBuyingPriceModifier(float data);
  void setPort(int32_t data);
  void setSeed(int32_t data);
  void setMasterPassword(std::string data);
};

#endif // _SETTINGS_H_
