byte extractStringFromClientMsg(String *string, char *keyword1, unsigned long timeToWait)
{
char ch, ch2;
unsigned long waitUntil;

  if (!findStringInFifo(keyword1, timeToWait)) return 0;
  *string = "";
  waitUntil = millis() + 2000;
  while (millis() < waitUntil)
  {
    if (Serial1.available())
    {
      ch = Serial1.read();
      #ifdef DEBUG
      Serial.print(ch);
      #endif
      if (ch == '%')
      {
        while (!Serial1.available());
        ch = (Serial1.read() - '0') << 4;
        while (!Serial1.available());
        ch2 = Serial1.read();
        if (ch2 > '9') ch2 -= ('A' - 10);
        else ch2 -= '0';
        ch += ch2;
      }
      else if (ch == '+') ch = ' ';
      else if (ch == ' ' || ch == '&') break;
      *string += ch;
    }
  }
  if (millis() < waitUntil) return 1;
  return 0;
}
