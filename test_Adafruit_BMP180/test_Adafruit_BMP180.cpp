/*!
@file   test_Adafruit_BMP180.cpp
@brief  Test for Adafruit_BMP180 sensor interface class.

BMP180 to Arduino nano328 connections:
+ Vin to +5V
+ GND to GND
+ SCL to AN5
+ SDA to AN4

*/

// Do not remove the include below
#include "test_Adafruit_BMP180.h"

#include "Adafruit_BMP180.h"


Adafruit_BMP180 sensor;

//"........................................"

void setup()
{
    /* Setup serial interface */
    Serial.begin( 115200 );
    Serial.println( "Adafruit BMP180 Pressure Sensor Test" );


    /* Setup Adafruit BMP180 sensor */
    Serial.print( "Init sensor BMP180......................" );
    if( sensor.init() != Adafruit_BMP180::NoError ) {
        Serial.println( "[FAILED]" );
        while( 1 );
    } else {
        Serial.println( "[  OK  ]" );
    }

}

void loop()
{
    /* Print Start Of Frame */
    Serial.print( "$test_Adafruit_BMP180;" );


    /* Print raw temperature */
    Serial.print( sensor.readRawTemperature() );
    Serial.print( ";" );


    /* Print Celsius temperature */
    Serial.print( sensor.getTemperature_c() );
    Serial.print( ";" );


    /* Print raw pressure */
    Serial.print( sensor.readRawPressure() );
    Serial.print( ";" );


    /* Print Pascals pressure */
    Serial.print( sensor.getPressure_Pa() );
    Serial.print( ";" );


    /* Print Altitude */
    Serial.print( sensor.getAltitude_m() );
    Serial.print( ";" );


    /* Print End Of Frame */
    Serial.println( "*" );

    delay( 100 );
}

