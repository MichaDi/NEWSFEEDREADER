boolean findStringInFifo(char *keyword, unsigned long timeToWait)
{
char ch, *filter = "\r\nOK\r\n\r\n+IPD";
static byte currentChar[2] = {0};
unsigned long waitUntil;

  if (keyword != "")
  {
    waitUntil = millis() + timeToWait;
    do
    {
      while (Serial1.available())
      {
        ch = Serial1.read();
        #ifdef DEBUG
        Serial.print(ch);
        #endif
        if (ch == keyword[currentChar[0]])
        {
          if (!keyword[++currentChar[0]])
          {
            currentChar[0] = currentChar[1] = 0;
            return 1;
          }
        }
        else currentChar[0] = 0;
        if (ch == filter[currentChar[1]])
        {
          if (!filter[++currentChar[1]])
          {
            currentChar[1] = 0;
            while(Serial1.read() != ':');
          }
        }
        else currentChar[1] = 0;
      }
    }
    while (millis() < waitUntil);
  }
  else currentChar[0] = currentChar[1] = 0;
  return 0;
}
