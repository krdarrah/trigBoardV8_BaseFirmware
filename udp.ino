//wifi event handler
boolean udpConnected = false;
void udpWiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      //When connected set
      udpConnected = true;
      break;
    //    case SYSTEM_EVENT_STA_DISCONNECTED:
    //      Serial.println("WiFi lost connection");
    //      connected = false;
    //      break;
    default: break;
  }
}


void getFourNumbersForIP(const char *ipChar) {
  char ipCharInput[20];
  strncpy(ipCharInput, ipChar, sizeof(ipCharInput));

  const char delimiter[] = ".";
  char parsedStrings[4][4];
  char *token =  strtok(ipCharInput, delimiter);
  if (token == NULL)
    return;
  strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
  for (int i = 1; i < 4; i++) {
    token =  strtok(NULL, delimiter);
    if (token == NULL)
      return;
    strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
  }
  oneIP = atoi(parsedStrings[0]);
  twoIP = atoi(parsedStrings[1]);
  threeIP = atoi(parsedStrings[2]);
  fourIP = atoi(parsedStrings[3]);
}


void udp() {
  if (strcmp(config.udpEnable, "t") == 0) {//only if enabled
    Serial.println("UDP MESSAGE");
    WiFi.disconnect(true);
    getFourNumbersForIP(config.udpStaticIP);
    IPAddress ip(oneIP, twoIP, threeIP, fourIP);
    getFourNumbersForIP(config.udpGatewayAddress);
    IPAddress gateway(oneIP, twoIP, threeIP, fourIP);
    getFourNumbersForIP(config.udpSubnetAddress);
    IPAddress subnet(oneIP, twoIP, threeIP, fourIP);
    getFourNumbersForIP(config.udpPrimaryDNSAddress);
    IPAddress primaryDNS(oneIP, twoIP, threeIP, fourIP);
    getFourNumbersForIP(config.udpSecondaryDNSAddress);
    IPAddress secondaryDNS(oneIP, twoIP, threeIP, fourIP);
    udpConnected = false;
    WiFi.onEvent(udpWiFiEvent);
    WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS);
    WiFi.begin(config.udpSSID, config.udpPW);
    unsigned long wifiStart = millis();
    while (!udpConnected) {
      if (millis() - wifiStart > config.wifiTimeout) {
        Serial.println("Couldn't connect");
        killPower();
        return;
      }
    }
    Serial.println("Connected");
    const char * udpAddress = config.udpTargetIP;
    const int udpPort = config.udpPort;

    WiFiUDP udp;

    for (int i = 0; i < config.udpBlastCount; i++) {
      if (udp.beginPacket(udpAddress, udpPort) == 1) {
        //if rssi append
        if (strcmp(config.appendRSSI, "t") == 0) {
          getRSSI();
          udp.printf("%s %s,%s", config.trigName, pushMessage, rssiChar);
        } else
          udp.printf("%s %s", config.trigName, pushMessage);
        if (udp.endPacket() == 1) {
          Serial.print(i);
          Serial.println(" success");
          delay(config.udptimeBetweenBlasts);
          //break;
        }
        else {
          delay(100);
          Serial.println("fail on end packet");
        }
      } else {
        delay(100);
        Serial.println("fail on begin packet");
      }
    }


  }
}
