byte writeStringToEeprom(int address, String *string)
{
const char *pString;

  pString = string->c_str();
  do
  {
    if (address > 1024) return 0;
    EEPROM.update(address++, *pString);
  }
  while (*pString++);
  return 1;
}
