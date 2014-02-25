// Do not remove the include below
#include "test_DFRobotPowerShield.h"

#include "DFRobotPowerShield.h"

DFRobotPowerShield  powerShield;

static const char CHAR_FRAME_BEGIN          = '$';
static const char CHAR_FRAME_END            = '*';
static const char CHAR_FRAME_FIELDSEPARATOR = ';';

void setup()
{
    Serial.begin( 115200 );
    Serial.println( "Hello World !" );

    // No need to call DFRobotPowerShield::init() as it has already been done
    // in the constructor.
//    Serial.print( "Calling DFRobotPowerShield::init()........" );
//    powerShield.init();
//    Serial.println( "[  OK  ]" );

    powerShield.setOutputEnabled( true );

}

void loop()
{
    static unsigned int loopsCount   = 0;

//    if( loopsCount > 50 ) {
//        powerShield.setOutputEnabled( ! powerShield.outputEnabled() );
//        loopsCount  = 0;
//    }


    /*
     * Print the start of frame and a tag to identify which project is running
     */
    Serial.print( CHAR_FRAME_BEGIN );
    Serial.print( "test_DFRobotPowerShield" );

    Serial.print( CHAR_FRAME_FIELDSEPARATOR );

    /*
     * Print the auxiliary power output's command state
     */
    if( powerShield.outputEnabled() == true )
        Serial.print( "1" );
    else
        Serial.print( "0" );

    Serial.print( CHAR_FRAME_FIELDSEPARATOR );


    /*
     * Print the voltage of the auxiliary power output
     */

    Serial.print( powerShield.voltage() );


    /*
     * Print the end of frame
     */
    Serial.println( CHAR_FRAME_END );



    ++loopsCount;
    delay( 100 );
}

