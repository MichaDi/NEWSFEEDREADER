char* prepareStringForDisplay(char *message, char *string, int *x)
{
char ch[2] = {string[0], 0}, *allBlank = "1";
byte cnt1 = 0, cnt2 = 0;

  *x -= 2;
  if (message != "")
  {
    if (-*x >= u8g.getStrWidth(ch))
    {
      *x += u8g.getStrWidth(ch);
      while (string[cnt1]) string[cnt1++] = string[cnt1];
      cnt1--;
      ch[0] = message[0];
      while (u8g.getStrWidth(string) + u8g.getStrWidth(ch) < u8g.getWidth() + 40 && ch[0])
      {
        string[cnt1++] = ch[0];
        string[cnt1] = 0;
        ch[0] = message[++cnt2];
      }
    }
    return &message[cnt2];
  }
  if (-*x >= u8g.getStrWidth(ch))
  {
    *x += u8g.getStrWidth(ch);
    while (string[cnt1])
    {
      string[cnt1++] = string[cnt1];
      if (string[cnt1] && string[cnt1] != ' ') allBlank = "";
    }
    cnt1--;
    while (u8g.getStrWidth(string) + u8g.getStrWidth(" ") < u8g.getWidth() + 40)
    {
      string[cnt1++] = ' ';
      string[cnt1] = 0;
    }
    return allBlank;
  }
  return "";
}
