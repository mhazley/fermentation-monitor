#include "Particle.h"
#include "temperature.h"
#include "DS18.h"
#include "status_message.h"
#include "board_defs.h"

#define ADDR_LEN        8
#define STATUS_TOPIC    "geometry.fermenter.status"

// Module Functions
void print_serial( void );
int get_status( String cmd );

// Module Data
static uint8_t              m_outside_addr[ADDR_LEN] = { 0x28, 0xAA, 0x30, 0x66, 0x53, 0x14, 0x01, 0x86 };
static uint8_t              m_ambient_addr[ADDR_LEN] = { 0x28, 0xAA, 0x91, 0x2A, 0x54, 0x14, 0x01, 0xF0 };
static uint8_t              m_thermow_addr[ADDR_LEN] = { 0x28, 0xAA, 0x27, 0x4A, 0x54, 0x14, 0x01, 0xB3 };
static DS18                 m_ds18_driver( ONE_WIRE_BUS_PIN );
static TemperatureSensor    m_temp_outside( &m_ds18_driver, m_outside_addr );
static TemperatureSensor    m_temp_ambient( &m_ds18_driver, m_ambient_addr );
static TemperatureSensor    m_temp_thermow( &m_ds18_driver, m_thermow_addr );
static bool                 m_data_request = false;

// Temperature Sampling Timers
Timer                       m_timer_temp_outside( 1000, &TemperatureSensor::sample, m_temp_outside );
Timer                       m_timer_temp_ambient( 1000, &TemperatureSensor::sample, m_temp_ambient );
Timer                       m_timer_temp_thermow( 1000, &TemperatureSensor::sample, m_temp_thermow );

// Serial Timer
Timer                       m_timer_serial( 1000, print_serial );

// static double               m_set_point_as_double;

void setup()
{
    Serial.begin(115200);

    // Particle.function("setpoint", setpoint);
    Particle.function("get_status", get_status);

    m_timer_temp_outside.start();
    m_timer_temp_ambient.start();
    m_timer_temp_thermow.start();
    m_timer_serial.start();
}

void loop()
{
    if( m_data_request )
    {
        StatusMessage msg;
        msg.init( m_temp_outside.get_temperature(), 
                  m_temp_ambient.get_temperature(),
                  m_temp_thermow.get_temperature() );
        
        // If it publishes successfully then we set request to false
        m_data_request = !Particle.publish(STATUS_TOPIC, msg.serialize_message(), PRIVATE );
    }
}

int setpoint(String setPoint)
{
    // if( setPoint.length() > 0 )
    // {
    //     m_set_point_as_double = setPoint.toFloat();
        
    //     // if (m_set_point_as_double == 0)
    //     // {
    //     //     // fermentationController->stop();
    //     // }
    //     // else
    //     // {
    //     //     // fermentationController->start(RUNNING, m_set_point_as_double);
    //     // }
        
    //     return 1;
    // }
    // else
    // {
    //     return -1;
    // }
}

int get_status( String cmd )
{
    m_data_request = true;
    return 1;
}

void print_serial( void )
{
    Serial.print(m_temp_outside.get_temperature());Serial.print(", ");
    Serial.print(m_temp_ambient.get_temperature());Serial.print(", ");
    Serial.print(m_temp_thermow.get_temperature());Serial.println();
}