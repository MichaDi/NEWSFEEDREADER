char* truncateTitle(char *charArray1, char *charArray2, int maxSize, const u8g_fntpgm_uint8_t *font, char *separator)
{
int cnt = 0;

  u8g.setFont(font);
  while (charArray1[cnt] && cnt < maxSize)
  {
    charArray2[cnt] = charArray1[cnt];
    charArray2[++cnt] = 0;
    if (u8g.getStrWidth(charArray2) > u8g.getWidth())
    {
      charArray1 = &charArray1[cnt - 1];
      while (u8g.getStrWidth(charArray2) + u8g.getStrWidth(separator) > u8g.getWidth()) charArray2[--cnt] = 0;
      while(*separator) charArray2[cnt++] = *separator++;
      charArray2[cnt] = 0;
      return charArray1;
    }
  }
  return "";
}
