#include "simpleTimer.h"

std::vector<simpleTimer*> simpleTimer::timers;

simpleTimer::simpleTimer(bool repeatTimer)
{
    this->repeatTimer = repeatTimer;
    simpleTimer::timerProcessingSet(this, true);
}

simpleTimer::~simpleTimer()
{
    simpleTimer::timerProcessingSet(this, false);
}

void simpleTimer::setDuration(unsigned long duration, TimeType type)
{
    /* this can be called to reset the timer and use it again (not repeating) */
    startTimeMillis = 0UL;
    durationMillis = duration * type;
    originalTimeType = type;
    complete = false;
}

unsigned long simpleTimer::getDuration(TimeType type)
{
    return durationMillis / type;
}

void simpleTimer::start()
{
    startTimeMillis = millis();
}

void simpleTimer::stop()
{
    startTimeMillis = 0UL;
    complete = false;
}

bool simpleTimer::isStarted()
{
    return startTimeMillis != 0UL;
}

void simpleTimer::process()
{
    if (!complete && (startTimeMillis != 0UL))
    {
        complete = millis() >= startTimeMillis + durationMillis;
    }
}

bool simpleTimer::isComplete()
{
    /* so that we can overwrite our current
     * complete class variable if needs be */
    bool returnComplete = complete;

    if (startTimeMillis == 0UL)
    {
        return false;
    }

    /* if this is a repeat timer start the timer again
     * and set the timer to not be completed again */
    if (complete && repeatTimer)
    {
        complete = false;
        start();
    }

    return returnComplete;
}

unsigned long simpleTimer::getTimeToEnd(TimeType returnType)
{
    return (durationMillis - (millis() - startTimeMillis)) / returnType;
}

void simpleTimer::timerProcessingSet(simpleTimer *timer, bool needsProcessed)
{
    if (needsProcessed)
    {
        /* add the timer to the vector */
        simpleTimer::timers.push_back(timer);
    }
    else
    {
        /* remove the timer from the timer vector if remove specified */
        simpleTimer::timers.erase(std::remove(simpleTimer::timers.begin(), simpleTimer::timers.end(), timer), simpleTimer::timers.end());
    }
}

std::vector<simpleTimer*>* simpleTimer::getTimers()
{
    return &simpleTimer::timers;
}

void simpleTimer::serialPrint()
{
    String timeTypeStr = originalTimeType == MINUTES ? " mins"
                       : originalTimeType == HOURS ? " hours"
                       : originalTimeType == MILLISECONDS ? " ms"
                       : originalTimeType == SECONDS ? " seconds"
                       : "undefined";

    Serial.print("\t\tDuration -> ");

    if (complete || startTimeMillis == 0UL)
    {
        if (complete)
        {
            Serial.print("complete ");
        }
        else if (startTimeMillis == 0UL)
        {
            Serial.print("not started ");
        }

        Serial.print((int) (durationMillis / originalTimeType));
        Serial.println(timeTypeStr);
    }
    else
    {
        Serial.print((int) ((millis() - startTimeMillis) / originalTimeType));
        Serial.print(" of ");
        Serial.print((int) (durationMillis / originalTimeType));
        Serial.println(timeTypeStr);
    }
}
