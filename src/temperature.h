#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "Particle.h"
#include "DS18.h"

#define INVALID_TEMP    -99.9

class TemperatureSensor
{
    private:
        void update_filter( void );

        float _temperature[4];
        float _filter[4];
        uint8_t* addr;
        DS18* ds18_driver; 
        bool has_converted = false;
        bool first_pass = true;

    public:
        TemperatureSensor( DS18* ds18_driver, uint8_t* addr );
        float get_temperature( void );
        void sample( void );
};

#endif // TEMPERATURE_H