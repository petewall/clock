#include "NTP.h"
// #include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

NTP::NTP(unsigned long interval, Clock* clock)
: PeriodicAction(interval), clock(clock) {}

bool NTP::updateTime() {
  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }

  WiFiUDP client;
  NTPClient ntpClient(client, "pool.ntp.org");

  Serial.println("[NTP] Getting time...");
  if (ntpClient.update()) {
    clock->setTime(ntpClient.getEpochTime());
    Serial.println("[NTP] Done.");
    return true;
  } else {
    Serial.println("[NTP] Failed to get time.");
  }
  return false;
}

bool NTP::run() {
  return this->updateTime();
}
