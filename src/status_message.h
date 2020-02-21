#ifndef STATUS_MESSAGE_H
#define STATUS_MESSAGE_H

#include "Particle.h"
#include "Arduino.h"
#include "ArduinoJson.h"
#include "fermentation.h"

class StatusMessage
{

private:
    DynamicJsonDocument json_buffer;

public:
    StatusMessage() : json_buffer(1024) {}
    String serialize_message( void );
    void init( float out_temp, float amb_temp, float tw_temp, bool heating, bool cooling, float setpoint, ferm_mode_t mode );
};

#endif // STATUS_MESSAGE_H