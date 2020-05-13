#include "temperature.h"

TemperatureSensor::TemperatureSensor( DS18* ds18_driver, uint8_t* addr )
{
    this->ds18_driver = ds18_driver;
    this-> addr = addr;
}

void TemperatureSensor::sample( void )
{
    if( has_converted )
    {
        if( ds18_driver->read( addr ) )
        {
            if( first_pass )
            {
                _temperature[0] = ds18_driver->celsius();
                for (int i = 3; i > 0; i--) { _temperature[i] = _temperature[0]; }
                first_pass = false;
            }
            else
            {
                for (int i = 3; i > 0; i--) { _temperature[i] = _temperature[i - 1]; }
                _temperature[0] = ds18_driver->celsius();
                update_filter();
            }
            
        }
    }

    ds18_driver->start_conv( addr );
    has_converted = true;
}

void TemperatureSensor::update_filter( void )
{
    for (int i = 3; i > 0; i--) { _filter[i] = _filter[i - 1]; }

    _filter[0] = (_temperature[3] + _temperature[0] + 3 * (_temperature[2] + _temperature[1]))/1.092799972e+03
                      + (0.6600489526 * _filter[3]) + (-2.2533982563 * _filter[2]) + (2.5860286592 * _filter[1]);
}

float TemperatureSensor::get_temperature( void )
{
    return _filter[0];
}