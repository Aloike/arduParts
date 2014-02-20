// Do not remove the include below
#include "test_GA1A1S202WP_lightSensor.h"


/*
  Test sketch for the Adafruit Analog Light Sensor
  by Bill Earl
  for Adafruit Industries

  Edited by GO M. on 2014-02-19:
    * Encapsulated sensor data input into a specific function
    * Formed output into a frame

  Connect sensor output to Analog Pin 0
  Connect 5v to VCC and GND to GND
  Connect 3.3v to the AREF pin

  Source: http://learn.adafruit.com/adafruit-ga1a12s202-log-scale-analog-light-sensor/use-it
 */

int sensorPin = A0;    // select the input pin for the potentiometer

float rawRange = 1024; // 3.3v
float logRange = 5.0; // 3.3v = 10^5 lux


static const float C_MAX_LUM    = pow( 10, logRange );

void setup()
{
    /* Use the 3.3V external reference (on AREF pin) instead of internal 5V one. */
    analogReference(EXTERNAL);

    /* Setup serial interface */
    Serial.begin( 115200 );
    Serial.println("Adafruit Analog Light Sensor Test");
}


void loop()
{
    Serial.print( "$test_GA1A1S202WP_lightSensor;" );
    Serial.print( analogRead(sensorPin) );
    Serial.print( ";" );
    Serial.print( getSensorData() );
    Serial.print( ";" );
    Serial.print( C_MAX_LUM );
    Serial.println( "*" );

//    Serial.print("Raw = ");
//    Serial.print(rawValue);
//    Serial.print(" - Lux = ");
//    Serial.println(RawToLux(rawValue));

    delay(100);
}

float getSensorData( void )
{
    // read the raw value from the sensor:
    int rawValue = analogRead(sensorPin);

    float logLux = rawValue * logRange / rawRange;
    return pow(10, logLux);
}
