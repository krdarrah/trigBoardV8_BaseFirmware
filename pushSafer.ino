/*
 * Library here: 
 * https://github.com/appzer/pushsafer-arduino-library
 * 
 * 
 * Original: by witnessmenow
 * https://github.com/witnessmenow/pushsafer-arduino-library
 * 
 * NOTE!! I had to make two changed to use SSL
 * 
 * Pushsafer.h 
 *  Uncomment - define SSL_PORT 443
 *  Comment - #define PORT 80
 *  
 *  Pushsafer.cpp
 *   Uncomment - if (client->connect(HOST, SSL_PORT)) {
 *   Comment - if (client->connect(HOST, PORT)) {
 *  
 */


void pushSafer() {
  if (strcmp(config.pushSaferEnable, "t") == 0) {//only if enabled
 
    Serial.println("Sendning Push Safer");
    WiFiClientSecure client;

    Pushsafer pushsafer(config.pushSaferKey, client);
    pushsafer.debug = true;

    struct PushSaferInput input;
    input.message = pushMessage;
    input.title = config.trigName;
    input.sound = "1";
    input.vibration = "1";
    input.icon = "1";
    input.iconcolor = "#FFCCCC";
    input.priority = "0";
    input.device = "a";
    Serial.println(pushsafer.sendEvent(input));
    Serial.println("Sent");
    delay(100);
  }
}
