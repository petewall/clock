#pragma once

#include <PeriodicAction.h>
#include <Adafruit_VCNL4010.h>
#include <stdint.h>

class ProxSensor : public PeriodicAction {
public:
  explicit ProxSensor(unsigned long interval, Adafruit_VCNL4010* sensor);

  virtual bool run() override;

  uint16_t getAmbientLight();
  uint16_t getProximity();
  bool isProximityTriggered();

private:
  Adafruit_VCNL4010* sensor;
  uint16_t lastProximity;
  bool proximityTriggered;
};
