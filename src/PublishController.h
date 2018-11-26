#ifndef PUBLISHCONTROLLER_H_
#define PUBLISHCONTROLLER_H_

#include <algorithm>
#include "MapHack.h"
#include "CallbackTimer.h"
#include "application.h"

enum PublishType
{
    PUBLISH_TYPE_TEMPERATURE = 0,
    PUBLISH_TYPE_SETPOINT    = 1,
    PUBLISH_TYPE_HEATING     = 2,
    PUBLISH_TYPE_COOLING     = 3,
    PUBLISH_TYPE_RUNNING     = 4,
    PUBLISH_TYPE_STATUS      = 5,

    PUBLISH_TYPE_MAX         = 6
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
        //typedef std::map<PublishType, String>::const_iterator publish_type_it;

        int publishType = 0;

        PublishMap publishMap;

        void sendValue(bool print);

        const char* getRemoteUrlString(PublishType type);
        
        bool publishable = false;

    public:
        PublishController();
        void start();
        void callbackRemote();
        void setPublishable( bool val );

        /* this may be used to set a value, if the type has not been
         * set before it will be added to the map, if it has been
         * seen then it will overwrite the value in the map */
        PublishError setValue(PublishType type, String data);
};

#endif
