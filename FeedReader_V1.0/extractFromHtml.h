byte extractFromHtml(char *charArray, char *keyword1, char *keyword2, byte noOfMsgs, int maxSize, byte skipMsgs, unsigned long timeToWait)
{
char ch, *filter[] = {keyword1, keyword2, "&lt;", "<![CDATA[", "]]>", "\r\nOK\r\n\r\n+IPD", "Ä", "ä", "Ü", "ü", "Ö", "ö", "ß", "&amp;quot;", "&gt;"};
static byte keywordCnt[13] = {0}, msgNo = 0, keyword1Found = 0, keywordNo = 0;
static int cnt1 = 0;
int cnt2;
unsigned long waitUntil;

  if (keyword1 != "")
  {
    maxSize -= 20;
    waitUntil = millis() + timeToWait;
    do
    {
      while (Serial1.available() && cnt1 < maxSize)
      {
        if (!keyword1Found)
        {
          if (!findStringInFifo(filter[keywordNo], timeToWait))
          {
            if (msgNo > skipMsgs) return msgNo - skipMsgs;
            else return 0;
          }
          keyword1Found = 1;
        }
        else
        {
          ch = Serial1.read();
          if (ch == filter[5][keywordCnt[4]])
          {
            if (!filter[5][++keywordCnt[4]])
            {
              keywordCnt[4] = 0;
              while(Serial1.read() != ':');
            }
          }
          else
          {
            keywordCnt[4] = 0;
            if (ch > 31) charArray[cnt1++] = ch;
            if (ch == filter[1][keywordCnt[0]])
            {
              if (!filter[1][++keywordCnt[0]])
              {
                cnt1 -= keywordCnt[0];
                charArray[cnt1++] = 0;
                keyword1Found = keywordNo = keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] = keywordCnt[5] = 
                keywordCnt[6] = keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
                if (++msgNo == noOfMsgs + skipMsgs)
                {
                  cnt1 = 0;
                  cnt2 = msgNo - skipMsgs;
                  msgNo = 0;
                  return cnt2;
                }
                if (msgNo <= skipMsgs) cnt1 = 0;
                break;
              }
            }
            else keywordCnt[0] = 0;
            if (ch == filter[2][keywordCnt[1]])
            {
              if (!filter[2][++keywordCnt[1]])
              {
                cnt1 -= keywordCnt[1];
                keywordNo = 14;
                keyword1Found = keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] = keywordCnt[5] =
                keywordCnt[6] = keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
                break;
              }
            }
            else keywordCnt[1] = 0;
            if (ch == filter[3][keywordCnt[2]])
            {
              if (!filter[3][++keywordCnt[2]])
              {
                cnt1 -= keywordCnt[2];
                keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] = keywordCnt[5] = keywordCnt[6] =
                keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
                break;
              }
            }
            else keywordCnt[2] = 0;
            if (ch == filter[4][keywordCnt[3]])
            {
              if (!filter[4][++keywordCnt[3]])
              {
                cnt1 -= keywordCnt[3];
                keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] = keywordCnt[5] = keywordCnt[6] =
                keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
                break;
              }
            }
            else keywordCnt[3] = 0;
            if (ch == filter[6][keywordCnt[5]])
            {
              if (!filter[6][++keywordCnt[5]])
              {
                cnt1 -= keywordCnt[5];
                charArray[cnt1++] = 0xc4;
                keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] = keywordCnt[5] = keywordCnt[6] =
                keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
                break;
              }
            }
            else keywordCnt[5] = 0;
            if (ch == filter[7][keywordCnt[6]])
            {
              if (!filter[7][++keywordCnt[6]])
              {
                cnt1 -= keywordCnt[6];
                charArray[cnt1++] = 0xe4;
                keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] = keywordCnt[5] = keywordCnt[6] =
                keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
                break;
              }
            }
            else keywordCnt[6] = 0;
            if (ch == filter[8][keywordCnt[7]])
            {
              if (!filter[8][++keywordCnt[7]])
              {
                cnt1 -= keywordCnt[7];
                charArray[cnt1++] = 0xdc;
                keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] = keywordCnt[5] = keywordCnt[6] =
                keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
                break;
              }
            }
            else keywordCnt[7] = 0;
            if (ch == filter[9][keywordCnt[8]])
            {
              if (!filter[9][++keywordCnt[8]])
              {
                cnt1 -= keywordCnt[8];
                charArray[cnt1++] = 0xfc;
                keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] = keywordCnt[5] = keywordCnt[6] =
                keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
                break;
              }
            }
            else keywordCnt[8] = 0;
            if (ch == filter[10][keywordCnt[9]])
            {
              if (!filter[10][++keywordCnt[9]])
              {
                cnt1 -= keywordCnt[9];
                charArray[cnt1++] = 0xd6;
                keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] = keywordCnt[5] = keywordCnt[6] =
                keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
                break;
              }
            }
            else keywordCnt[9] = 0;
            if (ch == filter[11][keywordCnt[10]])
            {
              if (!filter[11][++keywordCnt[10]])
              {
                cnt1 -= keywordCnt[10];
                charArray[cnt1++] = 0xf6;
                keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] = keywordCnt[5] = keywordCnt[6] =
                keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
                break;
              }
            }
            else keywordCnt[10] = 0;
            if (ch == filter[12][keywordCnt[11]])
            {
              if (!filter[12][++keywordCnt[11]])
              {
                cnt1 -= keywordCnt[11];
                charArray[cnt1++] = 0xdf;
                keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] = keywordCnt[5] = keywordCnt[6] =
                keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
                break;
              }
            }
            else keywordCnt[11] = 0;
            if (ch == filter[13][keywordCnt[12]])
            {
              if (!filter[13][++keywordCnt[12]])
              {
                cnt1 -= keywordCnt[12];
                charArray[cnt1++] = '"';
                keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] = keywordCnt[5] = keywordCnt[6] =
                keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
                break;
              }
            }
            else keywordCnt[12] = 0;
          }
        }
      }
      if (cnt1 >= maxSize)
      {
        charArray[maxSize - 1] = 0;
        cnt1 = 0;
        cnt2 = msgNo;
        msgNo = keyword1Found = keywordNo = keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] =
        keywordCnt[5] = keywordCnt[6] = keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
        if (cnt2 > skipMsgs) return cnt2 - skipMsgs;
        return 0;
      }
    }
    while (millis() < waitUntil);
  }
  else
  {
    cnt1 = 0;
    msgNo = keyword1Found = keywordNo = keywordCnt[0] = keywordCnt[1] = keywordCnt[2] = keywordCnt[3] = keywordCnt[4] =
    keywordCnt[5] = keywordCnt[6] = keywordCnt[7] = keywordCnt[8] = keywordCnt[9] = keywordCnt[10] = keywordCnt[11] = keywordCnt[12] = 0;
  }
  if (msgNo > skipMsgs) return msgNo - skipMsgs;
  return 0;
}
