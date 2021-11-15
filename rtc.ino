const int   daylightOffset_sec = 3600;

void writeRTC(byte regAddr, byte dataToWrite);
byte readRTC(byte regAddr);

void timestampAppend() {
  if (strcmp(config.clkAppendEnable, "t") == 0 && strcmp(config.clkEnable, "t") == 0 && !clockWake) {// append all push messages
    rtcGetTime();
    strcat(pushMessage, ",");
    strcat(pushMessage, rtcTimeStamp);
  }
  if (strcmp(config.clkAppendAlmEnable, "t") == 0 && strcmp(config.clkEnable, "t") == 0 && clockWake) {//append clock alarm message
    rtcGetTime();
    strcat(pushMessage, ",");
    strcat(pushMessage, rtcTimeStamp);
  }
}

void nptUpdateTime() {
  if (clockWake && strcmp(config.clkUpdateNPTenable, "t") == 0) {
    Serial.println(F("syncing with server time..."));
    for (int i = 0; i < 5; i++) {
      if (getNTPtime())
        i = 5;
      else
        delay(1000);
    }
  }
}

bool getNTPtime() {
  configTime(config.clkTimeZone * 3600, daylightOffset_sec,  "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println(F("Failed to obtain time"));
    return false;
  }
  Serial.println(&timeinfo, "Server Time: %A, %m %d %y %H:%M:%S");
  //  Serial.print("Day of week: ");
  //  Serial.println(&timeinfo, "%A");
  //Serial.print("Month: ");
  //Serial.println(&timeinfo, "%m");
  //  Serial.print("Day of Month: ");
  //  Serial.println(&timeinfo, "%d");
  //  Serial.print("Year: ");
  //  Serial.println(&timeinfo, "%Y");
  //  Serial.print("Hour: ");
  //  Serial.println(&timeinfo, "%H");
  //  Serial.print("Hour (12 hour format): ");
  //  Serial.println(&timeinfo, "%I");
  //  Serial.print("Minute: ");
  //  Serial.println(&timeinfo, "%M");
  //  Serial.print("Second: ");
  //  Serial.println(&timeinfo, "%S");
  //
  //  Serial.println("Time variables");
  //  char timeHour[3];
  //  strftime(timeHour, 3, "%H", &timeinfo);
  //  Serial.println(timeHour);
  //  char timeWeekDay[10];
  //  strftime(timeWeekDay, 10, "%A", &timeinfo);
  //  Serial.println(timeWeekDay);
  //  Serial.println();

  char timeSeconds[3];
  strftime(timeSeconds, 3, "%S", &timeinfo);
  byte timeSecondsRegister = (timeSeconds[1] - 48) + ((timeSeconds[0] - 48) << 4);
  //writeRTC(0x04, timeSecondsRegister);

  char timeMinutes[3];
  strftime(timeMinutes, 3, "%M", &timeinfo);
  byte timeMinutesRegister = (timeMinutes[1] - 48) + ((timeMinutes[0] - 48) << 4);
  //writeRTC(0x05, timeMinutesRegister);

  char timeHours[3];
  strftime(timeHours, 3, "%H", &timeinfo);
  byte timeHoursRegister = (timeHours[1] - 48) + ((timeHours[0] - 48) << 4);
  //writeRTC(0x06, timeHoursRegister);

  char timeDate[3];
  strftime(timeDate, 3, "%d", &timeinfo);
  byte timeDateRegister = (timeDate[1] - 48) + ((timeDate[0] - 48) << 4);
  //writeRTC(0x07, timeDateRegister);

  char timeMonth[3];
  strftime(timeMonth, 3, "%m", &timeinfo);
  byte timeMonthRegister = (timeMonth[1] - 48) + ((timeMonth[0] - 48) << 4);
  //writeRTC(0x09, timeMonthRegister);

  char timeYear[3];
  strftime(timeYear, 3, "%y", &timeinfo);
  byte timeYearRegister = (timeYear[1] - 48) + ((timeYear[0] - 48) << 4);
  //writeRTC(0x0A, timeYearRegister);

  Wire.beginTransmission(0x51);//7 bit for RTC
  Wire.write(0x04);
  Wire.write(timeSecondsRegister);
  Wire.write(timeMinutesRegister);
  Wire.write(timeHoursRegister);
  Wire.write(timeDateRegister);
  Wire.write(0);//day fo week
  Wire.write(timeMonthRegister);
  Wire.write(timeYearRegister);
  Wire.endTransmission();
  return true;
}


void rtcGetTime() {

  // I know... this is not great BCD to Int conversion, but it works...
  byte rtcSecondsRegister = readRTC(0x04) & 0x7F;
  byte rtcSeconds = (rtcSecondsRegister & 0x0F) + ((rtcSecondsRegister >> 4) & 1) * 10 + ((rtcSecondsRegister >> 5) & 1) * 20 + ((rtcSecondsRegister >> 6) & 1) * 40;
  byte rtcMinutesRegister = readRTC(0x05) & 0x7F;
  byte rtcMinutes = (rtcMinutesRegister & 0x0F) + ((rtcMinutesRegister >> 4) & 1) * 10 + ((rtcMinutesRegister >> 5) & 1) * 20 + ((rtcMinutesRegister >> 6) & 1) * 40;
  byte rtcHoursRegister = readRTC(0x06) & 0x3F;
  byte rtcHours = (rtcHoursRegister & 0x0F) + ((rtcHoursRegister >> 4) & 1) * 10 + ((rtcHoursRegister >> 5) & 1) * 20;
  byte rtcDateRegister = readRTC(0x07) & 0x3F;
  byte rtcDate = (rtcDateRegister & 0x0F) + ((rtcDateRegister >> 4) & 1) * 10 + ((rtcDateRegister >> 5) & 1) * 20;
  //day of the week?
  byte rtcMonthRegister = readRTC(0x09) & 0x1F;
  byte rtcMonth = (rtcMonthRegister & 0x0F) + ((rtcMonthRegister >> 4) & 1) * 10;
  byte rtcYearRegister = readRTC(0x0A);
  byte rtcYear = (rtcYearRegister & 0x0F) + ((rtcYearRegister >> 4) & 1) * 10 + ((rtcYearRegister >> 5) & 1) * 20 + ((rtcYearRegister >> 6) & 1) * 40 + ((rtcYearRegister >> 7) & 1) * 80;

  //Serial.printf("RTC: %i:%i:%i_%i/%i/%i\n", rtcHours, rtcMinutes, rtcSeconds, rtcMonth, rtcDate, rtcYear);
  if (rtcSeconds > 9 && rtcMinutes > 9)
    sprintf(rtcTimeStamp, "%i:%i:%i_%i/%i/%i", rtcHours, rtcMinutes, rtcSeconds, rtcMonth, rtcDate, rtcYear);
  else if (rtcSeconds > 9 && rtcMinutes < 10)
    sprintf(rtcTimeStamp, "%i:0%i:%i_%i/%i/%i", rtcHours, rtcMinutes, rtcSeconds, rtcMonth, rtcDate, rtcYear);
  else if (rtcSeconds < 10 && rtcMinutes > 9)
    sprintf(rtcTimeStamp, "%i:%i:0%i_%i/%i/%i", rtcHours, rtcMinutes, rtcSeconds, rtcMonth, rtcDate, rtcYear);
  else if (rtcSeconds < 10 && rtcMinutes < 10)
    sprintf(rtcTimeStamp, "%i:0%i:0%i_%i/%i/%i", rtcHours, rtcMinutes, rtcSeconds, rtcMonth, rtcDate, rtcYear);


  //Serial.println(rtcTimeStamp);
}

bool rtcInit(byte timeValue, bool setNewTime) {

  //https://www.microcrystal.com/fileadmin/Media/Products/RTC/App.Manual/RV-8263-C7_App-Manual.pdf

  //Interval Mode (TI_TP = 0):
  // TD = 11 1/60 so wake time is from 60s to 4hr 15min
  // 1 = 60sec
  //255 = 4hr 15min


  if (contactChanged) { //save a bit in RAM if contact changed on us
    writeRTC(0x03, 0x01);
  } else {
    //could be first boot, so let's set a flag in case contact was changed
    byte reg03 = readRTC(0x03);
    if (reg03 == 0x01) {
      Serial.println("Contact changed during last wake");//forcing pin state for logic
      if (digitalRead(contactStatusPin)) {
        contactLatchOpen = true;
        contactLatchClosed = false;
      } else {
        contactLatchClosed = true;
        contactLatchOpen = false;
      }
    }
    writeRTC(0x03, 0x00);//clear it if nothing changed
  }


  byte reg01 = readRTC(0x01);
  if (((reg01 >> 3) & 0x1) == 1) {//TF INT
    Serial.println("Timer Wake!");
    timerWake = true;
    writeRTC(0x01, 0x07);//clear that interrupt, but also keep clockout off
    writeRTC(0x11, 0x00);//freeze timer for now
  } else
    timerWake = false;

  if (((reg01 >> 6) & 0x1) == 1) {//AF INT
    Serial.println("Clock Wake!");
    reg01 &= ~(0x40);
    delay(1100);// 1sec plus a little extra to let interrupt clear
    writeRTC(0x01, reg01); //clear the interrupt
    clockWake = true;
  }
  else
    clockWake = false;

  byte timerMode = readRTC(0x11);
  if (((timerMode >> 2) & 0x1) == 0 || setNewTime) { //if timer fired, was never setup, or GUI changed something
    //writeRTC(0x00, 0x58);//soft reset for good measure
    writeRTC(0x01, 0x07);//turn off clock out FD=111
    writeRTC(0x10, timeValue);//set count down time, 1 = 60seconds = 1min, so 60 = 1hr
    if (strcmp(config.rtcCountdownMinute, "t") == 0 && !checkAgainSet) {
      //Serial.println("Minute Mode");
      writeRTC(0x11, 0x1E);//minute mode, timer and interrupts enabled
    }
    else {
      //Serial.println("Seconds Mode");
      writeRTC(0x11, 0x16);//seconds mode, timer and interrupts enabled
    }
  }


  // clock alarm settings
  if (strcmp(config.clkAlarmEnable, "t") == 0 && strcmp(config.clkEnable, "t") == 0) {
    reg01 = readRTC(0x01);
    reg01 |= 0x80;
    reg01 &= ~(0x40);
    writeRTC(0x01, reg01); //enable alarm
    writeRTC(0x0B, 0x00);//seconds alarm - fixing at 00
    byte ones = config.clkAlarmMinute % 10;
    byte tens = (config.clkAlarmMinute / 10) % 10;
    writeRTC(0x0C, ones + (tens << 4)); //minutes alarm
    ones = config.clkAlarmHour % 10;
    tens = (config.clkAlarmHour / 10) % 10;
    writeRTC(0x0D, ones + (tens << 4));//hours alarm

  } else {
    reg01 &= ~(0x80);
    writeRTC(0x01, reg01); //disable alarm
  }




  if (timerWake) {
    if (strcmp(config.checkAgain, "t") == 0 && strcmp(config.timerCheck, "t") == 0) {//this means we're checking the contact again
      Serial.println("THIS IS A MISSION CRITICAL WAKE");
      
      //reset
      strlcpy(config.timerCheck,                  // <- destination
              "f",  // <- source
              sizeof(config.timerCheck));         // <- destination's capacity

      saveConfiguration(filename, config);

      if (strcmp(config.lastState, "O") == 0 && contactStatusClosed) {
        Serial.println("WAS OPEN, BUT CHECK FOUND CLOSED");
        timerWake = false;//this should force the logic to treat this as a contact wake
        return false;
      }
      if (strcmp(config.lastState, "C") == 0 && !contactStatusClosed) {
        Serial.println("WAS CLOSED, BUT CHECK FOUND OPEN");
        timerWake = false;//this should force the logic to treat this as a contact wake
        return false;
      }
      Serial.println("ALL GOOD!");
      killPower();//this means everything is good on the check!

      //return false;
    }


    return true;
  }
  else
    return false;
}


void writeRTC(byte regAddr, byte dataToWrite) {
  Wire.beginTransmission(0x51);//7 bit for RTC
  Wire.write(regAddr);
  Wire.write(dataToWrite);
  Wire.endTransmission();
}
byte readRTC(byte regAddr) {
  byte readDataByte = 0;
  Wire.beginTransmission(0x51);//7 bit for RTC
  Wire.write(regAddr);
  Wire.endTransmission();
  Wire.requestFrom(0x51, 1);
  if (Wire.available()) {
    readDataByte = Wire.read();
  }
  else {
    Serial.println("Error with RTC");
  }
  return readDataByte;
}
