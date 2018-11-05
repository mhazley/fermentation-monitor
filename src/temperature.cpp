#include "temperature.h"

#include <math.h>
#include "spark_wiring.h"
#include "boardDefs.h"
#include <stdlib.h>

#define S_TEMP_A                ( "TEMP_A" )
#define S_TEMP_B                ( "TEMP_B" )
#define S_TEMP_UNKNOWN          ( "UNKNOWN" )
#define DEFAULT_SAMPLE_DELAY    ( 1000UL )

TemperatureSensor::TemperatureSensor(TemperatureSensorRole _role, uint8_t _controlPin)
{
    this->role = _role;
    this->controlPin = _controlPin;
    
    this->sampleTimer = new CallbackTimer<TemperatureSensor>(this, &TemperatureSensor::callback, true);
    this->sampleTimer->setDuration(DEFAULT_SAMPLE_DELAY, MILLISECONDS);
    this->avg = new Average();
    
    this->sampleTimer->start();
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

void TemperatureSensor::callback()
{
    this->avg->addValue( this->getCelsius() );
}

double TemperatureSensor::getTemperature()
{
    return this->avg->getAverage();
}

double TemperatureSensor::getCelsius()
{
    double temperature;

    double vout = (double)analogRead(controlPin)/4095.0*3.3*1000;

    double resistance = (double)(vout * 10000) / (double)(3300 - vout);

    // Uses B-Parameter equation: 1/T = 1/T0 + 1/B*log(R/R0)
    temperature = resistance / 10000;     // (R/Ro)
    temperature = log(temperature);       // ln(R/Ro)
    temperature /= 3950;                  // 1/B * ln(R/Ro)
    temperature += 1.0 / (25 + 273.15);   // + (1/To)
    temperature = 1.0 / temperature;      // Invert
    temperature -= 273.15;                // convert to C

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
