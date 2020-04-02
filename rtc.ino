
void writeRTC(byte regAddr, byte dataToWrite);
byte readRTC(byte regAddr);

bool rtcInit(byte timeValue, bool setNewTime) {

  //https://www.microcrystal.com/fileadmin/Media/Products/RTC/App.Manual/RV-8263-C7_App-Manual.pdf

  //Interval Mode (TI_TP = 0):
  // TD = 11 1/60 so wake time is from 60s to 4hr 15min
  // 1 = 60sec
  //255 = 4hr 15min
  Wire.begin(SDApin, SCLpin);

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
  if (((reg01 >> 3) & 0x1) == 1) {
    Serial.println("Timer Wake!");
    timerWake = true;
    writeRTC(0x01, 0x03);//clear that interrupt, but also keep clockout off
    writeRTC(0x11, 0x00);//freeze timer for now
  } else
    timerWake = false;

  byte timerMode = readRTC(0x11);
  if (((timerMode >> 2) & 0x1) == 0 || setNewTime) { //if timer fired, was never setup, or GUI changed something
    writeRTC(0x00, 0x58);//soft reset for good measure
    writeRTC(0x01, 0x03);//turn off clock out FD=111
    writeRTC(0x10, timeValue);//set count down time, 1 = 60seconds = 1min, so 60 = 1hr
    if (strcmp(config.rtcCountdownMinute, "t") == 0) {
      //Serial.println("Minute Mode");
      writeRTC(0x11, 0x1E);//minute mode, timer and interrupts enabled
    }
    else {
      //Serial.println("Seconds Mode");
      writeRTC(0x11, 0x16);//seconds mode, timer and interrupts enabled
    }
  }

  if (timerWake)
    return true;
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
