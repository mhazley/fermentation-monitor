#include "fermenter.h"
#include "PublishController.h"

#define DEFAULT_REMOTE_DELAY        10000UL

PublishController::PublishController()
{
      started = false;

      remoteTimer = new CallbackTimer<PublishController>(this, &PublishController::callbackRemote, true);

      remoteTimer->setDuration(DEFAULT_REMOTE_DELAY, MILLISECONDS);

      deviceId = System.deviceID();
}

void PublishController::start()
{
    remoteTimer->start();
    started = true;
    sendValues();
}

const char* PublishController::getRemoteUrlString(PublishType type)
{
    switch (type)
    {
        case PUBLISH_TYPE_TEMPERATURE:        return "geometry.fermenter.temperature"; break;
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
    sendValues();
}

void PublishController::sendValues()
{
    Serial.println("sendValues");
    for (publish_type_it iterator = publishMap.begin(); iterator != publishMap.end(); iterator++)
    {
        Serial.println("PUBLISH");
        Serial.println(getRemoteUrlString(iterator->first));
        Serial.println(iterator->second);
        Particle.publish(getRemoteUrlString(iterator->first), iterator->second, DEFAULT_PUBLISH_TTL, PRIVATE);
    }
}
