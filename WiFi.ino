bool connectWiFi() {
  Serial.print(F("connecting to WiFi - timeout="));
  Serial.println(config.wifiTimeout);
  if (strcmp(config.staticIPenable, "t") == 0) {//if static
    Serial.println("Setting up Static IP");
    //WiFi.disconnect();
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


  
  unsigned long wifiStart = millis();
  WiFi.begin(config.ssid, config.pw);
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - wifiStart > config.wifiTimeout) {
      killPower();
      return false;
    }

  }

  Serial.println("Connected");
  return true;
}
