#ifndef FERMENTATION_CONTROLLER_H
#define FERMENTATION_CONTROLLER_H

#include "application.h"

class FermentationController
{

private:
    bool   didStart = false;
    double setPoint = 0.0;
    bool   heating = false;
    bool   cooling = false;
    void   setHeatCoolPublishData(bool heat, bool cool);
    void   setSetPointPublishData();

public:
    void   process();
    void   start(double setPoint);
    void   stop();
    void   printDescription();
    bool   isStarted();

    FermentationController();
};

#endif
