#include "basicLibrary.ino"
LED ledGrouping[5];
LEDGroup LEDS(ledGrouping);
pin_t ledPins[5] = {D0, D1, D2, D3, D4};
LED noticePin;
Button accept;
Button decline;
Potentiometer p1;
void setup()
{
  noticePin.initialize(D11);
  LEDS.initialize(ledPins);
  accept.initialize(D10);
  decline.initialize(D9);
}
void loop()
{
  update();
}