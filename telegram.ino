/*******************************************************************
    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow

    Tutorial on setup:
    https://randomnerdtutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/
 *******************************************************************/
 
void telegram() {
  if (strcmp(config.telegramEnable, "t") == 0) {//only if enabled
    Serial.println("Sending to Telegram");
    WiFiClientSecure secured_client;
    UniversalTelegramBot bot(config.telegramBOT, secured_client);
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
    char telegramMessage[100];
    sprintf(telegramMessage, "%s\n%s", config.trigName, pushMessage);
    if(bot.sendMessage(config.telegramCHAT, telegramMessage, "")){
      Serial.println("Sent");
    }else{
      Serial.println("Failed");
    }
  }
}
