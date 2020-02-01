#include "Particle.h"
#include "temperature.h"
#include "DS18.h"

#define ADDR_LEN ( 8 )

void print_serial( void );

static uint8_t              outside_addr[ADDR_LEN] = { 0x28, 0xAA, 0x30, 0x66, 0x53, 0x14, 0x01, 0x86 };
static uint8_t              ambient_addr[ADDR_LEN] = { 0x28, 0xAA, 0x91, 0x2A, 0x54, 0x14, 0x01, 0xF0 };
static uint8_t              thermow_addr[ADDR_LEN] = { 0x28, 0xAA, 0x27, 0x4A, 0x54, 0x14, 0x01, 0xB3 };

static DS18                 ds18_driver(D9);

static TemperatureSensor    temp_outside( &ds18_driver, outside_addr );
static TemperatureSensor    temp_ambient( &ds18_driver, ambient_addr );
static TemperatureSensor    temp_thermow( &ds18_driver, thermow_addr );

// Temperature Sampling Timers
Timer                       timer_temp_outside( 1000, &TemperatureSensor::sample, temp_outside );
Timer                       timer_temp_ambient( 1000, &TemperatureSensor::sample, temp_ambient );
Timer                       timer_temp_thermow( 1000, &TemperatureSensor::sample, temp_thermow );

// Serial Timer
Timer                       timer_serial( 1000, print_serial );

// static double               m_set_point_as_double;

void setup()
{
    Serial.begin(115200);

    // Particle.function("setpoint", setpoint);
    // Particle.function("get_status", get_status);

    timer_temp_outside.start();
    timer_temp_ambient.start();
    timer_temp_thermow.start();
    timer_serial.start();
}

void loop()
{
    // ds18_driver.start_conv(outside_addr);
    // delay(1000);
    // ds18_driver.read(outside_addr);
    // Serial.println( ds18_driver.celsius() );
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

void print_serial( void )
{
    Serial.print(temp_outside.get_temperature());Serial.print(", ");
    Serial.print(temp_ambient.get_temperature());Serial.print(", ");
    Serial.print(temp_thermow.get_temperature());Serial.println();
}