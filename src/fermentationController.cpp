#include "fermentationController.h"
#include "PublishController.h"
#include "temperature.h"
#include "fermenter.h"
#include "boardDefs.h"

using namespace std;

#define HYSTERESIS            ( 1.0 )
#define PROCESS_DELAY         ( 1000 )
#define SAISON                ( 1 )

#define ACTIVATE_COOLING()    ( digitalWrite(IO_FREEZER_PIN, HIGH) )
#define DEACTIVATE_COOLING()  ( digitalWrite(IO_FREEZER_PIN, LOW) )
#define ACTIVATE_HEATING()    ( digitalWrite(IO_HEATER_PIN, HIGH) )
#define DEACTIVATE_HEATING()  ( digitalWrite(IO_HEATER_PIN, LOW) )

FermentationController::FermentationController()
{
    pinMode(IO_FREEZER_PIN, OUTPUT);
    pinMode(IO_HEATER_PIN, OUTPUT);

    this->mode = NOT_RUNNING;

    DEACTIVATE_HEATING();
    DEACTIVATE_COOLING();

    setHeatCoolPublishData( false, false );
    setSetPointPublishData();
    setRunningPublishData();
}

void FermentationController::start(ferm_mode_t mode, double setPoint)
{
    this->setPoint = setPoint;
    this->mode = mode;

    setSetPointPublishData();
    setRunningPublishData();
}

void FermentationController::stop()
{
    this->mode = NOT_RUNNING;
    DEACTIVATE_HEATING();
    DEACTIVATE_COOLING();
    setHeatCoolPublishData( false, false );
    setSetPointPublishData();
    setRunningPublishData();
}

bool FermentationController::isStarted()
{
    return !(this->mode == NOT_RUNNING);
}

void FermentationController::process()
{
    if( ( this->lastProcessTime + PROCESS_DELAY ) < millis() )
    {
        if( this->mode != NOT_RUNNING )
        {
#if SAISON==1
            if      ( ( TemperatureSensor::AllSensors()->at( TEMP_A )->getTemperature() ) > (this->setPoint + HYSTERESIS + 2 ) )
#else
            if      ( ( TemperatureSensor::AllSensors()->at( TEMP_A )->getTemperature() ) > (this->setPoint + HYSTERESIS ) )
#endif
            {
                Serial.println( "Cooling" );
                DEACTIVATE_HEATING();
                ACTIVATE_COOLING();

                setHeatCoolPublishData( false, true );
            }
            else if ( ( ( TemperatureSensor::AllSensors()->at( TEMP_A )->getTemperature() ) > this->setPoint ) && ( ( TemperatureSensor::AllSensors()->at( TEMP_A )->getTemperature() ) < ( this->setPoint + ( HYSTERESIS / 2 ) ) ) )
            {
                Serial.println( "Stable" );
                DEACTIVATE_HEATING();
                DEACTIVATE_COOLING();

                setHeatCoolPublishData( false, false );
            }
            else if ( ( TemperatureSensor::AllSensors()->at( TEMP_A )->getTemperature() ) < (this->setPoint - HYSTERESIS ) ) 
            {
                
                if( this->mode == RUNNING )
                {
                    // Only allow heating if in running mode
                    Serial.println( "Heating" );
                    ACTIVATE_HEATING();
                }
                DEACTIVATE_COOLING();

                setHeatCoolPublishData( true, false );
            }
            else if ( ( ( TemperatureSensor::AllSensors()->at( TEMP_A )->getTemperature() ) < this->setPoint ) && ( ( TemperatureSensor::AllSensors()->at( TEMP_A )->getTemperature() ) > ( this->setPoint - ( HYSTERESIS / 2 ) ) ) )
            {
                Serial.println( "Stable" );
                DEACTIVATE_HEATING();
                DEACTIVATE_COOLING();

                setHeatCoolPublishData( false, false );
            }
        }

        this->lastProcessTime = millis();
    }
}

void FermentationController::printDescription()
{
    if (this->mode != NOT_RUNNING)
    {
        Serial.println();
        Serial.print("Fermenting with setpoint: ");
        Serial.println(this->setPoint);
    }
}

void FermentationController::setHeatCoolPublishData(bool heat, bool cool)
{
    String sHeat = heat ? String(1) : String(0);
    String sCool = cool ? String(1) : String(0);

    getPublishController()->setValue(PUBLISH_TYPE_HEATING, sHeat);
    getPublishController()->setValue(PUBLISH_TYPE_COOLING, sCool);
}

void FermentationController::setSetPointPublishData()
{
    String sSetPoint = String((int)(this->setPoint * 100));

    getPublishController()->setValue(PUBLISH_TYPE_SETPOINT, sSetPoint);
}

void FermentationController::setRunningPublishData()
{
    String sRunning = "NOT RUNNING";
    if( this->mode != NOT_RUNNING )
    {
        if( this->mode == RUNNING )
        {
            sRunning = "RUNNING";
        }
        else if( this->mode == COOLING )
        {
            sRunning = "COOLING";
        }
    }

    getPublishController()->setValue(PUBLISH_TYPE_RUNNING, sRunning);
}
