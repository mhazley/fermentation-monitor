#include "status_message.h"

using namespace std;

void StatusMessage::init( float out_temp, float amb_temp, float tw_temp )
{
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