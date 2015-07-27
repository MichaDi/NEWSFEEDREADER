boolean connectToNetwork()
{
byte tryNo;
unsigned long waitUntil;

  for (tryNo = 0; tryNo < 3; tryNo++)
  {
    while (1)
    {
      if (!echoCommand("AT+RST", "ready\r\n", 5000))
      {
        digitalWrite(A9, LOW);
        delay(50);
        digitalWrite(A9, HIGH);
        delay(3000);
        while (Serial1.available()) Serial1.read();
        if (!echoCommand("AT+RST", "ready\r\n", 5000)) return 0;
      }
      if (!echoCommand("ATE0", "OK\r\n", 5000)) break;
      delay(20);
      if (!echoCommand("AT+CWMODE=3", "\r\n", 5000)) break;
      delay(20);
      if (!echoCommand("AT+CIPMUX=1", "\r\n", 5000)) break;
      delay(20);
      if (!echoCommand("AT+CWSAP=\"NEWSFEEDREADER\",\"NEWSFEEDREADER\",5,0", "OK\r\n", 5000)) break;
      delay(20);
      if (!echoCommand("AT+CIPSERVER=1,80", "OK\r\n", 5000)) break;
      delay(20);
      waitUntil = millis() + 10000;
      while (echoCommand("AT+CIFSR", "0.0.0.0", 2000))
      {
        if (millis() > waitUntil)
        {
          while (!webServer(0, &ssid, &pass, feed));
          if (!echoCommand("AT+CWJAP=\"" + ssid + "\",\"" + pass + "\"", "OK\r\n", 10000)) break;
          delay(20);
          waitUntil = 0xffffffff;
        }
        delay(100);
      }
      if (echoCommand("AT+CIFSR", "0.0.0.0", 2000)) break;
      return 1;
    }
  }
  return 0;
}
