#pragma once

#include <RTClib.h>

class Clock {
public:
  explicit Clock(RTC_DS3231* rtc);
  void check();

  DateTime getTime();
  void setTime(time_t unixTime);

  float getTemperature();

private:
  RTC_DS3231* rtc;
};
