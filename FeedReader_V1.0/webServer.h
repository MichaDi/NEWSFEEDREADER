byte webServer(byte netKnown, String *ssid, String *pass, feed_type *feed)
{
byte connId, msg = 0;
String command = "AT+CIPSEND=",
webSite = F(
"<HTML><HEAD><title>NewsfeedReader SETUP</title></HEAD>"
"<BODY bgcolor=\"#ADCEC0\" text=\"#000000\">"
"<FONT size=\"6\" FACE=\"Verdana\">NewsfeedReader SETUP<BR></FONT>"
"<form action=\"192.168.4.1\" method=\"GET\" accept-charset=\"ISO-8859-1\">"),
webSiteExt = F(
"WIFI-NET SSID<br>"
"<input type=\"text\" id=\"WIFI-NET SSID\" name=\"SSID\" required><br>"
"WIFI-NET PASS<br>"
"<input type=\"password\" name=\"PASS\" required><br>"),
webSiteEnd = 
"1. FEED-URL<br>"
"<input type=\"url\" name=\"FEEDURL1\" value=\"" + feed[0].url + "\" required><br>" +
"2. FEED-URL<br>" +
"<input type=\"url\" name=\"FEEDURL2\" value=\"" + feed[1].url + "\" required><br>" +
"3. FEED-URL<br>" +
"<input type=\"url\" name=\"FEEDURL3\" value=\"" + feed[2].url + "\" required><br><br>" +
"<input type=\"submit\" value=\"Submit\">" +
"<button type=\"reset\">Reset</button>" +
"</form>" +
"</FONT></BODY></HTML>";

  errCnt = 0;
  while (!findStringInFifo("+IPD,", 100))
  {
    if (!echoCommand("AT+CWLIF", "2.", 300))
    {
      displayPrint("Please connect to:", u8g, fonts, sizeof(fonts)/sizeof(u8g_fntpgm_uint8_t*));
      displayPrint("NEWSFEED READER", u8g2, fonts, sizeof(fonts)/sizeof(u8g_fntpgm_uint8_t*));
      while (!echoCommand("AT+CWLIF", "2.", 300));
      msg = 0;
    }
    else if (!msg)
    {
      msg = 1;
      displayPrint("IP-address of web-server:", u8g, fonts, sizeof(fonts)/sizeof(u8g_fntpgm_uint8_t*));
      displayPrint("192.168.4.1", u8g2, fonts, sizeof(fonts)/sizeof(u8g_fntpgm_uint8_t*));
    }
  }
  while (!Serial1.available());
  connId = Serial1.read() - '0';
  if (!findStringInFifo("OK\r\n", 5000)) return 0;
  command += connId;
  command += ",";
  if (netKnown)
  {
    command += (webSite.length() + webSiteEnd.length());
    Serial1.println(command);
    clearSerialBuffer(100);
    Serial1.print(webSite + webSiteEnd);
  }
  else
  {
    command += (webSite.length() + webSiteExt.length() + webSiteEnd.length());
    Serial1.println(command);
    clearSerialBuffer(100);
    Serial1.print(webSite + webSiteExt + webSiteEnd);
  }
  clearSerialBuffer(500);
  command = "AT+CIPCLOSE=";
  command += connId;
  Serial1.println(command);
  if (!findStringInFifo("+IPD,", 60000*10)) return 0;
  while (!Serial1.available());
  connId = Serial1.read() - '0';
  if (!netKnown)
  {
    if (!extractStringFromClientMsg(ssid, "SSID=", 1000)) return 0;
    if (!extractStringFromClientMsg(pass, "PASS=", 1000)) return 0; 
  }
  if (!extractStringFromClientMsg(&feed[0].url, "FEEDURL1=", 5000)) return 0;
  if (!extractStringFromClientMsg(&feed[1].url, "FEEDURL2=", 1000)) return 0;
  if (!extractStringFromClientMsg(&feed[2].url, "FEEDURL3=", 1000)) return 0;
  writeStringToEeprom(200, &feed[0].url);
  writeStringToEeprom(400, &feed[1].url);
  writeStringToEeprom(600, &feed[2].url);
  clearSerialBuffer(1000);
  displayPrint("Please disconnect from:", u8g, fonts, sizeof(fonts)/sizeof(u8g_fntpgm_uint8_t*));
  displayPrint("NEWSFEED READER", u8g2, fonts, sizeof(fonts)/sizeof(u8g_fntpgm_uint8_t*));
  while (echoCommand("AT+CWLIF", "2.", 500)) delay(10);
  command = "AT+CIPCLOSE=";
  command += connId;
  Serial1.println(command);
  clearSerialBuffer(100);
  return 1;
}
