#ifndef STATUS_MESSAGE_H
#define STATUS_MESSAGE_H

#include "Particle.h"
#include "Arduino.h"
#include "ArduinoJson.h"

class StatusMessage
{

private:
    DynamicJsonDocument json_buffer;

public:
    StatusMessage() : json_buffer(1024) {}
    String serialize_message( void );
    void init( float out_temp, float amb_temp, float tw_temp );
};

#endif // STATUS_MESSAGE_H