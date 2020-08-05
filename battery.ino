float getBattery() {
  int batteryRawCount = 0;
  for (int i = 0; i < 10; i++) {
    batteryRawCount = batteryRawCount + analogRead(BatteryPin);
  }
  batteryRawCount = batteryRawCount / 10;

  float m = 0.002424242;
  float b = 0.562424242;
  float batteryVoltage = batteryRawCount * m + b + config.batteryOffset;
  if (batteryVoltage < config.batteryThreshold)
    lowBattery = true;
  else
    lowBattery = false;

  dtostrf(batteryVoltage, 1, 2, batCharString);//convert float to str
  return batteryVoltage;
}
