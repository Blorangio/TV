SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);
#include "basicLibrary.ino"
#define potentiometer A4
#define returnFalseButton D9
#define returnTrueButton D10
#define noticeLight D11
LED ledGrouping[5];
LEDGroup LEDS(ledGrouping);
pin_t ledPins[5] = {D0, D1, D2, D3, D4}; // Pins for the LEDs that make up the screen
LED notice;
Button returnTrue;
Button returnFalse;
Potentiometer potentiometerSelecter;

double ledPotentiometerSelected;
bool animations[5][5][5];
bool isAnimationFileFull[5] = {false, false, false, false, false};
int ledIndex = 0;
int frameIndex = 0;
bool isFileFull;

void setup()
{
  notice.initialize(noticeLight);
  LEDS.initialize(ledPins);
  returnTrue.initialize(returnTrueButton);
  returnFalse.initialize(returnFalseButton);
  potentiometerSelecter.initialize(potentiometer);
}

void loop()
{
  update();
  ledPotentiometerSelected = map(potentiometerSelecter.val, 0, 4095, 0, 5);
  LEDS.allOff();
  LEDS.oneOn(ledPotentiometerSelected);
  isFileFull = isAnimationFileFull[(int)ledPotentiometerSelected];
  notice.val = isFileFull;
  if (isFileFull)
  {
    if (returnFalse.clicked)
    {
      remove((int)ledPotentiometerSelected);
    }
    if (returnTrue.clicked)
    {
      play((int)ledPotentiometerSelected);
    }
  }
  else if (returnTrue.clicked)
  {
    editProgram((int)ledPotentiometerSelected);
  }
}

void editProgram(int index)
{
  LEDS.allOff();
  ledIndex = 0;
  frameIndex = 0;
  while (frameIndex < 5)
  {
    update();
    if (ledIndex == 5)
    {
      notice.val = true;
      if (returnTrue.clicked || returnFalse.clicked)
      {
        LEDS.allOff();
        ledIndex = 0;
        notice.val = false;
        if (returnTrue.clicked)
        {
          frameIndex++;
        }
      }
    }
    else
    {
      if (returnTrue.clicked || returnFalse.clicked)
      {
        if (returnTrue.clicked)
        {
          LEDS.oneOn(ledIndex);
          animations[index][frameIndex][ledIndex] = true;
        }
        ledIndex++;
      }
    }
  }
  isAnimationFileFull[index] = true;
}

void play(int index)
{
  notice.val = false;
  returnTrue.bDown = false;
  while (!returnTrue.bDown)
  {
    for (bool *frame : animations[index])
    {
      update();
      LEDS.allOff();
      for (int ledIndex = 0; ledIndex < 5; ledIndex++)
      {
        LEDS.lightsInGroup[ledIndex].val = frame[ledIndex];
      }
      delay(200);
    }
  }
}

void remove(int index)
{
  isAnimationFileFull[index] = false;
  for (bool *frame : animations[index])
  {
    for (int light = 0; light < 5; light++)
    {
      frame[light] = false;
    }
  }
}