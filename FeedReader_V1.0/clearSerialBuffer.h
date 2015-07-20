void clearSerialBuffer(unsigned long wait)
{
  delay(wait);
  while (Serial1.available())
  {
    while (Serial1.available()) Serial1.read();
    delay(wait);
  }
}
