/*
   All credit for this Pushover code goes to:
   https://arduino-hannover.de
   Example from here:
   https://github.com/ArduinoHannover/Pushover

*/

boolean pushOver() {
  if (strcmp(config.pushOverEnable, "t") == 0) {//only if enabled

    WiFiClientSecure client;
    client.setInsecure();//had to add this in ESP32 1.0.6
    String _token = config.pushAPIKey;
    String _user = config.pushUserKey;
    //String _message = pushMessage;
    String _message = pushMessage;
    String _device = "";
    String _title = config.trigName;
    String _url;
    String _url_title;
    int8_t _priority = 0;
    uint16_t _retry;
    uint16_t _expire;
    uint32_t _timestamp = 0;
    boolean _html;
    uint16_t _timeout = 5000;
    String _sound = "bike";

    Serial.println("sending to pushover");
    Serial.println(pushMessage);

    if (!client.connect("api.pushover.net", 443)) {
      Serial.println("couldn't connect..");
      //client.stop();
      return false;
    }

    String post = String("token=") + _token + "&user=" + _user + "&title=" + _title + "&message=" + _message + "&device=" + _device + "&url=" + _url + "&url_title=" + _url_title + "&priority=" + _priority + "&retry=" + _retry + "&expire=" + _expire + "&sound=" + _sound;
    //Serial.println(post);
    if (_timestamp != 0) post += String("&timestamp=") + _timestamp;
    if (_html == true) post += String("&html=1");
    String http = String("POST /1/messages.json HTTP/1.1\r\nHost: api.pushover.net\r\nConnection: close\r\nAccept: Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nContent-Length: ") + post.length() + "\r\n\r\n" + post;
    client.print(http);
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

    return true;

  }
  return false;
}
