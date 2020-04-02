/* 
 *  Found example code here: https://iotdesignpro.com/projects/how-trigger-led-using-ifttt-and-esp32-email-notification
 *  
 */


void ifttt() {
  if (strcmp(config.iftttEnable, "t") == 0) {//only if enabled
    wiFiNeeded = true;
    Serial.println("Sending ifttt");
    WiFiClientSecure client;

    const char* host = "maker.ifttt.com";

    const int httpsPort = 443;

    if (!client.connect(host, httpsPort)) {
      Serial.println("couldn't connect..");
      //client.stop();
      return;
    }
    String url = String("/trigger/") + config.trigName + "/with/key/" + config.iftttMakerKey;

//    Serial.print("requesting URL: ");
//
//    Serial.println(url);

    String jsonData = String("{ \"value1\" : \"") + config.trigName + "\", \"value2\" :\"" + pushMessage + "\", \"value3\" : \" \" }";

    client.print(String("POST ") + url + " HTTP/1.1\r\n" +

                 "Host: " + host + "\r\n" +

                 "User-Agent: trigBoard\r\n" +

                 "Connection: close\r\n" + "Content-Type: application/json\r\n" + "Content-Length:" + jsonData.length() + "\r\n\r\n" + jsonData);

    //let's wait for something to come back... only a few seconds
    unsigned long startTimePush = millis();
    while (millis() - startTimePush < 5000) {
      if (client.available()){
        Serial.println("From Server");
        break;
      }
    }
    while (client.available() != 0) {
      Serial.print(char(client.read()));
    }
    client.stop();
    Serial.println("");
  }
}
