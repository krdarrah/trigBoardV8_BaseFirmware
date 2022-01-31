/*
   Library here:
   https://github.com/appzer/pushsafer-arduino-library


   Original: by witnessmenow
   https://github.com/witnessmenow/pushsafer-arduino-library

  8/16/21 code now includes entire library here

*/

void pushSafer() {
  if (strcmp(config.pushSaferEnable, "t") == 0) {//only if enabled

    Serial.println("Sending Push Safer");

    struct PushSaferInput input;
    input.message = pushMessage;
    input.title = config.trigName;
    input.sound = "1";
    input.vibration = "1";
    input.icon = "1";
    input.iconcolor = "#FFCCCC";
    input.priority = "0";
    input.device = "a";
    //    input.vibration = "1";
    //    input.url = "https://www.pushsafer.com";
    //    input.urlTitle = "Open Pushsafer.com";
    //    input.picture = "";
    //    input.picture2 = "";
    //    input.picture3 = "";
    //    input.time2live = "";
    //    input.retry = "";
    //    input.expire = "";
    //    input.answer = "";
    Serial.println(Pushsafer_sendEvent(input));
    Serial.println("Sent");
    delay(100);
  }
}

String Pushsafer_sendEvent(PushSaferInput input) {

  String body = "";
  String headers = "";
  long now;
  bool responseReceived;
  String boundary = "------------------------b8f610217e83e29b";

  WiFiClient client;
  //WiFiClientSecure client;
  //client.setInsecure();//had to add this in ESP32 1.0.6
  if (client.connect("pushsafer.com", 80)) {
    Serial.println("Connected");
    String start_request = "";
    String end_request = "";

    // required Params
    start_request = start_request + Pushsafer_buildString(boundary, "k", config.pushSaferKey);
    start_request = start_request + Pushsafer_buildString(boundary, "m", input.message);

    // Optional Params
    if (input.device != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "d", input.device);
    }
    if (input.sound != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "s", input.sound);
    }
    if (input.vibration != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "v", input.vibration);
    }
    if (input.icon != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "i", input.icon);
    }
    if (input.iconcolor != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "c", input.iconcolor);
    }
    if (input.title != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "t", input.title);
    }
    if (input.url != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "u", input.url);
    }
    if (input.urlTitle != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "ut", input.urlTitle);
    }
    if (input.time2live != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "l", input.time2live);
    }
    if (input.priority != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "pr", input.priority);
    }
    if (input.retry != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "re", input.retry);
    }
    if (input.expire != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "ex", input.expire);
    }
    if (input.answer != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "a", input.answer);
    }
    if (input.picture != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "p", input.picture);
    }
    if (input.picture2 != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "p2", input.picture2);
    }
    if (input.picture3 != "")
    {
      start_request = start_request + Pushsafer_buildString(boundary, "p3", input.picture3);
    }

    end_request = end_request + "--" + boundary + "--" + "\r\n";

    client.print("POST /api"); client.println(" HTTP/1.1");
    // Host header
    client.print("Host: "); client.println("pushsafer.com");
    client.println("User-Agent: arduino/1.0");
    client.println("Accept: */*");

    int contentLength = start_request.length() + end_request.length();
    Serial.println("Content-Length: " + String(contentLength));
    client.print("Content-Length: "); client.println(String(contentLength));
    client.println("Content-Type: multipart/form-data; boundary=" + boundary);
    client.println("");

    client.print(start_request);

    Serial.print(start_request);

    client.print(end_request);
    Serial.print(end_request);

    char c;
    now = millis();
    bool finishedHeaders = false;
    bool currentLineIsBlank = true;
    while (millis() - now < 5000) {
      while (client.available()) {
        Serial.println("response");
        char c = client.read();
        responseReceived = true;


        if (!finishedHeaders) {
          if (currentLineIsBlank && c == '\n') {
            finishedHeaders = true;
          }
          else {
            headers = headers + c;
          }
        } else {
          body = body + c;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }

      }

      if (responseReceived) {

        Serial.println(body);

        break;
      }
      delay(10); //Feed watchdog
    }
  } else {
    Serial.println("Not Connected");
  }

  return body;
}

String Pushsafer_buildString(String boundary, String name, String value) {

  String formData;
  formData = formData + "--" + boundary + "\r\n";
  formData = formData + "content-disposition: form-data; name=\"" + name + "\"" + "\r\n";
  formData = formData + "\r\n";
  formData = formData + value + "\r\n";

  return formData;
}
