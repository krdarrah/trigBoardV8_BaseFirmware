

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void mqtt() {
  if (strcmp(config.mqttEnable, "t") == 0) {//only if enabled
    
    Serial.println("sending mqtt");

    PubSubClient client;
    WiFiClientSecure secureClient;
    WiFiClient insecureClient;
    bool hasKey = strlen(mqttSSLKey) > 0;
    bool hasCert = strlen(mqttSSLCert) > 0;
    bool hasCA = strlen(mqttSSLCA) > 0;
    bool isSSL = (hasKey && hasCert) || (!hasKey && !hasCert && hasCA);
    if (strcmp(config.mqttSecureEnable, "t") == 0 && isSSL) {
      if(hasKey) {
        secureClient.setPrivateKey(mqttSSLKey); // for PKI encryption
      }
      if(hasCert) {
        secureClient.setCertificate(mqttSSLCert); // for x509 client verification
      }
      if(hasCA) {
        secureClient.setCACert(mqttSSLCA); // for signing trust
      }
      secureClient.setInsecure();
      client.setClient(secureClient);
    } else {
      client.setClient(insecureClient);
    }    
    
    client.setServer(config.mqttServer, config.mqttPort);
    client.setCallback(callback);
    unsigned long mqttStart = millis();
    while (!client.connected()) {
      if (millis() - mqttStart > config.wifiTimeout) {
        return;
      }
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect

      if (strcmp(config.mqttSecureEnable, "t") == 0) {
        client.connect(config.trigName, config.mqttUser, config.mqttPW);
      }
      else {
        client.connect(config.trigName);
      }

      if (client.connected()) {
        Serial.println("connected");
        // Once connected, publish an announcement...
        char mqttMessage[100];
        sprintf(mqttMessage, "%s %s", config.trigName, pushMessage);
        Serial.println(client.publish(config.mqttTopic, mqttMessage));
        delay(20);
        // ... and resubscribe
        //client.subscribe("inTopic");
        return;
      } else {
        Serial.print("failed, rc=");
        Serial.println(client.state());
        
        delay(100);
      }
    }
  }
}
