//wifi event handler
boolean wifiConnected = false;
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      //When connected set
      wifiConnected = true;
      break;
    //    case SYSTEM_EVENT_STA_DISCONNECTED:
    //      Serial.println("WiFi lost connection");
    //      connected = false;
    //      break;
    default: break;
  }
}

bool connectWiFi() {
  Serial.print(F("connecting to WiFi - timeout="));
  Serial.println(config.wifiTimeout);
  if (strcmp(config.staticIPenable, "t") == 0) {//if static
    Serial.println("Setting up Static IP");
    getFourNumbersForIP(config.staticIP);
    Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
    IPAddress ip(oneIP, twoIP, threeIP, fourIP);
    getFourNumbersForIP(config.staticGatewayAddress);
    Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
    IPAddress gateway(oneIP, twoIP, threeIP, fourIP);
    getFourNumbersForIP(config.staticSubnetAddress);
    Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
    IPAddress subnet(oneIP, twoIP, threeIP, fourIP);
    getFourNumbersForIP(config.staticPrimaryDNSAddress);
    Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
    IPAddress primaryDNS(oneIP, twoIP, threeIP, fourIP);
    getFourNumbersForIP(config.staticSecondaryDNSAddress);
    Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
    IPAddress secondaryDNS(oneIP, twoIP, threeIP, fourIP);
    WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS);
  }
  Serial.printf("%s\n", config.ssid);

  wifiConnected = false;
  WiFi.onEvent(WiFiEvent);
  unsigned long wifiStart = millis();
  WiFi.begin(config.ssid, config.pw);
  while (!wifiConnected) {
    if (millis() - wifiStart > config.wifiTimeout) {
      killPower();
      return false;
    }
  }

  Serial.println("Connected");
  if (strcmp(config.appendRSSI, "t") == 0) {
    getRSSI();
    strcat(pushMessage, ",");
    strcat(pushMessage, rssiChar);
  }
  return true;
}

void getRSSI() {
  //Serial.print("WiFi RSSI = ");
  int rssiAbs = WiFi.RSSI();
  //Serial.println(rssiAbs);
  if (rssiAbs < 0);
  rssiAbs = rssiAbs * -1;
  sprintf(rssiChar, "%idBm", rssiAbs);//can't have any dashes (-)
}
