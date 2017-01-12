#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "MapHack.h"

enum TemperatureSensorRole {
  TEMP_A      = 1u << 0,
  TEMP_B      = 1u << 1
};

class TemperatureSensor
{
    private:
        typedef std::map<TemperatureSensorRole, TemperatureSensor*> TemperatureSensorMap;
        static TemperatureSensorMap sensors;
        static TemperatureSensorMap initSensors(void);

        float value = 0;

    public:
        static TemperatureSensorMap* AllSensors   ();
        static TemperatureSensor*    GetSensor    (TemperatureSensorRole role);
        static const char*           RoleToString (TemperatureSensorRole role);

        TemperatureSensorRole role;
        uint8_t               controlPin;
        char                  id[2];

        TemperatureSensor(TemperatureSensorRole, uint8_t);

        const char* roleString();

        float getCelsius();
};

typedef std::map<TemperatureSensorRole, TemperatureSensor*> TemperatureSensorMap;

#endif // TEMPERATURE_H
