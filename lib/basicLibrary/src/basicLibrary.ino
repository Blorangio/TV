SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);
#include <functional>
#include <vector>
typedef std::function<void()> VoidFunc;
typedef std::vector<VoidFunc> FuncVector;
FuncVector updates;
class LED
{
public:
  bool val = false;
  pin_t pin;
  void initialize(pin_t PIN)
  {
    pin = PIN;
    pinMode(pin, OUTPUT);
  }
  void on()
  {
    val = true;
    digitalWrite(pin, HIGH);
  }
  void off()
  {
    val = false;
    digitalWrite(pin, LOW);
  }
  void vTog(bool iVal)
  {
    val = iVal;
    if (val)
    {
      digitalWrite(pin, HIGH);
    }
    else
    {
      digitalWrite(pin, LOW);
    }
  }
};
class Button
{
public:
  bool bDown = false;
  bool val = false;
  bool clicked = false;
  pin_t pin;
  void initialize(pin_t PIN)
  {
    updates.push_back(std::bind(&Button::update, this));
    pin = PIN;
    pinMode(pin, INPUT);
  }
  bool once = true;
  void update()
  {
    clicked = false;
    pinMode(pin, INPUT);
    if (digitalRead(pin) == HIGH && !bDown)
    {
      bDown = true;
      if (once)
      {
        clicked = true;
        once = false;
      }
      val = !val;
    }
    if (digitalRead(pin) == LOW)
    {
      bDown = false;
      once = true;
    }
  }
};
class Potentiometer
{
public:
  pin_t pin;
  void initialize(pin_t PIN)
  {
    pin = PIN;
    pinMode(pin, INPUT);
    updates.push_back(std::bind(&Potentiometer::update, this));
  }
  int val = analogRead(pin);
  void update()
  {
    val = analogRead(pin);
  }
};

class LEDGroup
{
public:
  LED *lightsInGroup = {};
  pin_t *pins = {};
  LEDGroup(LED *array)
  {
    lightsInGroup = array;
  }
  void set(LED setLights[sizeof(lightsInGroup) + 1])
  {
    lightsInGroup = setLights;
  }
  void oneOn(int index)
  {
    lightsInGroup[index].on();
  }
  void oneOff(int index)
  {
    lightsInGroup[index].off();
  }
  void initialize(pin_t *PINS)
  {
    pins = PINS;
    for (uint i = 0; i < sizeof(lightsInGroup) + 1; i++)
    {
      lightsInGroup[i].initialize(pins[i]);
    }
  }
  void allOff()
  {
    for (uint i = 0; i < sizeof(lightsInGroup) + 1; i++)
    {
      lightsInGroup[i].off();
    }
  }

  void allOn()
  {
    for (uint i = 0; i < sizeof(lightsInGroup) + 1; i++)
    {
      lightsInGroup[i].on();
    }
  }

  void onTo(int index)
  {
    for (int i = 0; i < index; i++)
    {
      lightsInGroup[i].on();
    }
  }

  void offTo(int index)
  {
    for (int i = 0; i < index; i++)
    {
      lightsInGroup[i].off();
    }
  }
};

void update()
{
  for (uint i = 0; i < updates.size(); i++)
  {
    updates[i]();
  }
}