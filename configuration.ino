void loadConfiguration(const char *filename, Config &config) {

  Serial.println(F("checking config file..."));
  if (SPIFFS.begin(true)) {
    File file = SPIFFS.open(filename, "r");

    StaticJsonDocument<2000> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
      Serial.println(F("Failed to read file"));
    }

    // Copy values from the JsonDocument to the Config
    strlcpy(config.ssid,                  // <- destination
            doc["ssid"] | "your ssid",  // <- source
            sizeof(config.ssid));         // <- destination's capacity
    //Serial.println(config.ssid);

    strlcpy(config.pw,                  // <- destination
            doc["pw"] | "your pw",  // <- source
            sizeof(config.pw));         // <- destination's capacity

    config.wifiTimeout = doc["wifiTimeout"] | 5000;

    strlcpy(config.trigName,                  // <- destination
            doc["trigName"] | "trigBoard Name",  // <- source
            sizeof(config.trigName));         // <- destination's capacity

    strlcpy(config.trigSelect,                  // <- destination
            doc["trigSelect"] | "Both",  // <- source
            sizeof(config.trigSelect));         // <- destination's capacity

    strlcpy(config.triggerOpensMessage,                  // <- destination
            doc["triggerOpensMessage"] | "Contact has Opened",  // <- source
            sizeof(config.triggerOpensMessage));         // <- destination's capacity

    strlcpy(config.triggerClosesMessage,                  // <- destination
            doc["triggerClosesMessage"] | "Contact has Closed",  // <- source
            sizeof(config.triggerClosesMessage));         // <- destination's capacity

    strlcpy(config.buttonMessage,                  // <- destination
            doc["buttonMessage"] | "Button Was Pressed",  // <- source
            sizeof(config.buttonMessage));         // <- destination's capacity

    config.timerCountDown = doc["timerCountDown"] | 5;

    strlcpy(config.timerSelect,                  // <- destination
            doc["timerSelect"] | "Either",  // <- source
            sizeof(config.timerSelect));         // <- destination's capacity

    strlcpy(config.StillOpenMessage,                  // <- destination
            doc["StillOpenMessage"] | "Contact Still Open",  // <- source
            sizeof(config.StillOpenMessage));         // <- destination's capacity

    strlcpy(config.StillClosedMessage,                  // <- destination
            doc["StillClosedMessage"] | "Contact Still Closed",  // <- source
            sizeof(config.StillClosedMessage));         // <- destination's capacity


    config.batteryThreshold = doc["batteryThreshold"] | 2500;
    config.batteryThreshold = config.batteryThreshold / 1000; //convert to volts

    config.batteryOffset = doc["batteryOffset"] | 0;
    config.batteryOffset = config.batteryOffset / 1000; //convert to volts

    strlcpy(config.pushUserKey,                  // <- destination
            doc["pushUserKey"] | "Your User Key",  // <- source
            sizeof(config.pushUserKey));         // <- destination's capacity
    strlcpy(config.pushAPIKey,                  // <- destination
            doc["pushAPIKey"] | "Your API Key",  // <- source
            sizeof(config.pushAPIKey));         // <- destination's capacity
    strlcpy(config.pushOverEnable,                  // <- destination
            doc["pushOverEnable"] | "f",  // <- source
            sizeof(config.pushOverEnable));         // <- destination's capacity
    strlcpy(config.pushSaferEnable,                  // <- destination
            doc["pushSaferEnable"] | "f",  // <- source
            sizeof(config.pushSaferEnable));         // <- destination's capacity
    strlcpy(config.iftttEnable,                  // <- destination
            doc["iftttEnable"] | "f",  // <- source
            sizeof(config.iftttEnable));         // <- destination's capacity
    strlcpy(config.udpEnable,                  // <- destination
            doc["udpEnable"] | "t",  // <- source
            sizeof(config.udpEnable));         // <- destination's capacity
    strlcpy(config.tcpEnable,                  // <- destination
            doc["tcpEnable"] | "f",  // <- source
            sizeof(config.tcpEnable));         // <- destination's capacity
    config.udpPort = doc["udpPort"] | 1234;
    strlcpy(config.pushSaferKey,                  // <- destination
            doc["pushSaferKey"] | "your pushSafer key",  // <- source
            sizeof(config.pushSaferKey));         // <- destination's capacity
    strlcpy(config.iftttMakerKey,                  // <- destination
            doc["iftttMakerKey"] | "your maker key",  // <- source
            sizeof(config.iftttMakerKey));         // <- destination's capacity
    strlcpy(config.udpStaticIP,                  // <- destination
            doc["udpStaticIP"] | "192.168.4.2",  // <- source
            sizeof(config.udpStaticIP));         // <- destination's capacity
    strlcpy(config.udpTargetIP,                  // <- destination
            doc["udpTargetIP"] | "192.168.4.1",  // <- source
            sizeof(config.udpTargetIP));         // <- destination's capacity
    strlcpy(config.udpGatewayAddress,                  // <- destination
            doc["udpGatewayAddress"] | "192.168.4.1",  // <- source
            sizeof(config.udpGatewayAddress));         // <- destination's capacity
    strlcpy(config.udpSubnetAddress,                  // <- destination
            doc["udpSubnetAddress"] | "255.255.0.0",  // <- source
            sizeof(config.udpSubnetAddress));         // <- destination's capacity
    strlcpy(config.udpPrimaryDNSAddress,                  // <- destination
            doc["udpPrimaryDNSAddress"] | "8.8.8.8",  // <- source
            sizeof(config.udpPrimaryDNSAddress));         // <- destination's capacity
    strlcpy(config.udpSecondaryDNSAddress,                  // <- destination
            doc["udpSecondaryDNSAddress"] | "8.8.4.4",  // <- source
            sizeof(config.udpSecondaryDNSAddress));         // <- destination's capacity
    strlcpy(config.udpSSID,                  // <- destination
            doc["udpSSID"] | "your_udp_SSID",  // <- source
            sizeof(config.udpSSID));         // <- destination's capacity
    strlcpy(config.udpPW,                  // <- destination
            doc["udpPW"] | "your_udp_PW",  // <- source
            sizeof(config.udpPW));         // <- destination's capacity
    strlcpy(config.rtcCountdownMinute,                  // <- destination
            doc["rtcCountdownMinute"] | "f",  // <- source
            sizeof(config.rtcCountdownMinute));         // <- destination's capacity
    strlcpy(config.mqttEnable,                  // <- destination
            doc["mqttEnable"] | "f",  // <- source
            sizeof(config.mqttEnable));         // <- destination's capacity
    config.mqttPort = doc["mqttPort"] | 1883;
    strlcpy(config.mqttServer,                  // <- destination
            doc["mqttServer"] | "192.168.1.117",  // <- source
            sizeof(config.mqttServer));         // <- destination's capacity
    strlcpy(config.mqttTopic,                  // <- destination
            doc["mqttTopic"] | "trigBoardTopic",  // <- source
            sizeof(config.mqttTopic));         // <- destination's capacity
    strlcpy(config.mqttSecureEnable,                  // <- destination
            doc["mqttSecureEnable"] | "t",  // <- source
            sizeof(config.mqttSecureEnable));         // <- destination's capacity
    strlcpy(config.mqttUser,                  // <- destination
            doc["mqttUser"] | "User Name",  // <- source
            sizeof(config.mqttUser));         // <- destination's capacity
    strlcpy(config.mqttPW,                  // <- destination
            doc["mqttPW"] | "password",  // <- source
            sizeof(config.mqttPW));         // <- destination's capacity
    strlcpy(config.staticIPenable,                  // <- destination
            doc["staticIPenable"] | "f",  // <- source
            sizeof(config.staticIPenable));         // <- destination's capacity
    strlcpy(config.staticIP,                  // <- destination
            doc["staticIP"] | "192.168.1.200",  // <- source
            sizeof(config.staticIP));         // <- destination's capacity
    strlcpy(config.staticGatewayAddress,                  // <- destination
            doc["staticGatewayAddress"] | "192.168.1.1",  // <- source
            sizeof(config.staticGatewayAddress));         // <- destination's capacity
    strlcpy(config.staticSubnetAddress,                  // <- destination
            doc["staticSubnetAddress"] | "255.255.0.0",  // <- source
            sizeof(config.staticSubnetAddress));         // <- destination's capacity
    strlcpy(config.staticPrimaryDNSAddress,                  // <- destination
            doc["staticPrimaryDNSAddress"] | "8.8.8.8",  // <- source
            sizeof(config.staticPrimaryDNSAddress));         // <- destination's capacity
    strlcpy(config.staticSecondaryDNSAddress,                  // <- destination
            doc["staticSecondaryDNSAddress"] | "8.8.4.4",  // <- source
            sizeof(config.staticSecondaryDNSAddress));         // <- destination's capacity

    strlcpy(config.highSpeed,                  // <- destination
            doc["highSpeed"] | "f",  // <- source
            sizeof(config.highSpeed));         // <- destination's capacity

    config.udpBlastCount = doc["udpBlastCount"] | 10;
    config.udptimeBetweenBlasts = doc["udptimeBetweenBlasts"] | 10;

    file.close();
  } else {
    Serial.println(F("SPIFFS fault"));
    killPower();
  }
}


void saveConfiguration(const char *filename, const Config &config) {

  Serial.println(F("saving.."));
  if (SPIFFS.begin(true)) {
    File file = SPIFFS.open(filename, "w");
    if (!file) {
      Serial.println(F("Failed to create file"));
      return;
    }
    StaticJsonDocument<2000> doc;

    // Set the values in the document
    doc["ssid"] = config.ssid;
    doc["pw"] = config.pw;
    doc["wifiTimeout"] = config.wifiTimeout;
    doc["trigName"] = config.trigName;
    doc["trigSelect"] = config.trigSelect;
    doc["triggerOpensMessage"] = config.triggerOpensMessage;
    doc["triggerClosesMessage"] = config.triggerClosesMessage;
    doc["buttonMessage"] = config.buttonMessage;
    doc["timerCountDown"] = config.timerCountDown;
    doc["timerSelect"] = config.timerSelect;
    doc["StillOpenMessage"] = config.StillOpenMessage;
    doc["StillClosedMessage"] = config.StillClosedMessage;

    doc["batteryThreshold"] = config.batteryThreshold * 1000;
    doc["batteryOffset"] = config.batteryOffset * 1000;

    doc["pushUserKey"] = config.pushUserKey;
    doc["pushAPIKey"] = config.pushAPIKey;
    doc["pushOverEnable"] = config.pushOverEnable;
    doc["pushSaferEnable"] = config.pushSaferEnable;
    doc["pushSaferKey"] = config.pushSaferKey;
    doc["iftttEnable"] = config.iftttEnable;
    doc["iftttMakerKey"] = config.iftttMakerKey;
    doc["udpEnable"] = config.udpEnable;
    doc["tcpEnable"] = config.tcpEnable;
    doc["udpTargetIP"] = config.udpTargetIP;
    doc["udpStaticIP"] = config.udpStaticIP;
    doc["udpGatewayAddress"] = config.udpGatewayAddress;
    doc["udpSubnetAddress"] = config.udpSubnetAddress;
    doc["udpPrimaryDNSAddress"] = config.udpPrimaryDNSAddress;
    doc["udpSecondaryDNSAddress"] = config.udpSecondaryDNSAddress;
    doc["udpPort"] = config.udpPort;
    doc["udpPW"] = config.udpPW;
    doc["udpSSID"] =  config.udpSSID;
    doc["rtcCountdownMinute"] =  config.rtcCountdownMinute;
    doc["mqttEnable"] =  config.mqttEnable;
    doc["mqttPort"] =  config.mqttPort;
    doc["mqttServer"] =  config.mqttServer;
    doc["mqttTopic"] =  config.mqttTopic;
    doc["mqttSecureEnable"] =  config.mqttSecureEnable;
    doc["mqttUser"] =  config.mqttUser;
    doc["mqttPW"] =  config.mqttPW;
    doc["staticIPenable"] =  config.staticIPenable;
    doc["staticIP"] =  config.staticIP;
    doc["staticGatewayAddress"] =  config.staticGatewayAddress;
    doc["staticSubnetAddress"] =  config.staticSubnetAddress;
    doc["staticPrimaryDNSAddress"] =  config.staticPrimaryDNSAddress;
    doc["staticSecondaryDNSAddress"] =  config.staticSecondaryDNSAddress;
    doc["highSpeed"] =  config.highSpeed;
    doc["udpBlastCount"] =  config.udpBlastCount;
    doc["udptimeBetweenBlasts"] =  config.udptimeBetweenBlasts;

    // Serialize JSON to file

    if (serializeJson(doc, file) == 0) {
      Serial.println(F("Failed to write to file"));
    }

    // Close the file
    file.close();


  }


}
