#include "fermentation.h"
#include "board_defs.h"

#define COOLING_HYSTERESIS    ( 0.5 )
#define HEATING_HYSTERESIS    ( 0.1 )
#define SAISON_UPPER_LIMIT    ( 26 )

#define COOLING(state) \
    do { \
        digitalWrite(IO_FRIDGE_PIN, state); \
        digitalWrite(IO_FRIDGE_LED_PIN, state); \
    } while (0)

#define HEATING(state) \
    do { \
        digitalWrite(IO_HEATER_PIN, state); \
        digitalWrite(IO_HEATER_LED_PIN, state); \
    } while (0)

Fermentation::Fermentation()
{
    this->mode = IDLE;

    pinMode( IO_FRIDGE_PIN, OUTPUT );
    pinMode( IO_FRIDGE_LED_PIN, OUTPUT );
    pinMode( IO_HEATER_PIN, OUTPUT );
    pinMode( IO_HEATER_LED_PIN, OUTPUT );

    COOLING( false );
    HEATING( false );
}

void Fermentation::stop()
{
    this->mode = IDLE;
    COOLING( false );
    HEATING( false );
}

void Fermentation::process( float beer_temp )
{
    if( this->mode != IDLE )
    {
        if      ( beer_temp > (this->setpoint + COOLING_HYSTERESIS ) )
        {
            if( ( this->mode != RUNNING_S ) || ( beer_temp > SAISON_UPPER_LIMIT) )
            {
                HEATING( false );
                COOLING( true );

                heating = false;
                cooling = true;
            }
            else if( ( this->mode == RUNNING_S ) && ( beer_temp < SAISON_UPPER_LIMIT) )
            {
                HEATING( false );
                COOLING( false );
            
                heating = false;
                cooling = false;
            }
        }
        else if ( ( beer_temp > this->setpoint ) && ( beer_temp <= ( this->setpoint + COOLING_HYSTERESIS ) ) )
        {
            HEATING( false );
            COOLING( false );
            
            heating = false;
            cooling = false;
        }
        else if ( beer_temp < (this->setpoint - HEATING_HYSTERESIS ) ) 
        {
            HEATING( true );
            COOLING( false );

            heating = true;
            cooling = false;
        }
        else if ( ( beer_temp < this->setpoint ) && ( beer_temp >= ( this->setpoint - HEATING_HYSTERESIS ) ) )
        {
            HEATING( false );
            COOLING( false );

            heating = false;
            cooling = false;
        }
    }
}

void Fermentation::get_state( bool *heating, bool *cooling, float *setpoint, ferm_mode_t *mode )
{
    *heating = this->heating;
    *cooling = this->cooling;
    *setpoint = this->setpoint;
    *mode = this->mode;
}

void Fermentation::start( float setpoint, ferm_mode_t mode )
{
    this->setpoint = setpoint;
    this->mode = mode;
}