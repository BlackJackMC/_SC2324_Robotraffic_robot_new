#include <Arduino.h>
#include "api.h"
#include "hall.h"
#include "motor.h"

void setup()
{
  api::setup();
  hall::setup([](){});
}

void loop()
{
}