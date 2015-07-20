byte makeCommands(String *command, char **ack, String url)
{
int chrNo;

  command[2] = "GET ";
  command[2] += url;
  command[2] += " HTTP/1.0\r\n\r\n";
  ack[2] = "OK\r\n";
  command[1] = "AT+CIPSEND=0,";
  command[1] += command[2].length() + 2;
  ack[1] = ">";
  command[0] = "AT+CIPSTART=0,\"TCP\",\"";
  chrNo = url.indexOf("//");
  if (chrNo < 0) return 0;
  url.remove(0, chrNo + 2);
  chrNo = url.indexOf('/');
  if (chrNo < 0) return 0;
  url.remove(chrNo);
  command[0] += url;
  command[0] += "\", 80";
  ack[0] = "Linked\r\n";
  return 1;
}
