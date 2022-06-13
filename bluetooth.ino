
String macAddressString;
//bluetooth callbacks
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      bluetoothStatusStartTime = millis();
      bluetoothParamStartTime = millis();

    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      char rxBuffer[600];

      if (rxValue.length() > 0) {
        if (rxValue.length() > 599)
          return;
        for (int i = 0; i < rxValue.length(); i++) {
          rxBuffer[i] = rxValue[i];
          //Serial.print(rxBuffer[i]);
        }
        //Serial.println("");
        rxBuffer[rxValue.length()] = NULL;
        //**************************************
        char *keyWordParam = strstr(rxBuffer, "#param");
        if (keyWordParam != NULL) {
          sendParam = true;
          Serial.println("param update");
        }
        //**************************************
        char *keyWordWiFi = strstr(rxBuffer, "#wifi");
        if (keyWordWiFi != NULL) { //found wifi credentials
          const char delimiter[] = ",";
          char parsedStrings[3][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 3; i++) {
            token =  strtok(NULL, delimiter);
            if (token == NULL && i == 2) { //means no password
              strcpy(parsedStrings[2], "");
              break;
            }
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          WiFi.disconnect();
          IPAddress ipBlank(0, 0, 0, 0);
          WiFi.config(ipBlank, ipBlank, ipBlank, ipBlank, ipBlank);
          delay(1000);
          WiFi.begin(parsedStrings[1], parsedStrings[2]);

          strlcpy(config.ssid,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.ssid));
          strlcpy(config.pw,                  // <- destination
                  parsedStrings[2],  // <- source
                  sizeof(config.pw));
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordtout = strstr(rxBuffer, "#tout");
        if (keyWordtout != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][10];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }

          int convertedToInt = atoi(parsedStrings[1]);
          if (convertedToInt > 60)
            convertedToInt = 60;
          if (convertedToInt < 1)
            convertedToInt = 1;
          config.wifiTimeout = convertedToInt * 1000;
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordname = strstr(rxBuffer, "#name");
        if (keyWordname != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          strlcpy(config.trigName,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.trigName));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordsel = strstr(rxBuffer, "#sel");
        if (keyWordsel != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][20];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          Serial.println(parsedStrings[1]);
          if (strcmp(parsedStrings[1], "Contact Close") == 0) {
            strlcpy(config.trigSelect,                  // <- destination
                    "Close",  // <- source
                    sizeof(config.trigSelect));         // <- destination's capacity
            saveConfiguration(filename, config);
          }
          if (strcmp(parsedStrings[1], "Contact Open") == 0) {
            strlcpy(config.trigSelect,                  // <- destination
                    "Open",  // <- source
                    sizeof(config.trigSelect));         // <- destination's capacity
            saveConfiguration(filename, config);
          }
          if (strcmp(parsedStrings[1], "Open and Close") == 0) {
            strlcpy(config.trigSelect,                  // <- destination
                    "Both",  // <- source
                    sizeof(config.trigSelect));         // <- destination's capacity
            saveConfiguration(filename, config);
          }
          sendParam = true;
        }
        //**************************************
        char *keyWordope = strstr(rxBuffer, "#ope");
        if (keyWordope != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          strlcpy(config.triggerOpensMessage,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.triggerOpensMessage));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordclo = strstr(rxBuffer, "#clo");
        if (keyWordclo != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          strlcpy(config.triggerClosesMessage,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.triggerClosesMessage));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordwak = strstr(rxBuffer, "#wak");
        if (keyWordwak != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          strlcpy(config.buttonMessage,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.buttonMessage));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordtim = strstr(rxBuffer, "#tim");
        if (keyWordtim != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][5];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }

          int convertedToInt = atoi(parsedStrings[1]);
          if (convertedToInt > 255)
            convertedToInt = 255;
          if (convertedToInt < 1)
            convertedToInt = 1;
          config.timerCountDown = convertedToInt;
          updateRTC = true;
          updateRTCtime = true;
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordtse = strstr(rxBuffer, "#tse");
        if (keyWordtse != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][30];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          Serial.println(parsedStrings[1]);
          if (strcmp(parsedStrings[1], "Nothing") == 0) {
            Serial.println(parsedStrings[1]);
            strlcpy(config.timerSelect,                  // <- destination
                    "Nothing",  // <- source
                    sizeof(config.timerSelect));         // <- destination's capacity
            saveConfiguration(filename, config);
          }
          if (strcmp(parsedStrings[1], "Contact Still Closed") == 0) {
            Serial.println(parsedStrings[1]);
            strlcpy(config.timerSelect,                  // <- destination
                    "Closed",  // <- source
                    sizeof(config.timerSelect));         // <- destination's capacity
            saveConfiguration(filename, config);
          }
          if (strcmp(parsedStrings[1], "Contact Still Open") == 0) {
            Serial.println(parsedStrings[1]);
            strlcpy(config.timerSelect,                  // <- destination
                    "Open",  // <- source
                    sizeof(config.timerSelect));         // <- destination's capacity
            saveConfiguration(filename, config);
          }
          if (strcmp(parsedStrings[1], "Either Contact") == 0) {
            Serial.println(parsedStrings[1]);
            strlcpy(config.timerSelect,                  // <- destination
                    "Either",  // <- source
                    sizeof(config.timerSelect));         // <- destination's capacity
            saveConfiguration(filename, config);
          }
          sendParam = true;
        }
        //**************************************
        char *keyWordtso = strstr(rxBuffer, "#tso");
        if (keyWordtso != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          strlcpy(config.StillOpenMessage,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.StillOpenMessage));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordtsc = strstr(rxBuffer, "#tsc");
        if (keyWordtsc != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          strlcpy(config.StillClosedMessage,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.StillClosedMessage));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordlob = strstr(rxBuffer, "#lob");
        if (keyWordlob != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][5];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }

          float convertedToFloat = atof(parsedStrings[1]);
          if (convertedToFloat > 5.0)
            convertedToFloat = 5.0;
          if (convertedToFloat < 0.0)
            convertedToFloat = 0.0;
          config.batteryThreshold = convertedToFloat;
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordpov = strstr(rxBuffer, "#pov");
        if (keyWordpov != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[3][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 3; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          strlcpy(config.pushUserKey,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.pushUserKey));         // <- destination's capacity
          strlcpy(config.pushAPIKey,                  // <- destination
                  parsedStrings[2],  // <- source
                  sizeof(config.pushAPIKey));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordpoe = strstr(rxBuffer, "#poe");
        if (keyWordpoe != NULL) {
          strlcpy(config.pushOverEnable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.pushOverEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        char *keyWordpod = strstr(rxBuffer, "#pod");
        if (keyWordpod != NULL) {
          strlcpy(config.pushOverEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.pushOverEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordpse = strstr(rxBuffer, "#pse");
        if (keyWordpse != NULL) {
          strlcpy(config.pushSaferEnable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.pushSaferEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        char *keyWordpsd = strstr(rxBuffer, "#psd");
        if (keyWordpsd != NULL) {
          strlcpy(config.pushSaferEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.pushSaferEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordude = strstr(rxBuffer, "#ude");
        if (keyWordude != NULL) {
          strlcpy(config.udpEnable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.udpEnable));         // <- destination's capacity
          strlcpy(config.tcpEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.tcpEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordudptcpDis = strstr(rxBuffer, "#udd");
        if (keyWordudptcpDis != NULL) {
          Serial.println("disabling udp and tcp");
          strlcpy(config.udpEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.udpEnable));         // <- destination's capacity
          strlcpy(config.tcpEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.tcpEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordtce = strstr(rxBuffer, "#tce");
        if (keyWordtce != NULL) {
          strlcpy(config.tcpEnable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.tcpEnable));         // <- destination's capacity
          strlcpy(config.udpEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.udpEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordife = strstr(rxBuffer, "#ife");
        if (keyWordife != NULL) {
          strlcpy(config.iftttEnable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.iftttEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        char *keyWordifd = strstr(rxBuffer, "#ifd");
        if (keyWordifd != NULL) {
          strlcpy(config.iftttEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.iftttEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordpsk = strstr(rxBuffer, "#psk");
        if (keyWordpsk != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          strlcpy(config.pushSaferKey,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.pushSaferKey));         // <- destination's capacity

          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordifk = strstr(rxBuffer, "#ifk");
        if (keyWordifk != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          strlcpy(config.iftttMakerKey,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.iftttMakerKey));         // <- destination's capacity

          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordudp = strstr(rxBuffer, "#udp");
        if (keyWordudp != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[12][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 12; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }

          strlcpy(config.udpSSID,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.udpSSID));         // <- destination's capacity
          strlcpy(config.udpPW,                  // <- destination
                  parsedStrings[2],  // <- source
                  sizeof(config.udpPW));         // <- destination's capacity
          strlcpy(config.udpStaticIP,                  // <- destination
                  parsedStrings[3],  // <- source
                  sizeof(config.udpStaticIP));         // <- destination's capacity
          strlcpy(config.udpTargetIP,                  // <- destination
                  parsedStrings[4],  // <- source
                  sizeof(config.udpTargetIP));         // <- destination's capacity
          strlcpy(config.udpGatewayAddress,                  // <- destination
                  parsedStrings[5],  // <- source
                  sizeof(config.udpGatewayAddress));         // <- destination's capacity
          strlcpy(config.udpSubnetAddress,                  // <- destination
                  parsedStrings[6],  // <- source
                  sizeof(config.udpSubnetAddress));         // <- destination's capacity
          strlcpy(config.udpPrimaryDNSAddress,                  // <- destination
                  parsedStrings[7],  // <- source
                  sizeof(config.udpPrimaryDNSAddress));         // <- destination's capacity
          strlcpy(config.udpSecondaryDNSAddress,                  // <- destination
                  parsedStrings[8],  // <- source
                  sizeof(config.udpSecondaryDNSAddress));         // <- destination's capacity
          config.udpPort = atoi(parsedStrings[9]);
          int blastCountInt = atoi(parsedStrings[10]);
          if (blastCountInt < 1)
            blastCountInt = 1;
          if (blastCountInt > 100)
            blastCountInt = 100;
          config.udpBlastCount = blastCountInt;
          int blastTimeInt = atoi(parsedStrings[11]);
          if (blastTimeInt < 1)
            blastTimeInt = 1;
          if (blastTimeInt > 100)
            blastTimeInt = 100;
          config.udptimeBetweenBlasts = blastTimeInt;

          getFourNumbersForIP(config.udpStaticIP);
          Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
          IPAddress ipudp(oneIP, twoIP, threeIP, fourIP);
          getFourNumbersForIP(config.udpGatewayAddress);
          Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
          IPAddress gatewayudp(oneIP, twoIP, threeIP, fourIP);
          getFourNumbersForIP(config.udpSubnetAddress);
          Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
          IPAddress subnetudp(oneIP, twoIP, threeIP, fourIP);
          getFourNumbersForIP(config.udpPrimaryDNSAddress);
          Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
          IPAddress primaryDNSudp(oneIP, twoIP, threeIP, fourIP);
          getFourNumbersForIP(config.udpSecondaryDNSAddress);
          Serial.printf("%i.%i.%i.%i\n", oneIP, twoIP, threeIP, fourIP);
          IPAddress secondaryDNSudp(oneIP, twoIP, threeIP, fourIP);
          WiFi.disconnect();
          delay(1000);
          WiFi.config(ipudp, gatewayudp, subnetudp, primaryDNSudp, secondaryDNSudp);
          WiFi.begin(config.udpSSID, config.udpPW);

          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordrtcm = strstr(rxBuffer, "#rtcme");
        if (keyWordrtcm != NULL) {
          strlcpy(config.rtcCountdownMinute,                  // <- destination
                  "t",  // <- source
                  sizeof(config.rtcCountdownMinute));         // <- destination's capacity
          updateRTC = true;
          updateRTCtime = true;
          saveConfiguration(filename, config);
          sendParam = true;
        }

        char *keyWordrtcmd = strstr(rxBuffer, "#rtcmd");
        if (keyWordrtcmd != NULL) {
          strlcpy(config.rtcCountdownMinute,                  // <- destination
                  "f",  // <- source
                  sizeof(config.rtcCountdownMinute));         // <- destination's capacity
          updateRTC = true;
          updateRTCtime = true;
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordmqen = strstr(rxBuffer, "#mqen");
        if (keyWordmqen != NULL) {
          strlcpy(config.mqttEnable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.mqttEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }

        char *keyWordmqdi = strstr(rxBuffer, "#mqdi");
        if (keyWordmqdi != NULL) {
          strlcpy(config.mqttEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.mqttEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordmqp = strstr(rxBuffer, "#mqset");
        if (keyWordmqp != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[7][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 6; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          config.mqttPort = atoi(parsedStrings[1]);
          strlcpy(config.mqttServer,                  // <- destination
                  parsedStrings[2],  // <- source
                  sizeof(config.mqttServer));         // <- destination's capacity
          strlcpy(config.mqttTopic,                  // <- destination
                  parsedStrings[3],  // <- source
                  sizeof(config.mqttTopic));         // <- destination's capacity
          strlcpy(config.mqttPW,                  // <- destination
                  parsedStrings[4],  // <- source
                  sizeof(config.mqttPW));         // <- destination's capacity
          strlcpy(config.mqttUser,                  // <- destination
                  parsedStrings[5],  // <- source
                  sizeof(config.mqttUser));         // <- destination's capacity

          saveConfiguration(filename, config);
          sendParam = true;
        }

        //**************************************
        char *keyWordmqsslke = strstr(rxBuffer, "#mqsslke");
        if (keyWordmqsslke != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[3][600];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 3; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          if(strcmp(parsedStrings[1], "w") == 0) {
            strlcpy(mqttSSLKey,                  // <- destination
                  parsedStrings[2],  // <- source
                  sizeof(mqttSSLKey));         // <- destination's capacity
          } else {
            //add some logic in here to resend all params when done
            strlcat(mqttSSLKey,                  // <- destination
                  parsedStrings[2],  // <- source
                  sizeof(mqttSSLKey));         // <- destination's capacity
          }

          saveKeyFile(mqttKeyFile, mqttSSLKey);
          //sendParam = true;
        }
        
        //**************************************
        char *keyWordmqsslce = strstr(rxBuffer, "#mqsslce");
        if (keyWordmqsslce != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[3][600];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 3; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          if(strcmp(parsedStrings[1], "w") == 0) {
            strlcpy(mqttSSLCert,                  // <- destination
                  parsedStrings[2],  // <- source
                  sizeof(mqttSSLCert));
          } else {
            strlcat(mqttSSLCert,                  // <- destination
                  parsedStrings[2],  // <- source
                  sizeof(mqttSSLCert));         // <- destination's capacity
          }
          
          saveKeyFile(mqttCertFile, mqttSSLCert);
          //sendParam = true;
        }
        
        //**************************************
        char *keyWordmqsslca = strstr(rxBuffer, "#mqsslca");
        if (keyWordmqsslca != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[3][600];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 3; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          if(strcmp(parsedStrings[1], "w") == 0) {
            strlcpy(mqttSSLCA,                  // <- destination
                  parsedStrings[2],  // <- source
                  sizeof(mqttSSLCA));         // <- destination's capacity
          } else {
            strlcat(mqttSSLCA,                  // <- destination
                  parsedStrings[2],  // <- source
                  sizeof(mqttSSLCA));         // <- destination's capacity
          }
          
          saveKeyFile(mqttCAFile, mqttSSLCA);
          //sendParam = true;
        }
        
        //**************************************
        char *keyWordmqsen = strstr(rxBuffer, "#mqsen");
        if (keyWordmqsen != NULL) {
          strlcpy(config.mqttSecureEnable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.mqttSecureEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        char *keyWordmqsdi = strstr(rxBuffer, "#mqsdi");
        if (keyWordmqsdi != NULL) {
          strlcpy(config.mqttSecureEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.mqttSecureEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordsipen = strstr(rxBuffer, "#sipen");
        if (keyWordsipen != NULL) {
          strlcpy(config.staticIPenable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.staticIPenable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        char *keyWordsipdi = strstr(rxBuffer, "#sipdi");
        if (keyWordsipdi != NULL) {
          strlcpy(config.staticIPenable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.staticIPenable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordsipset = strstr(rxBuffer, "#sipset");
        if (keyWordsipset != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[6][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 6; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          strlcpy(config.staticIP,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.staticIP));         // <- destination's capacity
          strlcpy(config.staticGatewayAddress,                  // <- destination
                  parsedStrings[2],  // <- source
                  sizeof(config.staticGatewayAddress));         // <- destination's capacity
          strlcpy(config.staticSubnetAddress,                  // <- destination
                  parsedStrings[3],  // <- source
                  sizeof(config.staticSubnetAddress));         // <- destination's capacity
          strlcpy(config.staticPrimaryDNSAddress,                  // <- destination
                  parsedStrings[4],  // <- source
                  sizeof(config.staticPrimaryDNSAddress));         // <- destination's capacity
          strlcpy(config.staticSecondaryDNSAddress,                  // <- destination
                  parsedStrings[5],  // <- source
                  sizeof(config.staticSecondaryDNSAddress));         // <- destination's capacity

          saveConfiguration(filename, config);
          sendParam = true;
          WiFi.disconnect();
          delay(1000);

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

          WiFi.begin(config.ssid, config.pw);
          //          delay(2000);
          //          connectWiFi();
        }
        //**************************************
        char *keyWordhighSpdOFF = strstr(rxBuffer, "#highSpdOFF");
        if (keyWordhighSpdOFF != NULL) {
          strlcpy(config.highSpeed,                  // <- destination
                  "f",  // <- source
                  sizeof(config.highSpeed));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordhighSpdON = strstr(rxBuffer, "#highSpdON");
        if (keyWordhighSpdON != NULL) {
          strlcpy(config.highSpeed,                  // <- destination
                  "t",  // <- source
                  sizeof(config.highSpeed));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordclockEnable = strstr(rxBuffer, "#clken");
        if (keyWordclockEnable != NULL) {
          strlcpy(config.clkEnable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.clkEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordclockDisable = strstr(rxBuffer, "#clkdi");
        if (keyWordclockDisable != NULL) {
          strlcpy(config.clkEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.clkEnable));         // <- destination's capacity
          strlcpy(config.clkAlarmEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.clkAlarmEnable));         // <- destination's capacity

          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordTimeZone = strstr(rxBuffer, "#clkzn");
        if (keyWordTimeZone != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][5];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }

          int convertedToInt = atoi(parsedStrings[1]);
          if (convertedToInt > 14)
            convertedToInt = 14;
          if (convertedToInt < -12)
            convertedToInt = -12;
          config.clkTimeZone = convertedToInt;
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordclockAppendEnable = strstr(rxBuffer, "#clkappen");
        if (keyWordclockAppendEnable != NULL) {
          strlcpy(config.clkAppendEnable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.clkAppendEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordclockAppendDisable = strstr(rxBuffer, "#clkappdi");
        if (keyWordclockAppendDisable != NULL) {
          strlcpy(config.clkAppendEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.clkAppendEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordclockAlarmEnable = strstr(rxBuffer, "#clkalmen");
        if (keyWordclockAlarmEnable != NULL) {
          strlcpy(config.clkAlarmEnable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.clkAlarmEnable));         // <- destination's capacity
          updateRTC = true;
          updateRTCtime = false;
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordclockAlarmDisable = strstr(rxBuffer, "#clkalmdi");
        if (keyWordclockAlarmDisable != NULL) {
          strlcpy(config.clkAlarmEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.clkAlarmEnable));         // <- destination's capacity
          updateRTC = true;
          updateRTCtime = false;
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordClkAlarmSetting = strstr(rxBuffer, "#clkalmtim");
        if (keyWordClkAlarmSetting != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[3][5];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 3; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }

          int convertedToIntHour = atoi(parsedStrings[1]);
          int convertedToIntMinute = atoi(parsedStrings[2]);
          if (convertedToIntHour > 23)
            convertedToIntHour = 23;
          if (convertedToIntHour < 0)
            convertedToIntHour = 0;
          if (convertedToIntMinute > 59)
            convertedToIntMinute = 59;
          if (convertedToIntMinute < 0)
            convertedToIntMinute = 0;

          config.clkAlarmHour = convertedToIntHour;
          config.clkAlarmMinute = convertedToIntMinute;
          updateRTC = true;
          updateRTCtime = false;
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordclockNPTenable = strstr(rxBuffer, "#clkNTPen");
        if (keyWordclockNPTenable != NULL) {
          strlcpy(config.clkUpdateNPTenable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.clkUpdateNPTenable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordclockNPTdisable = strstr(rxBuffer, "#clkNTPdi");
        if (keyWordclockNPTdisable != NULL) {
          strlcpy(config.clkUpdateNPTenable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.clkUpdateNPTenable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordAlarmMessage = strstr(rxBuffer, "#clkalarMsg");
        if (keyWordAlarmMessage != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          strlcpy(config.clkAlarmMessage,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.clkAlarmMessage));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordclockAppendAlmEN = strstr(rxBuffer, "#clkalmappen");
        if (keyWordclockAppendAlmEN != NULL) {
          strlcpy(config.clkAppendAlmEnable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.clkAppendAlmEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordclockAppendAlmDI = strstr(rxBuffer, "#clkalmappdi");
        if (keyWordclockAppendAlmDI != NULL) {
          strlcpy(config.clkAppendAlmEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.clkAppendAlmEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordTelegramEN = strstr(rxBuffer, "#teleEN");
        if (keyWordTelegramEN != NULL) {
          strlcpy(config.telegramEnable,                  // <- destination
                  "t",  // <- source
                  sizeof(config.telegramEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordTelegramDI = strstr(rxBuffer, "#teleDI");
        if (keyWordTelegramDI != NULL) {
          strlcpy(config.telegramEnable,                  // <- destination
                  "f",  // <- source
                  sizeof(config.telegramEnable));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordTelegramCred = strstr(rxBuffer, "#telcrd");
        if (keyWordTelegramCred != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[3][50];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 3; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }
          strlcpy(config.telegramBOT,                  // <- destination
                  parsedStrings[1],  // <- source
                  sizeof(config.telegramBOT));         // <- destination's capacity
          strlcpy(config.telegramCHAT,                  // <- destination
                  parsedStrings[2],  // <- source
                  sizeof(config.telegramCHAT));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }

        //**************************************
        char *keyWordNPTset = strstr(rxBuffer, "#clkNTPset");
        if (keyWordNPTset != NULL) {
          getNTPtime();
        }
        //**************************************
        char *keyWordRSSIen = strstr(rxBuffer, "#rssien");
        if (keyWordRSSIen != NULL) {
          strlcpy(config.appendRSSI,                  // <- destination
                  "t",  // <- source
                  sizeof(config.appendRSSI));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordRSSIdi = strstr(rxBuffer, "#rssidi");
        if (keyWordRSSIdi != NULL) {
          strlcpy(config.appendRSSI,                  // <- destination
                  "f",  // <- source
                  sizeof(config.appendRSSI));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordMissionEn = strstr(rxBuffer, "#missionen");
        if (keyWordMissionEn != NULL) {
          strlcpy(config.checkAgain,                  // <- destination
                  "t",  // <- source
                  sizeof(config.checkAgain));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordMissionDi = strstr(rxBuffer, "#missiondi");
        if (keyWordMissionDi != NULL) {
          strlcpy(config.checkAgain,                  // <- destination
                  "f",  // <- source
                  sizeof(config.checkAgain));         // <- destination's capacity
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordMissionTime = strstr(rxBuffer, "#tmiss");
        if (keyWordMissionTime != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][5];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }

          int convertedToInt = atoi(parsedStrings[1]);
          if (convertedToInt > 60)
            convertedToInt = 60;
          if (convertedToInt < 1)
            convertedToInt = 1;
          config.secondsAfterToCheckAgain = convertedToInt;
          saveConfiguration(filename, config);
          sendParam = true;
        }




        //**************************************
        char *keyWordbof = strstr(rxBuffer, "#bof");
        if (keyWordbof != NULL) {
          const char delimiter[] = ",";
          char parsedStrings[2][5];
          char *token =  strtok(rxBuffer, delimiter);
          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one
          for (int i = 1; i < 2; i++) {
            token =  strtok(NULL, delimiter);
            strncpy(parsedStrings[i], token, sizeof(parsedStrings[i]));
          }

          float convertedToFloat = atof(parsedStrings[1]);
          if (convertedToFloat > 5.0)
            convertedToFloat = 5.0;
          if (convertedToFloat < -5.0)
            convertedToFloat = -5.0;
          config.batteryOffset = convertedToFloat;
          saveConfiguration(filename, config);
          sendParam = true;
        }
        //**************************************
        char *keyWordpot = strstr(rxBuffer, "#pot");
        if (keyWordpot != NULL) {
          pushOver();
        }
        //**************************************
        char *keyWordkill = strstr(rxBuffer, "#kill");
        if (keyWordkill != NULL) {
          while (1) {
            killPower();
          }
        }
        //**************************************
        char *keyWordotaStart = strstr(rxBuffer, "#otaStart");
        if (keyWordotaStart != NULL) {
          setupOTA();
        }
      }
    }
};

void initBluetooth() {
  // Create the BLE Device
  macAddressString = WiFi.macAddress();
  Serial.println("Starting Config Mode");
  BLEDevice::init("trigBoard");
  BLEDevice::setMTU(517); // BT has a 5 byte header. This gives us 512 usable
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pServer->getAdvertising()->addServiceUUID(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_TX,
                        BLECharacteristic::PROPERTY_NOTIFY
                      );

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE
                                          );

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for bluetooth connection...");
}

void transmitData(char *prefix, char *dataTx) {
  // Max 512B plus null terminator
  char txString[513];
  //  Serial.print(dataTx);
  //  Serial.print(" ");
  
  sprintf(txString, "%s,%s", prefix, dataTx);
  // Serial.println(txString);
  // Serial.println(strlen(txString));
  pTxCharacteristic->setValue(txString);
  pTxCharacteristic->notify();
}

void packetizeBluetoothMessage(char *header, char *suffix, char *dataToSend) {
  Serial.println("Sending bluetooth packet...");
  size_t dataSize = strlen(dataToSend);
  size_t fullHeaderLen = strlen(header) + strlen(suffix) + 1; // add a slot for our new comma
  size_t totalTransmission = fullHeaderLen + dataSize; // leave only one null terminator
  char fullHeader[fullHeaderLen + 1]; // pad null terminator
  sprintf(fullHeader, "%s,%s", header, suffix);
  
  // Leave space for the extra comma in transmitData
  // We can move 511B here, plus 1B (comma) transmitData also creates
  if(totalTransmission <= 511) {  
    transmitData(fullHeader, dataToSend); 
    return;
  }
  // Max bluetooth transmission per packet is 512
  // Leave space for the extra comma in transmitData
  // Restrict our local string to 511B
  size_t bodyLength = (511 - fullHeaderLen); 
  size_t remainderLength = dataSize - bodyLength;
  char body[bodyLength + 1];  // pad null terminator
  char remainder[remainderLength + 1];  // pad null terminator
  int i;
  int r;
  for(i = 0; i < bodyLength; i++) {
    body[i] = dataToSend[i];
  }
  body[i] = '\0';
  for(r = 0; r < remainderLength; r++) {
    remainder[r] = dataToSend[i];
    i++;
  }
  remainder[r] = '\0';
  //Serial.print("Total length: ");
  //Serial.print(strlen(fullHeader) + strlen(body));
  //Serial.print("B\n");
  transmitData(fullHeader, body);
  delay(25);
  packetizeBluetoothMessage(header, "a", remainder);
  return;
}

void serviceBluetooth() {
  if (millis() - bluetoothFlasherTime > 200) {
    digitalWrite(LEDpin, !digitalRead(LEDpin));
    bluetoothFlasherTime = millis();
  }

  if (millis() - bluetoothParamStartTime > 1000 && deviceConnected && sendParam) {

    sendParam = false;
    transmitData("ssid", config.ssid);
    delay(25);
    transmitData("pw", config.pw);
    delay(25);
    char timeout[10];
    sprintf(timeout, "%i", config.wifiTimeout);
    transmitData("tout", timeout);
    delay(25);
    transmitData("name", config.trigName);
    delay(25);
    transmitData("sel", config.trigSelect);
    delay(25);
    transmitData("ope", config.triggerOpensMessage);
    delay(25);
    transmitData("clo", config.triggerClosesMessage);
    delay(25);
    transmitData("wak", config.buttonMessage);
    delay(25);
    char timeCountDown[10];
    sprintf(timeCountDown, "%i", config.timerCountDown);
    transmitData("tim", timeCountDown);
    delay(25);
    transmitData("tse", config.timerSelect);
    delay(25);
    transmitData("tso", config.StillOpenMessage);
    delay(25);
    transmitData("tsc", config.StillClosedMessage);
    delay(25);
    char floatValue[5];
    dtostrf(config.batteryThreshold, 1, 2, floatValue);//convert float to str
    transmitData("lob", floatValue);
    delay(25);
    char floatValuebof[5];
    dtostrf(config.batteryOffset, 1, 2, floatValuebof);//convert float to str
    transmitData("bof", floatValuebof);
    delay(25);
    transmitData("poe", config.pushOverEnable);
    delay(25);
    transmitData("pouser", config.pushUserKey);
    delay(25);
    transmitData("poapi", config.pushAPIKey);
    delay(25);
    transmitData("pse", config.pushSaferEnable);
    delay(25);
    transmitData("psk", config.pushSaferKey);
    delay(25);
    transmitData("ife", config.iftttEnable);
    delay(25);
    transmitData("ifk", config.iftttMakerKey);
    delay(25);
    transmitData("ude", config.udpEnable);
    delay(25);
    transmitData("tce", config.tcpEnable);
    delay(25);
    transmitData("udt", config.udpTargetIP);
    delay(25);
    transmitData("uds", config.udpStaticIP);
    delay(25);
    transmitData("udg", config.udpGatewayAddress);
    delay(25);
    transmitData("udb", config.udpSubnetAddress);
    delay(25);
    transmitData("uddns", config.udpPrimaryDNSAddress);
    delay(25);
    transmitData("uddnss", config.udpSecondaryDNSAddress);
    delay(25);
    transmitData("udsi", config.udpSSID);
    delay(25);
    transmitData("udpw", config.udpPW);
    delay(25);
    char udpPortCh[10];
    sprintf(udpPortCh, "%i", config.udpPort);
    transmitData("udport", udpPortCh);
    transmitData("rtcm", config.rtcCountdownMinute);
    delay(25);
    transmitData("mqse", config.mqttSecureEnable);
    delay(25);
    transmitData("mqe", config.mqttEnable);
    delay(25);
    char mqttPortCh[10];
    sprintf(mqttPortCh, "%i", config.mqttPort);
    transmitData("mqp", mqttPortCh);
    delay(25);
    transmitData("mqs", config.mqttServer);
    delay(25);
    transmitData("mqt", config.mqttTopic);
    delay(25);
    transmitData("mqsu", config.mqttUser);
    delay(25);
    transmitData("mqsp", config.mqttPW);
    delay(25);
    
    //------------------------------------------
    //break these into 512 byte chunks
    packetizeBluetoothMessage("mqsske", "w", mqttSSLKey);
    delay(25);
    packetizeBluetoothMessage("mqssce", "w", mqttSSLCert);
    delay(25);
    packetizeBluetoothMessage("mqssca", "w", mqttSSLCA);
    delay(25);
    //------------------------------------------
    
    transmitData("sipen", config.staticIPenable);
    delay(25);
    transmitData("sip", config.staticIP);
    delay(25);
    transmitData("gip", config.staticGatewayAddress);
    delay(25);
    transmitData("suip", config.staticSubnetAddress);
    delay(25);
    transmitData("pdnsip", config.staticPrimaryDNSAddress);
    delay(25);
    transmitData("sdnsip", config.staticSecondaryDNSAddress);
    delay(25);
    transmitData("highSpd", config.highSpeed);
    delay(25);
    char blastCountChar[5];
    sprintf(blastCountChar, "%i", config.udpBlastCount);
    transmitData("udpBla", blastCountChar);
    delay(25);
    char blastTimeChar[5];
    sprintf(blastTimeChar, "%i", config.udptimeBetweenBlasts);
    transmitData("udpTim", blastTimeChar);
    delay(25);
    transmitData("clkEnable", config.clkEnable);
    delay(25);
    char clkTimeZoneChar[5];
    sprintf(clkTimeZoneChar, "%i", config.clkTimeZone);
    transmitData("clkTimeZone", clkTimeZoneChar);
    delay(25);
    transmitData("clkAppendEnable", config.clkAppendEnable);
    delay(25);
    transmitData("clkAlarmEnable", config.clkAlarmEnable);
    delay(25);
    char clkAlarmHourChar[5];
    sprintf(clkAlarmHourChar, "%i", config.clkAlarmHour);
    transmitData("clkAlarmHour", clkAlarmHourChar);
    delay(25);
    char clkAlarmMinuteChar[5];
    if (config.clkAlarmMinute > 9)
      sprintf(clkAlarmMinuteChar, "%i", config.clkAlarmMinute);
    else
      sprintf(clkAlarmMinuteChar, "0%i", config.clkAlarmMinute);
    transmitData("clkAlarmMinute", clkAlarmMinuteChar);
    delay(25);
    transmitData("clkUpdateNPTenable", config.clkUpdateNPTenable);
    delay(25);
    transmitData("clkAlarmMessage", config.clkAlarmMessage);
    delay(25);
    transmitData("clkAppendAlmEnable", config.clkAppendAlmEnable);
    delay(25);
    transmitData("telegramEnable", config.telegramEnable);
    delay(25);
    transmitData("telegramBOT", config.telegramBOT);
    delay(25);
    transmitData("telegramCHAT", config.telegramCHAT);
    delay(25);
    transmitData("appendRSSI", config.appendRSSI);
    delay(25);
    transmitData("missionEnable", config.checkAgain);
    delay(25);
    char missionCriticalTimeChar[5];
    sprintf(missionCriticalTimeChar, "%i", config.secondsAfterToCheckAgain);
    transmitData("missionTimeafter", missionCriticalTimeChar);

    bluetoothParamStartTime = millis();
  }
  if (millis() - bluetoothStatusStartTime > 200 && deviceConnected) {//send status

    char txString[250] = "stat,";
    if (WiFi.status() == WL_CONNECTED)
      strcat(txString, "co,");
    else {
      strcat(txString, "di,");
      OTAsetup = false;
    }
    getBattery();
    strcat(txString, batCharString);
    if (digitalRead(contactStatusPin))
      strcat(txString, ",op");
    else
      strcat(txString, ",cl");
    if (!digitalRead(wakeButtonPin))
      strcat(txString, ",bt,");
    else
      strcat(txString, ",bf,");
    char macAddressChar[20];
    macAddressString.toCharArray(macAddressChar, sizeof(macAddressChar));

    strcat(txString, macAddressChar);
    strcat(txString, ",");
    strcat(txString, fwVersion);

    String ipAddressString = WiFi.localIP().toString();
    char ipAddressChar[20];
    ipAddressString.toCharArray(ipAddressChar, sizeof(ipAddressChar));

    strcat(txString, ",");
    strcat(txString, ipAddressChar);
    strcat(txString, ",");
    char ssidChar[50];
    WiFi.SSID().toCharArray(ssidChar, sizeof(ssidChar));
    getRSSI();
    strcat(txString, ssidChar);
    strcat(txString, "\n-");
    strcat(txString, rssiChar);
    strcat(txString, ",");
    rtcGetTime();
    strcat(txString, rtcTimeStamp);
    if (updateRTC) {
      rtcInit(config.timerCountDown, updateRTCtime);
      updateRTC = false;
    }
    pTxCharacteristic->setValue(txString);
    pTxCharacteristic->notify();

    bluetoothStatusStartTime = millis();
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");

    oldDeviceConnected = deviceConnected;
    while (1) {
      killPower();
    }
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }


}
