#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define LIGHT_MAX_POWER 10
#define LIGHT_DURATION 10

#define YEAR_DURATION 500
#define MONTH_DURATION 50
#define WEEK_DURATION 5
#define HOUR_DURATION 100

#define PORT 45678

namespace Settings {
  int getLightMaxPower();
  int getLighDuration();
  int getYearDuration(); // in days
  int getMonthDuration(); // in days
  int getWeekDuration(); // in days
  int getDayDuration(); // in hours
  int getHourDuration(); // in minutes
  int getYearDurationInTick(); // in ticks
  int getMonthDurationInTick(); // in ticks
  int getWeekDurationInTick(); // in ticks
  int getDayDurationInTick(); // in ticks
  int getHourDurationInTick(); // in ticks
  int getStartingYear();
  int getStartingMonth();
  int getStartingWeek();
  int getStartingDay();
  int getStartingHour();
  int getPort();
};

#endif // _SETTINGS_H_
