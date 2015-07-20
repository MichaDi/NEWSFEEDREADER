void displayPrint(char *string, U8GLIB disp, const u8g_fntpgm_uint8_t **fontsArray, byte numOfFonts)
{
char *pString, *stringLine[5] = {0}, stringBuf[50];
byte strLen = 0, remainingLen, charNo, fontNo, lineNo;

  while (string[strLen]) stringBuf[strLen] = string[strLen++];
  stringBuf[strLen] = 0;
  if (strLen > 7) fontNo = 4; else fontNo = 0;
  for (; fontNo < numOfFonts; fontNo++)
  {
    disp.setFont(fontsArray[fontNo]);
    charNo = 0;
    remainingLen = strLen;
    pString = stringBuf;
    for (lineNo = 0; lineNo < (disp.getHeight() - 26) / disp.getFontLineSpacing(); lineNo++)
    {
      charNo = remainingLen;
      while (disp.getStrWidth(pString) > disp.getWidth() && charNo)
      {
        if (charNo != remainingLen) pString[charNo] = ' ';
        while (--charNo)
        {
          if (pString[charNo] == ' ')
          {
            pString[charNo] = 0;
            break;
          }
        }
      }
      stringLine[lineNo] = pString;
      pString = &pString[charNo + 1];
      remainingLen -= charNo;
      if (remainingLen <= 0 || !charNo) break;
    }
    if (remainingLen <= 0) break;
    for (charNo = 0; charNo < strLen; charNo++) stringBuf[charNo] = string[charNo];
  }
  disp.firstPage();
  do
  {
    disp.setFontPosTop();
    for (byte lineCnt = 0; lineCnt <= lineNo; lineCnt++) disp.drawStr(0, 26 + lineCnt * disp.getFontLineSpacing(), stringLine[lineCnt]);
  }
  while (disp.nextPage());
}
