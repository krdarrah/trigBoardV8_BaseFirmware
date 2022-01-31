#define OTA_DEBUG
#include "includes.h"

const char fwVersion[] = "11/29/21";

void setup() {

  pinMode(ESPlatchPin, OUTPUT);
  digitalWrite(ESPlatchPin, HIGH);
  pinMode(LEDpin, OUTPUT);
  Serial.begin(115200);
  Wire.begin(SDApin, SCLpin);
  checkWakeupPins();
  loadConfiguration(filename, config);
  rtcInit(config.timerCountDown, false);
  
  
  Serial.println(getBattery(), 2);
  if (pushLogic()) { //decide if push will occur or nt and what message will be
    timestampAppend();
    if (wiFiNeeded) {
      if (connectWiFi()) {
        nptUpdateTime();
        pushOver();
        pushSafer();
        ifttt();
        telegram();
        mqtt();
      }
    }
    udp();
    tcp();
  }
  killPower();
  waitForButton();
  initBluetooth();
}

void loop() {
  if (!OTAsetup)
    serviceBluetooth();
  else
    checkOTA();
}
