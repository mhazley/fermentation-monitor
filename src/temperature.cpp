#include "temperature.h"

#include <math.h>
#include "spark_wiring.h"
#include "boardDefs.h"
#include <stdlib.h>

#define S_TEMP_A         "TEMP_A"
#define S_TEMP_B         "TEMP_B"

#define S_TEMP_UNKNOWN   "UNKNOWN"

TemperatureSensor::TemperatureSensor(TemperatureSensorRole _role, uint8_t _controlPin)
{
    this->role = _role;
    this->controlPin = _controlPin;
};

TemperatureSensorMap TemperatureSensor::initSensors()
{
    TemperatureSensorMap sensors;

    sensors[TEMP_A] = new TemperatureSensor(TEMP_A, IO_TEMP_A_PIN);

    return sensors;
};
TemperatureSensorMap TemperatureSensor::sensors = initSensors();

TemperatureSensorMap* TemperatureSensor::AllSensors()
{
    return &sensors;
}

TemperatureSensor* TemperatureSensor::GetSensor(TemperatureSensorRole role)
{
    return sensors[role];
}

float TemperatureSensor::getCelsius()
{
    float temperature;

    float vout = (float)analogRead(controlPin)/4095.0*3.3*1000;

    float resistance = (float)(vout * 10000) / (float)(3300 - vout);

    // Uses B-Parameter equation: 1/T = 1/T0 + 1/B*log(R/R0)
    temperature = resistance / 10000;     // (R/Ro)
    temperature = log(temperature);       // ln(R/Ro)
    temperature /= 3950;                  // 1/B * ln(R/Ro)
    temperature += 1.0 / (25 + 273.15);   // + (1/To)
    temperature = 1.0 / temperature;      // Invert
    temperature -= 273.15;                // convert to C

    value = temperature;

    return temperature;
};

const char* TemperatureSensor::roleString()
{
    return TemperatureSensor::RoleToString(this->role);
}

const char* TemperatureSensor::RoleToString(TemperatureSensorRole role)
{
    switch (role) {
        case TEMP_A:        return S_TEMP_A;      break;
    }

    return S_TEMP_UNKNOWN;
}
