#include "ProxSensor.h"
#include <numeric>

// #define PROXIMITY_ACTION_INTERRUPT_PIN D4


#define PROXIMITY_LED_POWER 200   // 200mA LED power (maximum)
#define PROXIMITY_THRESHOLD 2300  // ~ 3" away

#define AMBIENT_LIGHT_AVERAGING 16
unsigned short ambientLightMeasurementIndex = 0;
uint16_t ambientLightMeasurements[AMBIENT_LIGHT_AVERAGING] = {0};

ProxSensor::ProxSensor(unsigned long interval, Adafruit_VCNL4010* sensor)
: PeriodicAction(interval), sensor(sensor), lastProximity(0), proximityTriggered(false) {
  while (!sensor->begin()) {
    printf("[ProxSensor] Failed to start VCNL4010\n");
  }

  sensor->setLEDcurrent(PROXIMITY_LED_POWER);
  sensor->setFrequency(VCNL4010_62_5);
}

bool ProxSensor::run() {
  ambientLightMeasurements[ambientLightMeasurementIndex] = sensor->readAmbient();
  ambientLightMeasurementIndex = (ambientLightMeasurementIndex + 1) % AMBIENT_LIGHT_AVERAGING;

  this->lastProximity = sensor->readProximity();
  if (this->lastProximity > PROXIMITY_THRESHOLD) {
    if (!this->proximityTriggered) {
      this->proximityTriggered = true;
    }
  } else {
    this->proximityTriggered = false;
  }

  return true;
}

uint16_t ProxSensor::getAmbientLight() {
  return std::accumulate(ambientLightMeasurements, ambientLightMeasurements + AMBIENT_LIGHT_AVERAGING, 0) / AMBIENT_LIGHT_AVERAGING;
}

uint16_t ProxSensor::getProximity() {
  return this->lastProximity;
}

bool ProxSensor::isProximityTriggered() {
  return this->proximityTriggered;
}
