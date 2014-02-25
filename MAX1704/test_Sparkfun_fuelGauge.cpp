// Do not remove the include below
#include "test_Sparkfun_fuelGauge.h"

#include <Wire.h>

#include "MAX1704.h"

MAX1704 sensor;

void setup()
{
    /* Setup serial interface */
    Serial.begin( 115200 );
    Serial.println( "Sparkfun \"Fuel Gauge\" test ( using MAX1704 )" );


    Wire.begin();

    Serial.print( "MAX1704 reset..........................." );
    sensor.reset();
    Serial.println( "[  OK  ]" );

    Serial.print( "MAX1704 quick start....................." );
    sensor.quickStart();
    Serial.println( "[  OK  ]" );

//    Serial.print( "MAX1704 awake..........................." );
//    sensor.awake();
//    if( sensor.isSleeping() )
//        Serial.println( "[FAILED]" );
//    else
//        Serial.println( "[  OK  ]" );

    Serial.println( "========================================" );
    Serial.println( "== MAX1704 config ======================" );
    sensor.showConfig();
    Serial.println( "========================================" );


    Serial.print( "Press any key to continue..." );
    while( Serial.available() <= 0 );
    Serial.println();

    Serial.println( "Will now send frames..." );
    delay( 1000 );
}

void loop()
{
    Serial.print( "$test_Sparkfun_fuelGauge;" );

//    Serial.print( (byte)sensor.version(), HEX );
//    Serial.print( ';' );

//    Serial.print( sensor.alertThreshold() );
//    Serial.print( ';' );

    Serial.print( sensor.stateOfCharge() );
    Serial.print( ';' );

//    Serial.print( sensor.isSleeping() ? "sleeping":"awake" );
//    Serial.print( ';' );

//    Serial.print( sensor.isAlerting() ? "ALERT":"alert_none" );
//    Serial.print( ';' );

    Serial.println( "*" );

    delay(500);
}

