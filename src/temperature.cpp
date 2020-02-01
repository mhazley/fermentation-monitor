#include "temperature.h"

TemperatureSensor::TemperatureSensor( DS18* ds18_driver, uint8_t* addr )
{
    this->ds18_driver = ds18_driver;
    this-> addr = addr;
}

void TemperatureSensor::sample( void )
{
    if( this->has_converted )
    {
        if( ds18_driver->read( addr ) )
        {
            this->temperature = ds18_driver->celsius();
        }
    }

    ds18_driver->start_conv( addr );
    this->has_converted = true;
}

float TemperatureSensor:: get_temperature( void )
{
    return this->temperature;
}