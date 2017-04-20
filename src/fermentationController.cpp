#include "fermentationController.h"
#include "PublishController.h"
#include "temperature.h"
#include "fermenter.h"
#include "boardDefs.h"

using namespace std;

#define HYSTERESIS            ( 1 )

#define ACTIVATE_COOLING()    ( digitalWrite(IO_FREEZER_PIN, HIGH) )
#define DEACTIVATE_COOLING()  ( digitalWrite(IO_FREEZER_PIN, LOW) )
#define ACTIVATE_HEATING()    ( digitalWrite(IO_HEATER_PIN, HIGH) )
#define DEACTIVATE_HEATING()  ( digitalWrite(IO_HEATER_PIN, LOW) )

FermentationController::FermentationController()
{
    pinMode(IO_FREEZER_PIN, OUTPUT);
    pinMode(IO_HEATER_PIN, OUTPUT);

    DEACTIVATE_HEATING();
    DEACTIVATE_COOLING();

    setHeatCoolPublishData( false, false );
    setSetPointPublishData();

}

void FermentationController::start(double setPoint)
{
    if(didStart == false)
    {
        this->setPoint = setPoint;
        didStart = true;
    }
    else
    {
        this->setPoint = setPoint;
    }
}

void FermentationController::stop()
{
    if (didStart == true)
    {
        didStart = false;
        DEACTIVATE_HEATING();
        DEACTIVATE_COOLING();
    }
}

bool FermentationController::isStarted()
{
    return didStart;
}

void FermentationController::process()
{
    if (didStart)
    {
        if      ( ( TemperatureSensor::AllSensors()->at( TEMP_A )->getCelsius() ) > (setPoint + HYSTERESIS ) )
        {
            DEACTIVATE_HEATING();
            ACTIVATE_COOLING();

            setHeatCoolPublishData( false, true );
        }
        else if ( ( TemperatureSensor::AllSensors()->at( TEMP_A )->getCelsius() ) > setPoint )
        {
            DEACTIVATE_HEATING();
            DEACTIVATE_COOLING();

            setHeatCoolPublishData( false, false );
        }
        else if ( ( TemperatureSensor::AllSensors()->at( TEMP_A )->getCelsius() ) < (setPoint - HYSTERESIS ) )
        {
            ACTIVATE_HEATING();
            DEACTIVATE_COOLING();

            setHeatCoolPublishData( true, false );
        }
        else if ( ( TemperatureSensor::AllSensors()->at( TEMP_A )->getCelsius() ) < setPoint )
        {
            DEACTIVATE_HEATING();
            DEACTIVATE_COOLING();

            setHeatCoolPublishData( false, false );
        }
    }
}

void FermentationController::printDescription()
{
    if (didStart)
    {
        Serial.print("Fermenting with setpoint: ");
        Serial.println(this->setPoint);
    }
}

void FermentationController::setHeatCoolPublishData(bool heat, bool cool)
{
    String sHeat = heat ? String(1) : String(0);
    String sCool = heat ? String(1) : String(0);

    getPublishController()->setValue(PUBLISH_TYPE_HEATING, sHeat);
    getPublishController()->setValue(PUBLISH_TYPE_COOLING, sCool);
}

void FermentationController::setSetPointPublishData()
{
    String sSetPoint = String((int)(setPoint * 100));

    getPublishController()->setValue(PUBLISH_TYPE_SETPOINT, sSetPoint);
}
