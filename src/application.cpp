#include "fermenter.h"
#include "temperature.h"
#include "version.h"
#include "PublishController.h"
#include "simpleTimer.h"

PublishController* publishController;

#define STRINGIFY(val)  #val
#define TOSTRING(val)   STRINGIFY(val)

unsigned long lastSerialTime  = 0UL;
unsigned long serialDelay     = 1000UL;
bool          serialPrintLoop = false;

void setup()
{
    Serial.begin(115200);

    publishController = new PublishController();
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

    if (serialPrintLoop)
    {
        lastSerialTime = millis();
        serialPrintLoop = false;
    }

    /* iterate timers to process them */
    for (std::vector<simpleTimer*>::size_type i = 0; i != simpleTimer::timers.size(); i++) {
      (*simpleTimer::getTimers())[i]->process();
    }
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

bool checkCanSerialPrint()
{
    return millis() > lastSerialTime + serialDelay;
}

void printSerial() {

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
}
