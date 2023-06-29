/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/admin/Documents/CTD_2023/Personal_Projects/TV/src/TV.ino"
#include "basicLibrary.ino"
void setup();
void play(int index);
void remove(int index);
void editProgram(int index);
void loop();
#line 2 "/Users/admin/Documents/CTD_2023/Personal_Projects/TV/src/TV.ino"
LED ledGrouping[5];
LEDGroup LEDS(ledGrouping);
pin_t ledPins[5] = {D0, D1, D2, D3, D4};
LED notice;
Button accept;
Button decline;
Potentiometer p1;
double ledSelected;
bool programs[5][5][5];
bool emptyProgram[5][5];
bool programState[5] = {false, false, false, false, false};
void setup()
{
  notice.initialize(D11);
  LEDS.initialize(ledPins);
  accept.initialize(D10);
  decline.initialize(D9);
  p1.initialize(A4);
}

bool playing = false;
void play(int index)
{
  playing = true;
  while (playing)
  {
    update();
    playing = !accept.clicked;
    for (bool *frame : programs[index])
    {
      LEDS.allOff();
      for (int ledIndex = 0; ledIndex < 5; ledIndex++)
      {
        LEDS.lightsInGroup[ledIndex].vTog(frame[ledIndex]);
      }
      delay(200);
    }
  }
}

void remove(int index)
{
  programState[index] = false;
  for (bool *frame : programs[index])
  {
    for (int light = 0; light < 5; light++)
    {
      frame[light] = false;
    }
  }
}

bool editing = false;
int editingIndex = 0;
int frameIndex = 0;
void editProgram(int index)
{
  LEDS.allOff();
  editing = true;
  editingIndex = 0;
  frameIndex = 0;
  while (editing)
  {
    update();
    if (editingIndex == 5)
    {
      notice.on();
      if (accept.clicked)
      {
        LEDS.allOff();
        editingIndex = 0;
        frameIndex++;
      }
      if (decline.clicked)
      {
        LEDS.allOff();
        editingIndex = 0;
      }
    }
    else
    {
      notice.off();
      if (accept.clicked)
      {
        LEDS.oneOn(editingIndex);
        programs[index][frameIndex][editingIndex] = true;
        editingIndex++;
      }
      if (decline.clicked)
      {
        editingIndex++;
      }
    }
    if (frameIndex == 5)
    {
      editing = false;
    }
  }
  programState[index] = true;
}

void loop()
{
  update();
  ledSelected = map(p1.val, 0, 4095, 0, 5);
  LEDS.allOff();
  LEDS.oneOn(ledSelected);
  bool fileFull = programState[(int)ledSelected];
  notice.vTog(fileFull);

  if (fileFull && accept.clicked)
  {
    play((int)ledSelected);
  }
  if (fileFull && decline.clicked)
  {
    remove((int)ledSelected);
  }
  if (!fileFull && accept.clicked)
  {
    editProgram((int)ledSelected);
  }
}