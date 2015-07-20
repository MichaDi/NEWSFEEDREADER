void errorHalt(char *msg)
{
char *pMsg = msg, text[20];
byte cnt;
unsigned long waitUntil;
int x = 0;

  u8g.setFont(u8g_font_fur30);
  u8g.setFontPosTop();
  for (cnt = 0; cnt < 20;)
  {
    text[cnt++] = ' ';
    text[cnt] = 0;
    if (u8g.getStrWidth(text) > u8g.getWidth()) break;
  }
  cnt = 0;
  while(1)
  {
    waitUntil = millis() + SCROLLDELAY;
    if (!*pMsg)
    {
      cnt ^= 1;
      if (cnt) pMsg = "   ";
      else pMsg = msg;
    }
    pMsg = prepareStringForDisplay(pMsg, text, &x);
    u8g.firstPage();
    do
    {
      u8g.drawStr(x, 25, text);
    }
    while (u8g.nextPage());
    while (millis() < waitUntil);
  }
}
