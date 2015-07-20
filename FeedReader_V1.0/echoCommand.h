boolean echoCommand(String cmd, char *ack, unsigned long timeToWait)
{
  Serial1.println(cmd);
  if (!findStringInFifo(ack, timeToWait)) return 0;
  return 1;
}
