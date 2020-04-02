bool pushLogic() {
  // THIS STUFF ONLY REALLY USED IN HIGH SPEED CASES
  if (strcmp(config.highSpeed, "t") == 0) { //only if enabled
    Serial.println("High Speed Mode");
    if (contactLatchClosed && contactLatchOpen) {
      Serial.println("BOTH Latches were triggered - must have opened//closed real fast");
      Serial.println("Going to send first contact position first, then oposite");
      if (digitalRead(contactStatusPin)) {//is open now, so will send closed first
        contactLatchOpen = false;
      }
      else {
        contactLatchClosed = false;
      }
      contactStatusClosed = !contactStatusClosed;//just so we make sure to send opposite next reboot
    } else { //maybe something else weird covered here
      if (contactLatchOpen && contactStatusClosed) { //latched open, status is closed?
        if (timerWake) { //we're going to ignore this
          Serial.println("treating like a timer wake");
          contactLatchOpen = false;
          contactLatchClosed = false;
        } else {
          Serial.println("Latch Status mismatch, thought opened, but really closed");
          contactLatchOpen = true;
          contactLatchClosed = false;
          Serial.println("Going to send first contact position first, then oposite");
          contactStatusClosed = !contactStatusClosed;
        }
      }
      if (contactLatchClosed && !contactStatusClosed) { //latched closed, status is open?
        if (timerWake) { //we're going to ignore this
          Serial.println("treating like a timer wake");
          contactLatchOpen = false;
          contactLatchClosed = false;
        } else {
          Serial.println("Latch Status mismatch, thought closed, but really open");
          contactLatchOpen = false;
          contactLatchClosed = true;
          Serial.println("Going to send first contact position first, then oposite");
          contactStatusClosed = !contactStatusClosed;
        }

      }
    }
  } else {
    //slow normal speed
    Serial.println("Normal Mode");
    if (timerWake) { //timer wake, then do nothing with contact latches
      contactLatchOpen = false;
      contactLatchClosed = false;
    } else {
      if (contactStatusClosed) {
        contactLatchClosed = true;
        contactLatchOpen = false;
      } else {
        contactLatchClosed = false;
        contactLatchOpen = true;
      }
    }
  }

  //check to see if we need wifi for anything
  // mqtt or ifttt or pushsafer or pushover, udp is a private network configured separately
  if (strcmp(config.iftttEnable, "t") == 0 || strcmp(config.mqttEnable, "t") == 0 || strcmp(config.pushSaferEnable, "t") == 0 || strcmp(config.pushOverEnable, "t") == 0) {
    wiFiNeeded = true;
  }


  //**************************************
  if (buttonWasPressed) {
    Serial.println(F("BUTTON"));
    sprintf(pushMessage, "%s, %sV", config.buttonMessage, batCharString);
    rtcInit(config.timerCountDown, true);//reset timer
    return true;
  }
  //**************************************
  if (strcmp(config.trigSelect, "Close") == 0 && contactLatchClosed) {
    Serial.println(F("CONTACT CLOSE"));
    sprintf(pushMessage, "%s, %sV", config.triggerClosesMessage, batCharString);
    rtcInit(config.timerCountDown, true);//reset timer
    return true;
  }
  //**************************************
  if (strcmp(config.trigSelect, "Open") == 0 && contactLatchOpen) {
    Serial.println(F("CONTACT OPEN"));
    sprintf(pushMessage, "%s, %sV", config.triggerOpensMessage, batCharString);
    rtcInit(config.timerCountDown, true);//reset timer
    return true;
  }
  //**************************************
  if (strcmp(config.trigSelect, "Both") == 0) {
    if (contactLatchOpen) {
      Serial.println(F("CONTACT OPEN"));
      sprintf(pushMessage, "%s, %sV", config.triggerOpensMessage, batCharString);
      rtcInit(config.timerCountDown, true);//reset timer
      return true;
    }
    if (contactLatchClosed) {
      Serial.println(F("CONTACT CLOSE"));
      sprintf(pushMessage, "%s, %sV", config.triggerClosesMessage, batCharString);
      rtcInit(config.timerCountDown, true);//reset timer
      return true;
    }

  }

  //**************************************
  if (timerWake && lowBattery) { //priority
    Serial.println(F("TIMER + LOW BAT"));
    sprintf(pushMessage, "LOW BATTERY!, %sV", batCharString);
    return true;
  }
  //**************************************
  if (timerWake && strcmp(config.timerSelect, "Closed") == 0 && contactStatusClosed) {
    Serial.println(F("TIMER + CONTACT CLOSED"));
    sprintf(pushMessage, "%s, %sV", config.StillClosedMessage, batCharString);
    return true;
  }
  //**************************************
  if (timerWake && strcmp(config.timerSelect, "Open") == 0 && !contactStatusClosed) {
    Serial.println(F("TIMER + CONTACT OPEN"));
    sprintf(pushMessage, "%s, %sV", config.StillOpenMessage, batCharString);
    return true;
  }
  //**************************************
  if (timerWake && strcmp(config.timerSelect, "Either") == 0) {

    if (contactStatusClosed) {
      Serial.println(F("TIMER + CONTACT CLOSE"));
      sprintf(pushMessage, "%s, %sV", config.StillClosedMessage, batCharString);
      return true;
    }
    else {
      Serial.println(F("TIMER + CONTACT OPEN"));
      sprintf(pushMessage, "%s, %sV", config.StillOpenMessage, batCharString);
      return true;
    }

  }









  return false;

}
