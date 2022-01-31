
void checkWakeupPins() {
  pinMode(contactOpenedPin, INPUT);
  pinMode(contactClosedPin, INPUT);
  pinMode(contactStatusPin, INPUT);
  pinMode(wakeButtonPin, INPUT);

  if (digitalRead(contactStatusPin)) {
    Serial.println("Contact Status is Open");
    contactStatusClosed = false;
  }
  else {
    contactStatusClosed = true;
    Serial.println("Contact Status is Closed");
  }


  if (!digitalRead(contactOpenedPin)) {
    Serial.println("Contact from Closed to Open");
    contactLatchOpen = true;
  }
  else
    contactLatchOpen = false;


  if (!digitalRead(contactClosedPin)) {
    Serial.println("Contact from Open to Closed");
    contactLatchClosed = true;
  }
  else
    contactLatchClosed = false;


  if (!digitalRead(wakeButtonPin)) {
    Serial.println("Wake Button was pressed");
    buttonWasPressed = true;
  } else
    buttonWasPressed = false;

  pinMode(killPowerPin, OUTPUT);
  digitalWrite(killPowerPin, HIGH);
  //  delay(100);
  //  digitalWrite(killPowerPin, LOW);
}


void killPower() {
  Serial.println("killing power");
  WiFi.disconnect(true);//done with wireless for now
  WiFi.mode(WIFI_OFF);
  btStop();
  checkIfContactChanged();//in case contact changed while pushing message out
  //could get stuck if you really beat on it
  unsigned long waitForLatchStartTime = millis();
  while ((!digitalRead(contactClosedPin) || !digitalRead(contactOpenedPin))) {//in case a race condition
    //do nothing, we have to wait for things to unlatch before proceeding
  }
  checkIfContactChanged();//that might've taken a while, so better check to see if things changed

  //mission critical stuff to force a check after sleep
  if (!timerWake && !clockWake && !buttonWasPressed && strcmp(config.checkAgain, "t") == 0) { // check again mode only on normal contact wake
    Serial.println("setting check again");
    checkAgainSet = true; //go and set timer
    if (contactStatusClosed) {
      strlcpy(config.lastState,                  // <- destination
              "C",  // <- source
              sizeof(config.lastState));         // <- destination's capacity
    } else {
      strlcpy(config.lastState,                  // <- destination
              "O",  // <- source
              sizeof(config.lastState));         // <- destination's capacity
    }
    strlcpy(config.timerCheck,                  // <- destination
            "t",  // <- source
            sizeof(config.timerCheck));         // <- destination's capacity

    saveConfiguration(filename, config);
    rtcInit(config.secondsAfterToCheckAgain, true);
  }
  digitalWrite(ESPlatchPin, LOW);//when this goes LOW, we are done...
  Serial.println("zzz");
  if (digitalRead(wakeButtonPin))//and wake not pressed, but still alive
    rtcInit(config.timerCountDown, false);//just in case the RTC fired and we're still here
}

void checkIfContactChanged() {
  if (!digitalRead(contactStatusPin) != contactStatusClosed) {//changed
    contactChanged = true;
    rtcInit(config.timerCountDown, false);
    Serial.println("contact changed - rebooting");
    delay(100);
    ESP.restart();
    delay(1000);
  }
}

void waitForButton() {
  Serial.println("waiting for button");
  unsigned long buttonHoldStartTime = millis();
  while (millis() - buttonHoldStartTime < 3000) {
  }
  digitalWrite(ESPlatchPin, HIGH);

  for (int i = 0; i < 20; i++) {
    digitalWrite(LEDpin, !digitalRead(LEDpin));
    delay(50);
  }
  digitalWrite(LEDpin, LOW);//stay on
}
