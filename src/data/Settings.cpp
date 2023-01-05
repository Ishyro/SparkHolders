#include "data/Settings.h"

// time
#include<ctime>

namespace Settings {
  int lightMaxPower;
  int lighDuration;
  int yearDuration;
  int monthDuration;
  int weekDuration;
  int hourDuration;
  int startingYear;
  int startingMonth;
  int startingWeek;
  int startingDay;
  int startingHour;
  float maxNumberOfDaysAwake;
  float maxNumberOfDaysFasting;
  float staminaRecoveryRatio;
  float satietyRecoveryRatio;
  int staminaOverextendRatio;
  int satietyOverextendRatio;
  float buyingPriceModifier;
  int port;
  int seed;

  int getLightMaxPower() { return lightMaxPower; }
  int getLighDuration() { return lighDuration; }
  int getYearDuration() { return yearDuration; } // in days
  int getMonthDuration() { return monthDuration; } // in days
  int getWeekDuration() { return weekDuration; } // in days
  int getDayDuration() { return 2 * getLightMaxPower(); } // in hours
  int getHourDuration() { return hourDuration; } // in minutes
  int getYearDurationInRound() { return getYearDuration() * getDayDurationInRound(); } // in rounds
  int getMonthDurationInRound() { return getMonthDuration() * getDayDurationInRound(); } // in rounds
  int getWeekDurationInRound() { return getWeekDuration() * getDayDurationInRound(); } // in rounds
  int getDayDurationInRound() { return getDayDuration() * getHourDurationInRound() ; } // in rounds
  int getHourDurationInRound() { return getLighDuration(); } // in rounds
  int getStartingYear() { return startingYear; }
  int getStartingMonth() { return startingMonth; }
  int getStartingWeek() { return startingWeek; }
  int getStartingDay() { return startingDay; }
  int getStartingHour() { return startingHour; }
  float getMaxNumberOfDaysAwake() { return maxNumberOfDaysAwake; }
  float getMaxNumberOfDaysFasting() { return maxNumberOfDaysFasting; }
  float getStaminaRecoveryRatio() { return staminaRecoveryRatio; }
  float getSatietyRecoveryRatio() { return satietyRecoveryRatio; }
  int getStaminaOverextendRatio() { return staminaOverextendRatio; }
  int getSatietyOverextendRatio() { return satietyOverextendRatio; }
  float getBuyingPriceModifier() { return buyingPriceModifier; }
  int getPort() { return port; }
  int getSeed() { return seed; }

  void setLightMaxPower(int data) { lightMaxPower = data; }
  void setLighDuration(int data) { lighDuration = data; }
  void setYearDuration(int data) { yearDuration = data; } // in days
  void setMonthDuration(int data) { monthDuration = data; } // in days
  void setWeekDuration(int data) { weekDuration = data; } // in days
  void setHourDuration(int data) { hourDuration = data; } // in minutes
  void setStartingYear(int data) { startingYear = data; }
  void setStartingMonth(int data) { startingMonth = data; }
  void setStartingWeek(int data) { startingWeek = data; }
  void setStartingDay(int data) { startingDay = data; }
  void setStartingHour(int data) { startingHour = data; }
  void setMaxNumberOfDaysAwake(float data) { maxNumberOfDaysAwake = data; }
  void setMaxNumberOfDaysFasting(float data) { maxNumberOfDaysFasting = data; }
  void setStaminaRecoveryRatio(float data) { staminaRecoveryRatio = data; }
  void setSatietyRecoveryRatio(float data) { satietyRecoveryRatio = data; }
  void setStaminaOverextendRatio(int data) { staminaOverextendRatio = data; }
  void setSatietyOverextendRatio(int data) { satietyOverextendRatio = data; }
  void setBuyingPriceModifier(float data) { buyingPriceModifier = data; }
  void setPort(int data) { port = data; }
  void setSeed(int data) { seed = data; }
};
