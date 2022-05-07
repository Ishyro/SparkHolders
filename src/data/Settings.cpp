#include "data/Settings.h"

namespace Settings {
  int getLightMaxPower() { return LIGHT_MAX_POWER; }
  int getLighDuration() { return LIGHT_DURATION; }
  int getYearDuration() { return YEAR_DURATION; } // in days
  int getMonthDuration() { return MONTH_DURATION; } // in days
  int getWeekDuration() { return WEEK_DURATION; } // in days
  int getDayDuration() { return 2 * getLightMaxPower(); } // in hours
  int getHourDuration() { return HOUR_DURATION; } // in minute
  int getYearDurationInTick() { return getYearDuration() * getDayDurationInTick(); } // in ticks
  int getMonthDurationInTick() { return getMonthDuration() * getDayDurationInTick(); } // in ticks
  int getWeekDurationInTick() { return getWeekDuration() * getDayDurationInTick(); } // in ticks
  int getDayDurationInTick() { return getDayDuration() * getHourDurationInTick() ; } // in ticks
  int getHourDurationInTick() { return getLighDuration(); } // in ticks
  int getStartingYear() { return 0; }
  int getStartingMonth() { return 0; }
  int getStartingWeek() { return 0; }
  int getStartingDay() { return 0; }
  int getStartingHour() { return 4; }
  int getPort() { return PORT; }
};
