
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
  delay(100);
  digitalWrite(killPowerPin, LOW);
}


void killPower() {
  Serial.println("killing power");
  checkIfContactChanged();
  //could get stuck if you really beat on it
  unsigned long waitForLatchStartTime = millis();
  while ((!digitalRead(contactClosedPin) || !digitalRead(contactOpenedPin))) {//in case a race condition
    if (millis() - waitForLatchStartTime > 1000) {//added this in after finding that sometimes it freezes up in here
      Serial.println("latches did not unlatch - trying again");
      digitalWrite(killPowerPin, HIGH);
      delay(100);
      digitalWrite(killPowerPin, LOW);
      waitForLatchStartTime = millis();
    }
    //Serial.println("dying");
  }
  checkIfContactChanged();
  rtcInit(config.timerCountDown, false);//just in case the RTC timer tripped while we were doing something else
  checkIfContactChanged();
  digitalWrite(ESPlatchPin, LOW);
  Serial.println("last breath...");
  delay(100);
  if (digitalRead(wakeButtonPin))//and wake not pressed, but still alive
    rtcInit(config.timerCountDown, false);//just in case the RTC fired and we're still here

}

void checkIfContactChanged() {
  if (!digitalRead(contactStatusPin) != contactStatusClosed) {//chnaged
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
