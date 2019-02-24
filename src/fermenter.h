#ifndef FERMENTER_H
#define FERMENTER_H

#include "application.h"
#include "PublishController.h"

#define ONBOARD_LED_PIN             D7
#define DEFAULT_PUBLISH_TTL         60

#define DEBUG_PRINT(debug, newLine) do { \
  if (checkCanSerialPrint()) {           \
    if (newLine)                         \
      Serial.println(debug);             \
    else                                 \
      Serial.print(debug);               \
  }                                      \
} while (0)

void setTemperaturePublishData();
int setSetpoint(String setpoint);
int setCoolingSetpoint(String setpoint);
int setPublishable(String publishable);
bool checkCanSerialPrint();
void printSerial();
PublishController* getPublishController();


#endif // FERMENTER_H
