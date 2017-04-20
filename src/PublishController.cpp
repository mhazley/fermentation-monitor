#include "fermenter.h"
#include "PublishController.h"

#define DEFAULT_REMOTE_DELAY        1000UL

PublishController::PublishController()
{
      started = false;

      remoteTimer = new CallbackTimer<PublishController>(this, &PublishController::callbackRemote, true);

      remoteTimer->setDuration(DEFAULT_REMOTE_DELAY, MILLISECONDS);
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
    sendValue();

    if( ++publishType == PUBLISH_TYPE_MAX )
    {
        publishType = 0;
    }

}

void PublishController::sendValue()
{
    Serial.println("===> Publish");
    Serial.println(getRemoteUrlString((PublishType)publishType));
    Serial.println(publishMap[(PublishType)publishType]);
    Particle.publish(getRemoteUrlString((PublishType)publishType), publishMap[(PublishType)publishType], DEFAULT_PUBLISH_TTL, PRIVATE);
}
