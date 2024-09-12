#pragma once

#include <PeriodicAction.h>
#include <Clock.h>

class NTP : public PeriodicAction {
public:
  explicit NTP(unsigned long interval, Clock* clock);
  bool updateTime();

protected:
  bool run() override;

private:
  Clock* clock;
};
