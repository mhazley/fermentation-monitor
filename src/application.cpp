#include "fermenter.h"
#include "temperature.h"
#include "version.h"
#include "PublishController.h"
#include "simpleTimer.h"
#include "fermentationController.h"

#define STRINGIFY(val)      #val
#define TOSTRING(val)       STRINGIFY(val)
#define NO_ROUTE_MATCHES    -1
#define ROUTE_OK            0

PublishController*          publishController;
FermentationController*     fermentationController;

unsigned long lastSerialTime  = 0UL;
unsigned long serialDelay     = 2000UL;
bool          serialPrintLoop = false;

void setup()
{
    Serial.begin(115200);

    Particle.function("route", functionRouter);

    publishController      = new PublishController();
    fermentationController = new FermentationController();

    setTemperaturePublishData();
    publishController->start();
}

void loop()
{
    if (checkCanSerialPrint())
    {
        serialPrintLoop = true;
        printSerial();
        setTemperaturePublishData();
    }

    fermentationController->process();

    if (serialPrintLoop)
    {
        lastSerialTime = millis();
        serialPrintLoop = false;
    }

    /* iterate timers to process them */
    for (std::vector<simpleTimer*>::size_type i = 0; i != simpleTimer::timers.size(); i++)
    {
        (*simpleTimer::getTimers())[i]->process();
    }
}

PublishController* getPublishController()
{
    return publishController;
}

void setTemperaturePublishData()
{
    TemperatureSensorMap* sensors = TemperatureSensor::AllSensors();

    for (TemperatureSensorMap::const_iterator iterator = sensors->begin();
         iterator != sensors->end();
         iterator++)
    {
        TemperatureSensor* sensor = iterator->second;

        String temperature = String((int)(sensor->getCelsius() * 100));

        publishController->setValue(PUBLISH_TYPE_TEMPERATURE, temperature);
    }
}

/**
 * Takes a string, parses and routes it to the
 * responsible function.
 *
 * Current functions and their parameter layouts:
 *
 * setpoint,23.4
 */
int functionRouter(String params)
{
    char** args;
    char*  pt;
    char*  argString = new char[params.length() + 1];
    char*  argString_copy = new char[params.length() + 1];
    int    argCount = 0;
    int    index = 0;
    int    returnValue = -1;

    strcpy( argString, params.c_str() );
    strcpy( argString_copy, params.c_str() );

    pt = strtok(argString, ",");

    // Count arguments first for malloc
    while (pt != NULL)
    {
        argCount++;
        pt = strtok(NULL, ",");
    }

    args = (char**) malloc(argCount * sizeof(char*));

    if (args != NULL)
    {
        pt = strtok(argString_copy, ",");

        while (pt != NULL)
        {
            args[index++] = pt;
            pt = strtok(NULL, ",");
        }

        Serial.println("===> Routing");
        for ( index = 0; index < argCount; index++ )
        {
            Serial.print( "\t" );
            Serial.println( args[index] );
        }

        if ( ( strcmp("setpoint", args[0]) == 0 ) && argCount == 2 )
        {
            Serial.println( "setSetpoint()!!" );
            setSetpoint( args[1] );
            returnValue = ROUTE_OK;
        }

        // Free malloc'd memory
        for ( index = 0; index < argCount; index++ )
        {
            free( args[index] );
        }
    }

    free( args );
    delete argString;
    delete argString_copy;

    return returnValue;
}

void setSetpoint(char* temp)
{
    double setPoint = atof(temp);

    if (setPoint == 0)
    {
        fermentationController->stop();
    }
    else
    {
        fermentationController->start(setPoint);
    }
}

bool checkCanSerialPrint()
{
    return millis() > lastSerialTime + serialDelay;
}

void printSerial()
{

    Serial.write(27);
    Serial.print("[2J");
    Serial.write(27);
    Serial.print("[H");

    Serial.print("Fermenter - v");Serial.print(FW_VERSION_STR);
#ifdef BUILD_SHA
    Serial.print(" (Build: ");
    Serial.print(TOSTRING(BUILD_SHA));
#ifdef DIRTY_TREE
    Serial.print("-dirty");
#endif
    Serial.print(", Particle: ");
    Serial.print(TOSTRING(PARTICLE_SHA));
    Serial.println(")");
#else
    Serial.println();
#endif

    Serial.println("===> Temperatures");
    TemperatureSensorMap* allSensors = TemperatureSensor::AllSensors();
    for (TemperatureSensorMap::const_iterator iterator = allSensors->begin();
         iterator != allSensors->end();
         iterator++)
    {
        TemperatureSensor *sensor = iterator->second;
        Serial.print("\t");
        Serial.print(sensor->roleString());
        Serial.print(" -> ");
        Serial.print(sensor->getCelsius());
        Serial.println("°C");
    }

    if( fermentationController->isStarted() ) fermentationController->printDescription();
}
