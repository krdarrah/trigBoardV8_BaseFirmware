unsigned int oldProgress;
void setupOTA() {
  if (WiFi.status() == WL_CONNECTED) {
    ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
      oldProgress = 0;
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      unsigned int progressPercentage = (progress / (total / 100));
      if (progressPercentage >= oldProgress+2) {
        char progressChar[50];
        sprintf(progressChar, "%u%%", progressPercentage);
        Serial.printf("Progress: %u%%\n", progressPercentage);
        transmitData("OTAprog", progressChar);
        digitalWrite(LEDpin, !digitalRead(LEDpin));
        oldProgress = progressPercentage;
      }

    })
    .onError([](ota_error_t error) {
      transmitData("OTAprog", "ERROR");
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

    ArduinoOTA.begin();
    OTAsetup = true;
  }
}

void checkOTA() {
  if (WiFi.status() == WL_CONNECTED && OTAsetup) {
    ArduinoOTA.handle();
  }
}
