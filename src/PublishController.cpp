#include "fermenter.h"
#include "PublishController.h"
#include "JsonParserGeneratorRK.h"

#define DEFAULT_REMOTE_DELAY        1UL

PublishController::PublishController()
{
      started = false;

      remoteTimer = new CallbackTimer<PublishController>(this, &PublishController::callbackRemote, true);

      remoteTimer->setDuration(DEFAULT_REMOTE_DELAY, MINUTES);
}

void PublishController::start()
{
    remoteTimer->start();
    started = true;
}

const char* PublishController::getRemoteUrlString(PublishType type)
{
    switch (type)
    {
        case PUBLISH_TYPE_TEMPERATURE:        return "geometry.fermenter.temperature"; break;
        case PUBLISH_TYPE_SETPOINT:           return "geometry.fermenter.setpoint"; break;
        case PUBLISH_TYPE_HEATING:            return "geometry.fermenter.heating"; break;
        case PUBLISH_TYPE_COOLING:            return "geometry.fermenter.cooling"; break;
        case PUBLISH_TYPE_RUNNING:            return "geometry.fermenter.running"; break;
        case PUBLISH_TYPE_STATUS:             return "geometry.fermenter.status"; break;
    }

    return "geometry.fermenter.unknown";
}

PublishError PublishController::setValue(PublishType type, String data)
{
    publishMap[type] = data;

    return E_SUCCESS;
}

void PublishController::callbackRemote()
{
    JsonWriterStatic<256> jw;

    {
        JsonWriterAutoObject obj(&jw);
        jw.insertKeyValue("temperature", publishMap[PUBLISH_TYPE_TEMPERATURE]);
	    jw.insertKeyValue("setpoint",    publishMap[PUBLISH_TYPE_SETPOINT]);
	    jw.insertKeyValue("heating",     publishMap[PUBLISH_TYPE_HEATING]);
        jw.insertKeyValue("cooling",     publishMap[PUBLISH_TYPE_COOLING]);
        jw.insertKeyValue("running",     publishMap[PUBLISH_TYPE_RUNNING]);
    }

    publishType = PUBLISH_TYPE_STATUS;
    publishMap[PUBLISH_TYPE_STATUS] = jw.getBuffer();
    sendValue(true);

}

void PublishController::sendValue(bool print)
{
    if( print )
    {
        Serial.println("===> Publish");
        Serial.println(getRemoteUrlString((PublishType)publishType));
        Serial.println(publishMap[(PublishType)publishType]);
    }
    Particle.publish(getRemoteUrlString((PublishType)publishType), publishMap[(PublishType)publishType], DEFAULT_PUBLISH_TTL, PRIVATE);
}
