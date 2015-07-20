//#define DEBUG
#define NUMOFFEEDS             3
#define NUMOFMESSAGES          10
#define MAXFEEDSIZE            1000
#define MAXTITLELEN            100
#define UPDATEINTERVAL         60000*3
#define SCROLLDELAY            25
#define FEEDCHANGEINTERVAL     60000*5
#define CHECKFORCLIENTINTERVAL 3000
#define WAITFORENDOFTRANSFER   5000

#include <EEPROM.h>
#include <SPI.h>
#include <u8g_teensy.h>
#include "U8glib.h"
#include "structures.h"

U8GLIB u8g(&u8g_dev_ssd1306_128x64_hw_spi, u8g_com_hw_spi_fn1);
U8GLIB u8g2(&u8g_dev_ssd1306_128x64_2x_hw_spi, u8g_com_hw_spi_fn2);

feed_type feed[NUMOFFEEDS];
byte errCnt = 0;
char feedBuf[NUMOFMESSAGES * MAXFEEDSIZE], text[2][20];
int x = 0;
const u8g_fntpgm_uint8_t *fonts[] = {u8g_font_fur30, u8g_font_fur25, u8g_font_fur20, u8g_font_fur17, u8g_font_fur14, u8g_font_fur11, u8g_font_profont12};
String ssid, pass;

#include "clearSerialBuffer.h"
#include "prepareStringForDisplay.h"
#include "displayPrint.h"
#include "readStringFromEeprom.h"
#include "writeStringToEeprom.h"
#include "errorHalt.h"
#include "truncateTitle.h"
#include "findStringInFifo.h"
#include "echoCommand.h"
#include "makeCommands.h"
#include "extractStringFromClientMsg.h"
#include "webServer.h"
#include "connectToNetwork.h"
#include "extractFromHtml.h"

void setup()
{
  #ifdef DEBUG
  Serial.begin(115200);
  #endif
  pinMode(A9, OUTPUT);
  digitalWrite(A9, HIGH);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
//  Serial1.begin(9600); delay(2000); Serial1.println("AT+CIOBAUD=115200"); while(1); //uncomment on first use of ESP8266
  u8g.setColorIndex(1);
  u8g2.setColorIndex(1);
  Serial1.begin(115200);
  delay(1500);
  clearSerialBuffer(1000);
}
void loop()
{
char *pMessage[2] = {feed[0].feedStrings, feed[0].feedStrings};
byte cmdCnt = 0, waitForAck = 0, msgCnt, downloadDone = 0, tryNo = 0, fontNo = 0, greatestDtMsgNo = 0, feedNo,
     stillReceiving = 1;
unsigned long waitUntil1, waitUntil2, waitUntil3, waitUntil4, waitUntil5, cnt, cnt2, strWidth, dispPosX;

  displayPrint("Please wait...", u8g, fonts, sizeof(fonts)/sizeof(u8g_fntpgm_uint8_t*));
  displayPrint(" ", u8g2, fonts, sizeof(fonts)/sizeof(u8g_fntpgm_uint8_t*));
  if (!(readStringFromEeprom(200, &feed[0].url, 200) && readStringFromEeprom(400, &feed[1].url, 200) && readStringFromEeprom(600, &feed[2].url, 200)))
  {
    feed[0].url = "http://www.spiegel.de/schlagzeilen/index.rss";
    feed[1].url = "http://rss.kicker.de/news/aktuell";
    feed[2].url = "http://rss.chip.de/c/573/f/7440/index.rss";
    if (!connectToNetwork()) errorHalt("unable to initialize wifi!");
    while (!(readStringFromEeprom(200, &feed[0].url, 200) && readStringFromEeprom(400, &feed[1].url, 200) && readStringFromEeprom(600, &feed[2].url, 200))) while (!webServer(1, &ssid, &pass, feed));
  }
  else if (!connectToNetwork()) errorHalt("unable to initialize wifi!");
  if (errCnt++ > 2)
  {
    while (!webServer(1, &ssid, &pass, feed));
    return;
  }
  for (feedNo = 0; feedNo < NUMOFFEEDS; feedNo++)
  {
    makeCommands(feed[feedNo].command, feed[feedNo].ack, feed[feedNo].url);
    for (cnt = 0; cnt < 3; cnt++) if (!echoCommand(feed[feedNo].command[cnt], feed[feedNo].ack[cnt], 5000)) return;
    if (!extractFromHtml(feed[feedNo].feedTitle, "<title>", "</title>", 1, MAXTITLELEN, 0, 10000)) return;
    if (!(feed[feedNo].numOfMsgs = extractFromHtml(feed[feedNo].feedStrings, "<description>", "</description>", NUMOFMESSAGES, NUMOFMESSAGES * MAXFEEDSIZE, 1, 10000))) return;
    cnt2 = 0;
    for (cnt = 0; cnt < feed[feedNo].numOfMsgs; cnt++)
    {
      feed[feedNo].startAddrMsg[cnt] = &feed[feedNo].feedStrings[cnt2];
      while (feed[feedNo].feedStrings[cnt2++]);
      #ifdef DEBUG
      Serial.println(feed[feedNo].startAddrMsg[cnt]);
      #endif
    }
    truncateTitle(truncateTitle(feed[feedNo].feedTitle, feed[feedNo].dispTitle[0], MAXTITLELEN, u8g_font_fur11, ""), feed[feedNo].dispTitle[1], MAXTITLELEN, u8g_font_fur11, "...");
    clearSerialBuffer(1500);
    Serial1.println("AT+CIPCLOSE=0");
    clearSerialBuffer(100);
  }
  u8g.setFont(u8g_font_fur30);
  feedNo = 0;
  for (cnt = 0; cnt < 20;)
  {
    text[0][cnt++] = ' ';
    text[0][cnt] = 0;
    if (u8g.getStrWidth(text[0]) > u8g.getWidth()) break;
  }
  feed[0].msgTime[0] = millis();
  waitUntil1 = millis() + UPDATEINTERVAL;
  waitUntil3 = millis() + FEEDCHANGEINTERVAL;
  waitUntil4 = millis() + CHECKFORCLIENTINTERVAL;
  waitUntil5 = millis() + WAITFORENDOFTRANSFER;
  while (1)
  {
    while (1)
    {
      waitUntil2 = millis() + SCROLLDELAY;
      u8g.firstPage();
      do
      {
        u8g.setFont(u8g_font_fur11);
        u8g.setFontPosTop();
        u8g.drawStr(0, 0, feed[feedNo].dispTitle[0]);
        u8g.setFont(u8g_font_fur30);
        u8g.setFontPosTop();
        u8g.drawStr(x, 25, text[0]);
      }
      while (u8g.nextPage());
      if (pMessage[1] < pMessage[0])
      {
        cnt = 0;
        while (*pMessage[1]++ == ' ');
        if (*--pMessage[1])
        {
          for (cnt = 0; cnt < 20; cnt++)
          {
            text[1][cnt] = *pMessage[1]++;
            if (!*pMessage[1] || *pMessage[1] == ' ') break;
          }
          if (cnt == 21)
          {
            for (cnt = 16; cnt > 3; cnt--) if (text[1][cnt] == '.' || text[1][cnt] == ',' || text[1][cnt] == '-') break;
            if (cnt == 3) cnt = 20;
            else pMessage[1] -= (20 - cnt);
          }
          text[1][++cnt] = 0;
          if (cnt > 10) cnt2 = 3; else cnt2 = 0;
          for (; cnt2 < sizeof(fonts)/sizeof(u8g_fntpgm_uint8_t*);)
          {
            u8g2.setFont(fonts[cnt2++]);
            strWidth = u8g2.getStrWidth(text[1]);
            if (strWidth <= u8g2.getWidth()) break;
          }
          if (cnt2 == 7)
          {
            do
            {
              text[1][--cnt] = 0;
              strWidth = u8g2.getStrWidth(text[1]);
              pMessage[1]--;
            }
            while (strWidth > u8g2.getWidth());
            text[1][--cnt] = '-';
          }
          fontNo = --cnt2;
        }
        else pMessage[1]++;
      }
      if (strWidth < u8g2.getWidth()) dispPosX = (u8g2.getWidth() - strWidth) / 2;
      else dispPosX = 0;
      u8g2.firstPage();
      do
      {
        u8g2.setFont(u8g_font_fur11);
        u8g2.setFontPosTop();
        u8g2.drawStr(0, 0, feed[feedNo].dispTitle[1]);
        u8g2.setFont(fonts[fontNo]);
        u8g2.setFontPosCenter();
        u8g2.drawStr(dispPosX, (u8g2.getHeight() - 26) / 2 + 26, text[1]);
      }
      while (u8g2.nextPage());
      if (!*pMessage[0])
      {
        if (prepareStringForDisplay("", text[0], &x) != "") break;
      }
      else pMessage[0] = prepareStringForDisplay(pMessage[0], text[0], &x);
      while (millis() < waitUntil2)
      {
        if (waitForAck)
        {
          if (findStringInFifo(feed[feedNo].ack[cmdCnt], 0))
          {
            waitForAck = 0;
            cmdCnt++;
          }
          else if (millis() > waitUntil1 + 10000)
          {
            if (cmdCnt) Serial1.println("AT+CIPCLOSE=0");
            if (++tryNo == 3) return;
            cmdCnt = 0;
            waitUntil1 = millis() + UPDATEINTERVAL;
          }
        }
        else if (millis() > waitUntil1 && !downloadDone)
        {
          if (cmdCnt < 3)
          {
            while (Serial1.available()) Serial1.read();
            Serial1.println(feed[feedNo].command[cmdCnt]);
            waitForAck = 1;
          }
          else
          {
            feed[0].numOfMsgs = extractFromHtml(feedBuf, "<description>", "</description>", NUMOFMESSAGES, NUMOFMESSAGES * MAXFEEDSIZE, 1, 0);
            if (feed[0].numOfMsgs == NUMOFMESSAGES)
            {
              Serial1.println("AT+CIPCLOSE=0");
              cmdCnt = 0;
              downloadDone = 1;
              tryNo = 0;
              stillReceiving = 1;
              waitUntil5 = millis() + 5000;
              waitUntil1 = millis() + UPDATEINTERVAL;
            }
            else if (millis() > waitUntil1 + 20000)
            {
              extractFromHtml(feedBuf, "", "", 0, 0, 0, 0);
              Serial1.println("AT+CIPCLOSE=0");
              if (!feed[0].numOfMsgs)
              {
                if (++tryNo == 3) errorHalt("extractFromHtml timeout!");
              }
              else downloadDone = 1;
              cmdCnt = 0;
              stillReceiving = 1;
              waitUntil5 = millis();
              waitUntil1 = millis() + UPDATEINTERVAL;
            }
          }
        }
        else if (stillReceiving)
        {
          if (millis() > waitUntil5) stillReceiving = 0;
          else if (Serial1.available())
          {
            while(Serial1.available()) Serial1.read();
            waitUntil5 = millis() + WAITFORENDOFTRANSFER;
          }
        }
        else if (millis() > waitUntil4)
        {
          if (Serial1.available())
          {
            while (Serial1.available())
            {
              if (char(Serial1.read()) == '2')
              {
                while (!Serial1.available());
                if (char(Serial1.read()) == '.')
                {
                  while (!webServer(1, &ssid, &pass, feed));
                  return;
                }
              }
            }
          }
          else
          {
            Serial1.println("AT+CWLIF");
            waitUntil4 = millis() + CHECKFORCLIENTINTERVAL;
          }
        }
      }
    }
    if (downloadDone)
    {
      downloadDone = cnt = msgCnt = 0;
      while (feedBuf[cnt] && msgCnt++ < feed[feedNo].numOfMsgs)
      {
        cnt2 = 0;
        while (feedBuf[cnt])
        {
          if (feedBuf[cnt++] != feed[feedNo].startAddrMsg[0][cnt2++])
          {
            for (cnt2 = feed[feedNo].numOfMsgs - 1; cnt2 > 0; cnt2--) feed[feedNo].msgTime[cnt2] = feed[feedNo].msgTime[cnt2 - 1];
            feed[feedNo].msgTime[0] = 0;
            while (feedBuf[cnt++]);
            break;
          }
        }
      }
      cnt = 0;
      for (msgCnt = 0; msgCnt < feed[feedNo].numOfMsgs; msgCnt++)
      {
        feed[feedNo].startAddrMsg[msgCnt] = &feed[feedNo].feedStrings[cnt];
        while (feedBuf[cnt])
        {
          feed[feedNo].feedStrings[cnt] = feedBuf[cnt];
          cnt++;
        }
        feed[feedNo].feedStrings[cnt] = feedBuf[cnt];
        cnt++;
      }
    }
    if (millis() > waitUntil3)
    {
      if (++feedNo == NUMOFFEEDS) feedNo = 0;
      waitUntil3 = millis() + FEEDCHANGEINTERVAL;
    }
    for (msgCnt = 0; msgCnt < feed[feedNo].numOfMsgs; msgCnt++) if (feed[feedNo].msgTime[msgCnt] < feed[feedNo].msgTime[greatestDtMsgNo]) greatestDtMsgNo = msgCnt;
    pMessage[0] = pMessage[1] = feed[feedNo].startAddrMsg[greatestDtMsgNo];
    feed[feedNo].msgTime[greatestDtMsgNo] = millis();
  }
}

