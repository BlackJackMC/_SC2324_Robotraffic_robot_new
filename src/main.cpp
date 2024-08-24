#include <Arduino.h>
#include "net.h"
#include "api.h"
#include "mqtt.h"
#include "hall.h"
#include "motor.h"
#include "line.h"

void setup()
{
  Serial.begin(9600);
  while(!Serial); // Wait for serial

  net::setup();
  api::setup(net::wifi);
  mqtt::setup(net::wifi);
  hall::setup([](){});
  motor::setup();
  line::setup();
}

void loop()
{
}