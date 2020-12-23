char tcpMessage[100];
boolean sendTCPmessage() {
  WiFiClient client;
  if (client.connect(config.udpTargetIP, 80)) {//port 80 by default, but suppose you could use the udp port as an option
    Serial.println("sending data");
    client.println(tcpMessage);
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println();
    //let's wait for something to come back... only a few seconds
    unsigned long startTimePush = millis();
    while (millis() - startTimePush < 1000) {
      if (client.available()) {
        Serial.println("From Server");
        while (client.available() != 0) {
          Serial.print(char(client.read()));
        }
        client.stop();
        Serial.println("");
        return true;
      }
    }
    client.stop();
    return false;//nothing came back

  } else {
    Serial.println("TCP connection failed");
    client.stop();
    return false;
  }
}



//wifi event handler
boolean tcpConnected = false;
void tcpWiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      //When connected set
      tcpConnected = true;
      break;
    //    case SYSTEM_EVENT_STA_DISCONNECTED:
    //      Serial.println("WiFi lost connection");
    //      connected = false;
    //      break;
    default: break;
  }
}
boolean tcpWiFiConnect() {
  WiFi.disconnect(true);
  tcpConnected = false;
  WiFi.begin(config.udpSSID, config.udpPW);
  unsigned long wifiStart = millis();
  while (!tcpConnected) {
    if (millis() - wifiStart > config.wifiTimeout) {
      Serial.println("Couldn't connect to network");
      //killPower();
      return false;
    }
  }
  Serial.println("Connected");
  return true;
}



void tcp() {
  if (strcmp(config.tcpEnable, "t") == 0) {//only if enabled
    Serial.println("TCP MESSAGE");

    //tcp shares same udp setting, since that code was written first
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

    WiFi.onEvent(tcpWiFiEvent);
    WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS);


    //const char * udpAddress = config.udpTargetIP;
    //const int udpPort = config.udpPort;

    char tcpPushMessage[100];
    sprintf(tcpPushMessage, "%s %s$$$%li", config.trigName, pushMessage, micros());
    //strip out spaces and replace with + so can be passed by parameter
    for (int i = 0; i < strlen(tcpPushMessage); i++) {
      if (tcpPushMessage[i] == ' ')
        tcpPushMessage[i] = '+';
    }
    sprintf(tcpMessage, "GET /trigBoard?message=%s HTTP/1.1", tcpPushMessage);

    for (int i = 0; i < config.udpBlastCount; i++) {
      Serial.printf("TCP ATTEMPT #%i ***************************\n", i);
      if (tcpWiFiConnect()) {
        if (sendTCPmessage())
          break;
        if (i == config.udpBlastCount - 1) {
          Serial.println("TCP FAILED FAILED FAILED FAILED FAILED");
          //        while (1) {}
          break;
        }
      }
    }


  }
}
