#include "Arduino.h"

typedef struct
{
  String url;
  String command[3];
  char *ack[3];
  char feedStrings[NUMOFMESSAGES * MAXFEEDSIZE];
  char feedTitle[MAXTITLELEN];
  char dispTitle[2][40];
  char *startAddrMsg[NUMOFMESSAGES];
  byte numOfMsgs;
  unsigned long msgTime[NUMOFMESSAGES] = {0};
}feed_type;

