#include "data/Settings.h"

namespace Settings {
  int getLightMaxPower() { return LIGHT_MAX_POWER; }
  int getLighDuration() { return LIGHT_DURATION; }
  int getYearDuration() { return YEAR_DURATION; } // in days
  int getMonthDuration() { return MONTH_DURATION; } // in days
  int getWeekDuration() { return WEEK_DURATION; } // in days
  int getDayDuration() { return 2 * getLightMaxPower(); } // in hours
  int getHourDuration() { return HOUR_DURATION; } // in minute
  int getYearDurationInRound() { return getYearDuration() * getDayDurationInRound(); } // in ticks
  int getMonthDurationInRound() { return getMonthDuration() * getDayDurationInRound(); } // in ticks
  int getWeekDurationInRound() { return getWeekDuration() * getDayDurationInRound(); } // in ticks
  int getDayDurationInRound() { return getDayDuration() * getHourDurationInRound() ; } // in ticks
  int getHourDurationInRound() { return getLighDuration(); } // in ticks
  int getStartingYear() { return 0; }
  int getStartingMonth() { return 0; }
  int getStartingWeek() { return 0; }
  int getStartingDay() { return 0; }
  int getStartingHour() { return 4; }
  int getPort() { return PORT; }
  float getMaxNumberOfDaysAwake() { return MAX_NUMBER_DAYS_AWAKE; }
  float getMaxNumberOfDaysFasting() { return MAX_NUMBER_DAYS_FASTING; }
  float getStaminaRecoveryRatio() { return STAMINA_RECOVERY_RATIO; }
  float getSatietyRecoveryRatio() { return SATIETY_RECOVERY_RATIO; }
  int getStaminaOverextendRatio() { return STAMINA_OVEREXTEND_RATIO; }
  int getSatietyOverextendRatio() { return SATIETY_OVEREXTEND_RATIO; }
};
