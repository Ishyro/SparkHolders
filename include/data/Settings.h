#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define LIGHT_MAX_POWER 10
#define LIGHT_DURATION 50

#define YEAR_DURATION 500
#define MONTH_DURATION 50
#define WEEK_DURATION 5
#define HOUR_DURATION 50

#define PORT 45678

#define MAX_NUMBER_DAYS_AWAKE 3
#define MAX_NUMBER_DAYS_FASTING 3

#define STAMINA_RECOVERY_RATIO 3
#define SATIETY_RECOVERY_RATIO 3

#define STAMINA_OVEREXTEND_RATIO 2
#define SATIETY_OVEREXTEND_RATIO 2

namespace Settings {
  int getLightMaxPower();
  int getLighDuration();
  int getYearDuration(); // in days
  int getMonthDuration(); // in days
  int getWeekDuration(); // in days
  int getDayDuration(); // in hours
  int getHourDuration(); // in minutes
  int getYearDurationInRound(); // in ticks
  int getMonthDurationInRound(); // in ticks
  int getWeekDurationInRound(); // in ticks
  int getDayDurationInRound(); // in ticks
  int getHourDurationInRound(); // in ticks
  int getStartingYear();
  int getStartingMonth();
  int getStartingWeek();
  int getStartingDay();
  int getStartingHour();
  int getPort();
  float getMaxNumberOfDaysAwake();
  float getMaxNumberOfDaysFasting();
  float getStaminaRecoveryRatio();
  float getSatietyRecoveryRatio();
  int getStaminaOverextendRatio();
  int getSatietyOverextendRatio();
  float getBuyingPriceModifier();
};

#endif // _SETTINGS_H_
