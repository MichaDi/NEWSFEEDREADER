byte readStringFromEeprom(int address, String *string, int maxLen)
{
char ch;

  *string = "";
  maxLen += address;
  for (; address < maxLen; address++)
  {
    ch = EEPROM.read(address);
    if (!ch) return 1;
    *string += ch;
  }
  return 0;
}
