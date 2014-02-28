// Do not remove the include below
#include "test_PCA9685.h"

#include "PCA9685.h"



/* ######################################################################### */
/* ######################################################################### */

PCA9685 controller;

/* ######################################################################### */
/* ######################################################################### */

void    pca9685_printConfig( PCA9685& argController )
{
    Serial.println( "========================================" );
    Serial.println( "PCA9685 controller config:" );

    Serial.print( "+-- Address = 0x" );
    Serial.println( argController.address(), HEX );

    Serial.print( "+-- clock (" );
    if( argController.isUsingExternalClock() ) {
        Serial.print( "EXTERNAL" );
    } else {
        Serial.print( "internal" );
    }
    Serial.print( ") = " );
    Serial.print( argController.clockFrequency() );
    Serial.println( " Hz" );



    Serial.println( "+-- Power management" );
    Serial.print( "    +-- Sleeping ?   ==" ); Serial.println( argController.isSleeping()? "YES":"no" );
    Serial.print( "    +-- CanRestart ? ==" ); Serial.println( argController.canRestart()? "yes":"no" );


    Serial.println( "+-- I2C adresses:" );
    Serial.print( "    +-- subaddress 1 enabled........" );
    Serial.println( argController.subAddress1Enabled() ? "[ true ]" : "[  NO  ]" );

    Serial.print( "    +-- subaddress 2 enabled........" );
    Serial.println( argController.subAddress2Enabled() ? "[ true ]" : "[  NO  ]" );

    Serial.print( "    +-- subaddress 3 enabled........" );
    Serial.println( argController.subAddress3Enabled() ? "[ true ]" : "[  NO  ]" );

    Serial.print( "    +-- answer to led all call address" );
    Serial.println( argController.ledAllCallAddressEnabled() ? "[ true ]" : "[  NO  ]" );


    Serial.println( "+-- Output management:" );
    Serial.print( "    +-- Frequency == " );
    Serial.print( argController.outputFrequency() );
    Serial.println( " Hz" );


    Serial.print( "    +-- Logical state inverted = " );
    Serial.println( argController.outputStateInverted() ? "YES" : "no" );



    Serial.print( "    +-- changes on I2C " );
    switch( argController.outputChangeEvent() )
    {
        case PCA9685::ChangeOnStop:
            Serial.println( "STOP" );
            break;

        case PCA9685::ChangeOnAcknowledge:
            Serial.println( "ACK" );
            break;

        default:
            Serial.println( "UNKNOWN !!!" );
            break;
    }


    Serial.print( "    +-- struct is " );
    switch( argController.outputStructure() )
    {
        case PCA9685::OutputStructureOpenDrain:
            Serial.println( "Open-drain" );
            break;

        case PCA9685::OutputStructureTotemPole:
            Serial.println( "Totem pole" );
            break;

        default:
            Serial.println( "UNKNOWN !!!" );
            break;
    }


    Serial.print( "    +-- policy when /OE=1 --> " );
    switch( argController.disabledOutputPolicy() )
    {
        case PCA9685::OutputFalse:
            Serial.println( "logical false" );
            break;

        case PCA9685::OutputHighImpedance:
            Serial.println( "high impendance" );
            break;

        case PCA9685::OutputTrue:
            Serial.println( "logical 1" );
            break;

        default:
            Serial.println( "UNKNOWN !!!" );
            break;
    }

    Serial.println( "========================================" );
}
/* ######################################################################### */
/* ######################################################################### */

void setup()
{
    Serial.begin( 115200 );
    Serial.println( "========================================" );
    Serial.println( "========================================" );
    Serial.println( "Hello World !" );
    Serial.println( "PCA9685 test" );
    Serial.println( "========================================" );

    Serial.print( "PCA9685 controller instanciated with address 0x" );
    Serial.println( controller.address(), HEX );

    Serial.print( "PCA9685 controller instanciated with clock frequency of " );
    Serial.print( controller.clockFrequency() );
    Serial.println( " Hz" );

    Serial.println( "========================================" );


    Serial.print( "PCA9685 init............................" );
    controller.init();
    Serial.println( "[  OK  ]" );


#if 0
    /*
     *
     *  These are just dev tests, migh be cleaned later
     *
     */
    if( controller.isSleeping() ) {
        Serial.println( "PCA9685 controller is sleeping !" );
    } else {
        Serial.println( "PCA9685 controller is awake !" );
    }

    controller.setSleeping( true );
    if( controller.isSleeping() ) {
        Serial.println( "PCA9685 controller is sleeping !" );
    } else {
        Serial.println( "PCA9685 controller is awake !" );
    }


    if( controller.canRestart() ) {
        if( controller.restart() ) {
            Serial.println( "PCA9685 controller has been restarted !" );
        } else {
            Serial.println( "PCA9685 controller can't be restarted (<--not an error?)" );
        }
    } else {
        Serial.println( "PCA9685 controller can NOT be restarted (<--not an error)" );
    }


    Serial.println( "========================================" );

//    controller.setUseExternalClock();
#if 0
    Serial.print( "PCA9685 controller is using " );
    if( controller.isUsingExternalClock() ) {
        Serial.print( "EXTERNAL" );
    } else {
        Serial.print( "internal" );
    }
    Serial.println( " clock source." );
#else
    Serial.print( "PCA9685 is using external clock........." );
    Serial.println( controller.isUsingExternalClock() ? "[ true ]" : "[  NO  ]" );
#endif


//    controller.setLedAllCallAddressEnabled( true );

    Serial.print( "PCA9685 I2C subaddress 1 enabled........" );
    Serial.println( controller.subAddress1Enabled() ? "[ true ]" : "[  NO  ]" );

    Serial.print( "PCA9685 I2C subaddress 2 enabled........" );
    Serial.println( controller.subAddress2Enabled() ? "[ true ]" : "[  NO  ]" );

    Serial.print( "PCA9685 I2C subaddress 3 enabled........" );
    Serial.println( controller.subAddress3Enabled() ? "[ true ]" : "[  NO  ]" );

    Serial.print( "PCA9685 will answer led all call address" );
    Serial.println( controller.ledAllCallAddressEnabled() ? "[ true ]" : "[  NO  ]" );


    Serial.print( "PCA9685 output's logical state inverted." );
    Serial.println( controller.outputStateInverted() ? "[ true ]" : "[  NO  ]" );


//    controller.setOutputChange( PCA9685::ChangeOnStop );
    Serial.print( "PCA9685 output changes on I2C STOP......" );
    Serial.println( controller.outputChangeEvent()==PCA9685::ChangeOnStop ? "[ true ]" : "[  NO  ]" );
    Serial.print( "PCA9685 output changes on I2C ACK......." );
    Serial.println( controller.outputChangeEvent()==PCA9685::ChangeOnAcknowledge ? "[ true ]" : "[  NO  ]" );


//    controller.setOutputStructure( PCA9685::OutputStructureTotemPole );
//    controller.setOutputStructure( PCA9685::OutputStructureOpenDrain );
    Serial.print( "PCA9685 output struct is open drain....." );
    Serial.println( controller.outputStructure()==PCA9685::OutputStructureOpenDrain ? "[ true ]" : "[  NO  ]" );
    Serial.print( "PCA9685 output struct is Totem Pole....." );
    Serial.println( controller.outputStructure()==PCA9685::OutputStructureTotemPole ? "[ true ]" : "[  NO  ]" );



    controller.setDisabledOutputPolicy( PCA9685::OutputHighImpedance );
    Serial.print( "PCA9685 output policy when /OE=1........" );
    switch( controller.disabledOutputPolicy() )
    {
        case PCA9685::OutputFalse:
            Serial.println( "[DISABL]" );
            break;

        case PCA9685::OutputHighImpedance:
            Serial.println( "[HIGH-Z]" );
            break;

        case PCA9685::OutputTrue:
            Serial.println( "[ENABLE]" );
            break;

        default:
            Serial.println( "[UNKNOW]" );
    }


    Serial.print( "PCA9685 controller output Estimated frequency==" );
    Serial.print( controller.outputFrequency() );
    Serial.println( " Hz" );


    Serial.println( "========================================" );
    controller.setOutputFrequency( 0 );
    controller.setOutputFrequency( 25000 );
    controller.setOutputFrequency( 6101 );
    controller.setOutputFrequency( 42 );
    controller.setOutputFrequency( 50 );
    Serial.print( "Output frequency is now " );
    Serial.print( controller.outputFrequency() );
    Serial.println( " Hz" );

    Serial.println( "========================================" );

    Serial.println( "---> Resetting controller...<---" );
    controller.reset();

    Serial.println( "========================================" );
#endif
}

/* ######################################################################### */
/* ######################################################################### */


/* ######################################################################### */
/* ######################################################################### */

void loop()
{
    static uint16_t step        = 10;


    /* juste a check to be shure the controller is awake */
    if( controller.isSleeping() ) {
        Serial.println( "PCA9685 controller is sleeping !" );
        Serial.println( "+-- Waking it up..." );
        controller.setSleeping( false );
        if( controller.isSleeping() ) {
            Serial.println( "+-- Still sleeping !!" );
        } else {
            Serial.println( "+-- Controller is now awake !" );
        }
    }


    /* Print controller's configuration periodically */
    pca9685_printConfig( controller );



    /* Control each output, one by one. */
    for( int outputNbr = 0 ; outputNbr < PCA9685::OUTPUT_COUNT ; ++outputNbr )
    {
        Serial.println( String( "Controlling output " ) + outputNbr );

        Serial.println( "+-- up..." );
        for( uint16_t val = 0 ; val < 4096 ; val = val+step )
        {
            controller.setOutput( outputNbr, 0, val );
        }

        Serial.println( "+-- down..." );
        for( int val = 4095 ; val > 0 ; val = val-step )
        {
            controller.setOutput( outputNbr, 0, val );
        }
    }
}

/* ######################################################################### */
/* ######################################################################### */
