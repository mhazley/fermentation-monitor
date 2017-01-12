#ifndef TIMER_H_
#define TIMER_H_

#include <vector>
#include <algorithm>
#include "application.h"

enum TimeType {
  MILLISECONDS = 1,
  SECONDS = MILLISECONDS * 1000,
  MINUTES = SECONDS * 60,
  HOURS = MINUTES * 60
};

class simpleTimer
{
    private:
        unsigned long startTimeMillis = 0UL;
        unsigned long durationMillis = 0UL;
        TimeType originalTimeType;
        bool repeatTimer = false;

    protected:
        bool complete = false;

    public:
        simpleTimer(bool repeatTimer);
        ~simpleTimer();
        void start();
        void stop();
        bool isStarted();
        virtual void process();
        void setDuration(unsigned long duration, TimeType type);
        unsigned long getDuration(TimeType type);
        unsigned long getTimeToEnd(TimeType returnType);
        bool isComplete();
        void serialPrint();
        static void timerProcessingSet(simpleTimer *timer, bool needsProcessed);
        static std::vector<simpleTimer*>* getTimers();
        /* all timers in system will add themselves to this */
        static std::vector<simpleTimer*> timers;
};

#endif
