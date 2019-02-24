#ifndef FERMENTATION_CONTROLLER_H
#define FERMENTATION_CONTROLLER_H

#include "application.h"

typedef enum {
    NOT_RUNNING = 0,
    RUNNING = 1,
    COOLING = 2
} ferm_mode_t;

class FermentationController
{

private:
    ferm_mode_t   mode = NOT_RUNNING;
    double        setPoint = 0.0;
    bool          heating = false;
    bool          cooling = false;
    unsigned long lastProcessTime  = 0UL;

    void   setHeatCoolPublishData(bool heat, bool cool);
    void   setSetPointPublishData();
    void   setRunningPublishData();

public:
    void   process();
    void   start(ferm_mode_t mode, double setPoint);
    void   stop();
    void   printDescription();
    bool   isStarted();

    FermentationController();
};

#endif
