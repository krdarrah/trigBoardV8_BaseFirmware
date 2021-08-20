

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
    Serial.println ("MQTTT SERVER");
    Serial.println (config.mqttServer);
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


        if (strcmp(config.homeAssistantIntegration , "t") == 0)
        { // push data to homeassisant
          Serial.print("Send data to home Assisant as json");
          char status[4];
          char lowBattString[6];
          char timerWakeString[6];
          char mqttDiscoveryMessage[150];
          char mqttDiscoveryTopic[100];
          char stateTopic[50];
          char valueTemplate[30];
          char name[10];
          char device_class[10];
          uint8_t baseMac[6];
          // Get MAC address for WiFi station
          esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
          char baseMacChr[9] = {0};
          sprintf(baseMacChr, "%02X%02X%02X",  baseMac[3], baseMac[4], baseMac[5]);
          if (strcmp(config.homeAssistantDiscovery, "t") == 0){ // HomeAssistant MQTT discovery enabled
            //Create & Send discovery topic for each value : sensorString, lowBattString, batCharString, timerWakeString,
            Serial.println("Generate and send discovery message");
            for (int i = 0; i <= 3; i++)
            {
              sprintf(mqttDiscoveryTopic, "");
              sprintf(stateTopic, "");
              //topic is {HomeAssitantPrefix}/{sensortype}/{MACAdress}/config : 1 per device
              sprintf(stateTopic, "%s/%s/state", config.homeAssistantPrefix, baseMacChr);
              char name[10];
              char device_class[10];
              switch (i)
              {
                case STATUT:
                  sprintf(mqttDiscoveryTopic, "%s/binary_sensor/status%s/config", config.homeAssistantPrefix, baseMacChr);
                  sprintf(valueTemplate, "{{value_json.status}}");
                  sprintf(name, "Status");
                  sprintf(device_class, "door");
                  break;
                case LOW_BATT:
                  sprintf(mqttDiscoveryTopic, "%s/binary_sensor/lowbatt%s/config", config.homeAssistantPrefix, baseMacChr);
                  sprintf(valueTemplate, "{{value_json.low_batt}}");
                  sprintf(name, "Low Batt");
                  sprintf(device_class, "battery");
                  break;
                case BATT_V:
                  sprintf(mqttDiscoveryTopic, "%s/sensor/batt%s/config", config.homeAssistantPrefix, baseMacChr);
                  sprintf(valueTemplate, "{{value_json.batt_v}}");
                  sprintf(name, "Tension");
                  sprintf(device_class, "voltage");
                  break;
                case TIME_WAKE:
                  sprintf(mqttDiscoveryTopic, "%s/binary_sensor/waketime%s/config", config.homeAssistantPrefix, baseMacChr);
                  sprintf(valueTemplate, "{{value_json.timer_wake}}");
                  sprintf(name, "WakeTime");
                  sprintf(device_class, "power");
                  break;
              }
              //Short Version
              sprintf(mqttDiscoveryMessage, "{\"name\": \"%s\", \"stat_t\":\"%s\", \"val_tpl\":\"%s\", \"dev_cla\":\"%s\"}", name, stateTopic, valueTemplate, device_class);
              Serial.println("*******************");
              Serial.println("Publish discovery data: ");
              Serial.println(mqttDiscoveryMessage);
              Serial.println("to:");
              Serial.println(mqttDiscoveryTopic);
              Serial.println(client.publish_P(mqttDiscoveryTopic, mqttDiscoveryMessage, false));
              Serial.println("*******************");
            }
          }else{ // Direct sent to mqtt without discovery message
             sprintf(stateTopic, "");
              sprintf(stateTopic, "%s/%s/state", config.homeAssistantPrefix, baseMacChr);
          }
          //Create & Send message
          if (contactLatchClosed)
          {
            sprintf(status, "OFF");
          }
          else
          {
            sprintf(status, "ON");
          }
          if (lowBattery)
          {
            sprintf(lowBattString, "ON");
          }
          else
          {
            sprintf(lowBattString, "OFF");
          }
          if (timerWake)
          {
            sprintf(timerWakeString, "ON");
          }
          else
          {
            sprintf(timerWakeString, "OFF");
          }

          Serial.println("--------------------------");
          sprintf(mqttMessage, "{\"status\": \"%s\", \"batt_v\": \"%s\", \"low_batt\": \"%s\", \"timer_wake\": \"%s\"}",
                  status,
                  batCharString,
                  lowBattString,
                  timerWakeString);
          Serial.println("*******************");
          Serial.println("Publish state data: ");
          Serial.println(mqttMessage);
          Serial.println("to:");
          Serial.println(stateTopic);
          Serial.println("*******************");
          Serial.println("----------PUB STATE RESULT ----------------");
          Serial.println(client.publish_P(stateTopic, mqttMessage, false)); //OK
          delay(20);
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
