#include "Particle.h"
#include "DS18.h"

#define INVALID_TEMP    -99.9

class TemperatureSensor
{
    private:
        float temperature = INVALID_TEMP;
        uint8_t* addr;
        DS18* ds18_driver; 
        bool has_converted = false;

    public:
        TemperatureSensor( DS18* ds18_driver, uint8_t* addr );
        float get_temperature( void );
        void sample( void );
};