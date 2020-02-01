// #include "Particle.h"
// #include "DS18.h"

// #define ADDR_LEN ( 8 )

// static uint8_t sensor_one_addr[ADDR_LEN] = { 0x28, 0xE2, 0xBA, 0xDF, 0x0B, 0x00, 0x00, 0xF1 };
// static uint8_t sensor_two_addr[ADDR_LEN] = { 0x28, 0x00, 0xDE, 0xDF, 0x0B, 0x00, 0x00, 0xB8 };

// DS18 sensor(D9);

// void setup()
// {
//     pinMode(D10, OUTPUT);
//     digitalWrite(D10, LOW);
//     Serial.begin(115200); 
// }

// void loop()
// {
//     // sensor.mh_start_conv(sensor_one_addr);
//     // sensor.mh_start_conv(sensor_two_addr);
//     // digitalWrite(D10, LOW);
//     // delay(1000);
//     // digitalWrite(D10, HIGH);
//     // delay(1000);

//     // if (sensor.mh_read(sensor_one_addr))
//     if (sensor.read())
//     {
//         Serial.println(sensor.celsius());
//     }

//     // if (sensor.mh_read(sensor_two_addr))
//     // {
//     //     Serial.print("sensor_2: ");
//     //     Serial.println(sensor.celsius());
//     // }

// }
