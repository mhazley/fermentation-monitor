#ifndef FERMENTATION_H
#define FERMENTATION_H

#include "application.h"

typedef enum {
    IDLE = 0,
    RUNNING = 1,
    RUNNING_S = 2
} ferm_mode_t;

class Fermentation
{

private:
    ferm_mode_t   mode = IDLE;
    double        setpoint = 0.0;
    bool          heating = false;
    bool          cooling = false;
    bool          stable = false;

public:
    void   process( float beer_temp );
    void   start( float setpoint, ferm_mode_t mode );
    void   stop();
    bool   is_started();

    Fermentation();
    void get_state( bool *heating, bool *cooling, float *setpoint, ferm_mode_t *mode );
};

#endif