#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <string>

namespace Settings {
  int getLightMaxPower();
  int getLighDuration();
  int getYearDuration(); // in days
  int getMonthDuration(); // in days
  int getWeekDuration(); // in days
  int getDayDuration(); // in hours
  int getHourDuration(); // in minutes
  int getMinuteDuration(); // in seconds/tick
  int getYearDurationInRound(); // in rounds
  int getMonthDurationInRound(); // in rounds
  int getWeekDurationInRound(); // in rounds
  int getDayDurationInRound(); // in rounds
  int getHourDurationInRound(); // in rounds
  int getStartingYear();
  int getStartingMonth();
  int getStartingWeek();
  int getStartingDay();
  int getStartingHour();
  float getMaxNumberOfDaysAwake();
  float getMaxNumberOfDaysFasting();
  float getStaminaRecoveryRatio();
  float getSatietyRecoveryRatio();
  int getStaminaOverextendRatio();
  int getSatietyOverextendRatio();
  float getBuyingPriceModifier();
  int getPort();
  int getSeed();
  std::string getMasterPassword();

  void setLightMaxPower(int data);
  void setLighDuration(int data);
  void setYearDuration(int data); // in days
  void setMonthDuration(int data); // in days
  void setWeekDuration(int data); // in days
  void setHourDuration(int data); // in minutes
  void setMinuteDuration(int data); // in seconds/ticks
  void setStartingYear(int data);
  void setStartingMonth(int data);
  void setStartingWeek(int data);
  void setStartingDay(int data);
  void setStartingHour(int data);
  void setMaxNumberOfDaysAwake(float data);
  void setMaxNumberOfDaysFasting(float data);
  void setStaminaRecoveryRatio(float data);
  void setSatietyRecoveryRatio(float data);
  void setStaminaOverextendRatio(int data);
  void setSatietyOverextendRatio(int data);
  void setBuyingPriceModifier(float data);
  void setPort(int data);
  void setSeed(int data);
  void setMasterPassword(std::string data);
};

#endif // _SETTINGS_H_
