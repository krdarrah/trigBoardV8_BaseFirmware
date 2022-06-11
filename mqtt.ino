

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

    //WiFiClientSecure client;
    //client.setInsecure();//trigger this with a config var, enable one-way mqtt ssl
    WiFiClient espClient;
    PubSubClient client(espClient);
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
