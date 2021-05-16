

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void mqtt()
{
  if (strcmp(config.mqttEnable, "t") == 0)
  { //only if enabled

    Serial.println("sending mqtt");

    WiFiClient espClient;
    PubSubClient client(espClient);
    client.setServer(config.mqttServer, config.mqttPort);
    client.setCallback(callback);
    unsigned long mqttStart = millis();
    while (!client.connected())
    {
      if (millis() - mqttStart > config.wifiTimeout)
      {
        return;
      }
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect

      if (strcmp(config.mqttSecureEnable, "t") == 0)
      {
        client.connect(config.trigName, config.mqttUser, config.mqttPW);
      }
      else
      {
        client.connect(config.trigName);
      }

      if (client.connected())
      {
        Serial.println("connected");
        // Once connected, publish an announcement...
        char mqttMessage[70];

        if (config.homeAssistantIntegration == 1)
        { // push data to homeassisant
          char status[7];
          char lowBattString[6];
          char timerWakeString[6];

          char stateTopic[20];
          sprintf(stateTopic, "%s/%s/state", config.homeAssistantPrefix, WiFi.macAddress());
          //Create & Send discovery topic for each value : sensorString, lowBattString, batCharString, timerWakeString,
          for (int i = 0; i < 3; i++)
          {
            char mqttDiscoveryMessage[5];
            char mqttDiscoveryTopic[10];
            //topic is {HomeAssitantPrefix}/binary_sensor/{MACAdress}/config
            char valueTemplate[30];
            char name[10];
            char device_class[10];
            switch (i)
            {
              case STATUT:
                sprintf(mqttDiscoveryTopic, "%s/binary_sensor/status%s/config", config.homeAssistantPrefix, WiFi.macAddress());
                sprintf(valueTemplate, "{{value_json.status}}");
                sprintf(name, "Status");
                sprintf(device_class, "door");
                break;
              case LOW_BATT:
                sprintf(mqttDiscoveryTopic, "%s/sensor/lowbatt%s/config", config.homeAssistantPrefix, WiFi.macAddress());
                sprintf(valueTemplate, "{{value_json.low_batt}}");
                sprintf(name, "Low Batt");
                sprintf(device_class, "");
                break;
              case BATT_V:
                sprintf(mqttDiscoveryTopic, "%s/sensor/batt%s/config", config.homeAssistantPrefix, WiFi.macAddress());
                sprintf(valueTemplate, "{{value_json.batt_v}}");
                sprintf(name, "Tension");
                sprintf(device_class, "voltage");
                break;
              case TIME_WAKE:
                sprintf(mqttDiscoveryTopic, "%s/sensor/waketime%s/config", config.homeAssistantPrefix, WiFi.macAddress());
                sprintf(valueTemplate, "{{value_json.timer_wake}}");
                sprintf(name, "WakeTime");
                sprintf(device_class, "timestamp");
                break;
            }
            sprintf(mqttDiscoveryMessage, "{name: \'%s\', ~:\'%s\', val_tpl:\'%s\', dev_cla=\'%s\'}", name, stateTopic, valueTemplate);
            Serial.println(client.publish(mqttDiscoveryTopic, mqttDiscoveryMessage));
          }
          //Create & Send message
          if (contactLatchClosed)
          {
            sprintf(status, "off");
          }
          else
          {
            sprintf(status, "on");
          }
          if (lowBattery)
          {
            sprintf(lowBattString, "true");
          }
          else
          {
            sprintf(lowBattString, "false");
          }
          if (timerWake)
          {
            sprintf(timerWakeString, "true");
          }
          else
          {
            sprintf(timerWakeString, "false");
          }
          sprintf(mqttMessage, "{\"status\": \"%s\", \"batt_v\": %s, \"low_batt\": %s, \"timer_wake\": %s}",
                  status,
                  batCharString,
                  lowBattString,
                  timerWakeString);
          Serial.println(client.publish(stateTopic, mqttMessage));
        }
        else
        { // handle message as usual
          sprintf(mqttMessage, "%s %s", config.trigName, pushMessage);
          Serial.println(client.publish(config.mqttTopic, mqttMessage));
          delay(20);
        }
        // ... and resubscribe
        //client.subscribe("inTopic");
        return;
      }
      else
      {
        Serial.print("failed, rc=");
        Serial.println(client.state());

        delay(100);
      }
    }
  }
}
