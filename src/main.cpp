// System dependencies
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif
#include <SPI.h>

// External dependencies
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

// Internal libraries
#include <Clock.h>
#include <NTP.h>
#include <OTA.h>
#include <ProxSensor.h>

AsyncWebServer* webServer;

Clock* rtClock;
NTP* ntp;
OTA* otaHandler;
ProxSensor* proxSensor;

#define FIVE_SECONDS 5 * 1000
#define MINUTE 60 * 1000
#define TEN_MINUTES 10 * MINUTE
#define ONE_DAY 24 * 60 * 60 * 1000

void connectToWiFi(void) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupOTA(void) {
  otaHandler = new OTA();
  ElegantOTA.onStart(std::bind(std::mem_fn(&OTA::onStart), otaHandler));
  ElegantOTA.onProgress(std::bind(std::mem_fn(&OTA::onProgress), otaHandler, std::placeholders::_1, std::placeholders::_2));
  ElegantOTA.onEnd(std::bind(std::mem_fn(&OTA::onEnd), otaHandler, std::placeholders::_1));
  ElegantOTA.begin(webServer);
}

void setupWebServer(void) {
  webServer = new AsyncWebServer(80);
  webServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo.");
  });
  webServer->begin();
  Serial.println("HTTP server started");
}

void setup(void) {
  Serial.begin(115200);
  connectToWiFi();

  setupWebServer();
  setupOTA();

  rtClock = new Clock(new RTC_DS3231());
  ntp = new NTP(ONE_DAY, rtClock);
  proxSensor = new ProxSensor(20, new Adafruit_VCNL4010());
}

unsigned long counter = 0;
void loop(void) {
  unsigned long now = millis();
  ElegantOTA.loop();
  ntp->check(now);
  rtClock->check();
  proxSensor->check(now);
  if (now - counter > 1000) {
    DateTime currentTime = rtClock->getTime();
    Serial.printf("time %s    prox: %u   light: %u   triggered: %s   temp: %f\n", currentTime.timestamp().c_str(), proxSensor->getProximity(), proxSensor->getAmbientLight(), proxSensor->isProximityTriggered() ? "true" : "false", rtClock->getTemperature());
    counter = now;
  }
}
