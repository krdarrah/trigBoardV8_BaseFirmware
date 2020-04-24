#include "includes.h"

const char fwVersion[] = "4/08/20";

void setup() {
  pinMode(ESPlatchPin, OUTPUT);
  digitalWrite(ESPlatchPin, HIGH);
  pinMode(LEDpin, OUTPUT);
  Serial.begin(115200);
  checkWakeupPins();
  loadConfiguration(filename, config);
  rtcInit(config.timerCountDown, false);
  Serial.println(getBattery(), 2);
  if (pushLogic()) { //decide if push will occur or nt and what message will be
    if (wiFiNeeded) {
      if (connectWiFi()) {
        pushOver();
        pushSafer();
        ifttt();
        mqtt();
      }
    }
    udp();
  }

  killPower();
  waitForButton();
  initBluetooth();
}

void loop() {
  serviceBluetooth();
  checkOTA();
}
