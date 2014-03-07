// Do not remove the include below
#include "test_Sparkfun_fuelGauge.h"

#include <Wire.h>

#include "MAX1704X.h"

MAX1704X sensor( MAX1704X::PartMAX17043 );

void setup()
{
    /* Setup serial interface */
    Serial.begin( 115200 );
    Serial.println( "Sparkfun \"Fuel Gauge\" test ( using MAX1704X class for part MAX17043 )" );


    Wire.begin();

//    Serial.print( "MAX1704 reset..........................." );
//    sensor.reset();
//    Serial.println( "[  OK  ]" );

//    Serial.print( "MAX1704 quick start....................." );
//    sensor.quickStart();
//    Serial.println( "[  OK  ]" );

//    Serial.print( "MAX1704 awake..........................." );
//    sensor.awake();
//    if( sensor.isSleeping() )
//        Serial.println( "[FAILED]" );
//    else
//        Serial.println( "[  OK  ]" );

//    Serial.println( "========================================" );
//    Serial.println( "== MAX1704 config ======================" );
//    sensor.showConfig();
//    Serial.println( "========================================" );


//    Serial.print( "Press any key to continue..." );
//    while( Serial.available() <= 0 );
//    Serial.println();

//    Serial.println( "Will now send frames..." );
//    delay( 1000 );
}

void loop()
{
    static uint8_t loopsBeforeQuickStart  = 10;

    if( ! sensor.setAlertThreshold_pc( loopsBeforeQuickStart ) ) {
        Serial.print( "Error while setting alert threshold to " );
        Serial.println( loopsBeforeQuickStart );
    }


    Serial.print( "$test_Sparkfun_fuelGauge;" );

    Serial.print( sensor.stateOfCharge_pc() );
    Serial.print( ';' );

    Serial.print( sensor.cellVoltage_V() );
    Serial.print( ';' );

    Serial.print( sensor.version() );
    Serial.print( ';' );

    Serial.print( sensor.isSleeping() ? "sleeping":"awake" );
    Serial.print( ';' );

    Serial.print( sensor.isAlerting() ? "ALERT":"alert_none" );
    Serial.print( ';' );

    Serial.print( sensor.alertThreshold_pc() );
    Serial.print( ';' );

    Serial.println( "*" );

    delay(1000);


//    sensor.setSleeping( true );
    sensor.setSleeping( ! sensor.isSleeping() );


    Serial.print( "Reset in " );
    Serial.println( loopsBeforeQuickStart );

    if( loopsBeforeQuickStart <= 0 ) {
        sensor.triggerReset();
        loopsBeforeQuickStart = 100;
    } else {
        loopsBeforeQuickStart--;
    }
}

