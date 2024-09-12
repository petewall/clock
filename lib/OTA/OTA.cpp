#include <OTA.h>
#include <Arduino.h>

unsigned long ota_progress_millis = 0;

void OTA::onStart() {
  Serial.println("OTA update started!");
  // Blank screen
}

void OTA::onProgress(size_t current, size_t final) {
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
  // Progress bar
}

void OTA::onEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
    // Screen flash
  } else {
    Serial.println("There was an error during OTA update!");
    // Screen X's
  }
}
