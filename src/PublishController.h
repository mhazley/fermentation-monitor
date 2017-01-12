#ifndef PUBLISHCONTROLLER_H_
#define PUBLISHCONTROLLER_H_

#include <algorithm>
#include "MapHack.h"
#include "CallbackTimer.h"
#include "application.h"

enum PublishType
{
    PUBLISH_TYPE_TEMPERATURE      = 1 << 1,
};

enum PublishDestination
{
    PUBLISH_DESTINATION_REMOTE    = 1,
    PUBLISH_DESTINATION_LOCAL     = 1 << 1,
    PUBLISH_DESTINATION_ALL       = PUBLISH_DESTINATION_REMOTE | PUBLISH_DESTINATION_LOCAL
};

enum PublishError
{
    E_SUCCESS = 0,
    E_NO_TYPE_SPECIFIED = 1,
    E_PUBLISH_MAX_DATA = 2
};

class PublishController
{

    private:
        CallbackTimer<PublishController>* remoteTimer;

        bool started;

        typedef std::map<PublishType, String> PublishMap;
        typedef std::map<PublishType, String>::const_iterator publish_type_it;

        String deviceId;

        PublishMap publishMap;

        void sendValues();

        const char* getRemoteUrlString(PublishType type);

    public:
        PublishController();
        void start();
        void callbackRemote();

        /* this may be used to set a value, if the type has not been
         * set before it will be added to the map, if it has been
         * seen then it will overwrite the value in the map */
        PublishError setValue(PublishType type, String data);
};

#endif
