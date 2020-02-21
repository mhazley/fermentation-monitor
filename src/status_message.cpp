#include "status_message.h"

using namespace std;

void StatusMessage::init( float out_temp, float amb_temp, float tw_temp, bool heating, bool cooling, float setpoint, ferm_mode_t mode )
{
    
    if( mode == IDLE ) 
    {
        this->json_buffer["mode"] = "idle";
    }
    else if( mode == RUNNING ) 
    {
        this->json_buffer["mode"] = "running";
    }
    else if( mode == RUNNING_S ) 
    {
        this->json_buffer["mode"] = "running-saison";
    }

    this->json_buffer["h"] = heating;
    this->json_buffer["c"] = cooling;
    this->json_buffer["sp"] = setpoint;
    this->json_buffer["ot"] = out_temp;
    this->json_buffer["at"] = amb_temp;
    this->json_buffer["tt"] = tw_temp;
}

String StatusMessage::serialize_message()
{
    String m;
    serializeJson(this->json_buffer, m);
    return m;
}