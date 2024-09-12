#include "Clock.h"

#define CLOCK_TICK_INTERRUPT_PIN D3

volatile bool tick;

IRAM_ATTR void clockTick() {
  tick = true;
}

Clock::Clock(RTC_DS3231* rtc)
: rtc(rtc) {
  while (!rtc->begin()) {
    printf("[Clock] Failed to find RTC.\n");
  }
  rtc->disable32K();
  rtc->writeSqwPinMode(DS3231_SquareWave1Hz);
  pinMode(CLOCK_TICK_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLOCK_TICK_INTERRUPT_PIN), clockTick, FALLING);
}

void Clock::check() {
  if (tick) {
    if (rtc->lostPower()) {
      printf("[Clock] Time not set\n");
    }
    tick = false;
  }
}

DateTime Clock::getTime() {
  return rtc->now();
}

void Clock::setTime(time_t unixTime) {
  printf("[Clock] Setting time to %lld\n", unixTime);
  rtc->adjust(DateTime(unixTime));
}

float Clock::getTemperature() {
  return rtc->getTemperature();
}
