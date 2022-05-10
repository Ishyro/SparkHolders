#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define LIGHT_MAX_POWER 10
#define LIGHT_DURATION 10

#define YEAR_DURATION 500
#define MONTH_DURATION 50
#define WEEK_DURATION 5
#define HOUR_DURATION 50

#define PORT 45678

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
};

#endif // _SETTINGS_H_
