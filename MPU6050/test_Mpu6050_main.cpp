// Do not remove the include below
#include "test_Mpu6050_main.h"

#include <Arduino.h>
#include <Servo.h>


/* ######################################################################### */
/* ######################################################################### */

#define VERBOSE                     /*!< A commenter pour desactiver log */
#define PRINT_ACCEL_RAW             /*!< Commenter pour desactiver affichage des valeurs "brutes" de l'accelerometre */
#define PRINT_ACCEL_G               /*!< Commenter pour desactiver affichage des valeurs en G de l'accelerometre */
#define PRINT_CALCULATED_ANGLES     /*!< Commenter pour desactiver affichage des valeurs de l'inclinaison calculée à partir de l'acceleration */
#define PRINT_GYRO_RAW              /*!< Commenter pour desactiver affichage des valeurs "brutes" du gyroscope */
#define PRINT_GYRO_DEG_PER_S        /*!< Commenter pour desactiver affichage des valeurs en degres par seconde du gyroscope */
#define PRINT_TEMPERATURE           /*!< Commenter pour desactiver affichage de la temperature */



#ifdef VERBOSE
#   define  LOG(msg)    Serial.print( msg )
#   define  LOGLN(msg)  Serial.println( msg )
#else
#   define LOG(msg)
#   define LOGLN(msg)
#endif


/* ######################################################################### */
/* ######################################################################### */

struct CartesianCoordinates {
    double  x;
    double  y;
    double  z;
};

/* ######################################################################### */
/* ######################################################################### */

static const double C_DEG_TO_RAD    =  0.017453293;
static const double C_RAD_TO_DEG    = 57.295779506;

Mpu6050         sensor;

/* ######################################################################### */
/* ######################################################################### */

void setup()
{
    String errorString;

    /* Initialisation de la liaison série */
    Serial.begin( 115200 );
    LOGLN( "Hello World !" );


    /* Initialisation du capteur */
    LOG( "MPU6050 init...................." );
    if( ! sensor.init( &errorString ) ) {
        /* Si une erreur est survenue lors de l'initialisation */
        Serial.println( "[Erreur]");
        Serial.print( "Erreur a l'init du mpu6050 :: " );
        Serial.println( errorString );
        for(;;);
    }
    LOGLN( "[  OK  ]" );

#ifdef VERBOSE
    Serial.print( "+-- MPU6050 is \"0x" );
    Serial.print( sensor.whoAmI(), HEX );
    Serial.println( "\"\n" );
#endif

    delay( 1000 ); /*< Pour avoir le temps de lire les infos à l'ecran */
}

/* ######################################################################### */
/* ######################################################################### */

void loop()
{
    Serial.println( "========================================" );


    /* Update the data */
    LOG( "updating sensor's data..........." );
    if( ! sensor.updateData() ) {
        Serial.println( "[ERREUR]" );
        return; /*!< ce return permet de sortir de la fonction loop; cette fonction etant integree dans une boucle infinie, elle sera immediatement rappelee. */
    } else {
        LOGLN( "[OK]" );
    }



    /* Print data */

#ifdef PRINT_ACCEL_RAW
    Serial.print( "raw Ax = " ); Serial.println( sensor.rawAccelX() );
    Serial.print( "raw Ay = " ); Serial.println( sensor.rawAccelY() );
    Serial.print( "raw Az = " ); Serial.println( sensor.rawAccelZ() );
#endif

#ifdef PRINT_ACCEL_G
    Serial.print( "Ax = " ); Serial.print( sensor.accelX_g() );Serial.println( "g" );
    Serial.print( "Ay = " ); Serial.print( sensor.accelY_g() );Serial.println( "g" );
    Serial.print( "Az = " ); Serial.print( sensor.accelZ_g() );Serial.println( "g" );
#endif


#ifdef PRINT_CALCULATED_ANGLES
    Serial.print( "angle X==" ); Serial.print( sensor.angleX_deg(), 6 ); Serial.println( "" );
    Serial.print( "angle Y==" ); Serial.print( sensor.angleY_deg(), 6 ); Serial.println( "" );
    Serial.print( "angle Z==" ); Serial.print( sensor.angleZ_deg(), 6 ); Serial.println( "" );
#endif

#ifdef PRINT_GYRO_RAW
    Serial.print( "raw Rx = " ); Serial.println( sensor.gyroX_deg_s() );
    Serial.print( "raw Ry = " ); Serial.println( sensor.gyroY_deg_s() );
    Serial.print( "raw Rz = " ); Serial.println( sensor.gyroZ_deg_s() );
#endif

#ifdef PRINT_GYRO_DEG_PER_S
    Serial.print( "Rx = " ); Serial.print( sensor.gyroX_deg_s() );Serial.println( "°/s" );
    Serial.print( "Ry = " ); Serial.print( sensor.gyroY_deg_s() );Serial.println( "°/s" );
    Serial.print( "Rz = " ); Serial.print( sensor.gyroZ_deg_s() );Serial.println( "°/s" );
#endif

#ifdef PRINT_TEMPERATURE
    Serial.print(F("Temperature: "));
    Serial.print( sensor.temperature_celsius(), 3 );
    Serial.print(F(" degres Celsius"));
    Serial.println(F(""));
#endif


    delay( 500 );
}

/* ######################################################################### */
/* ######################################################################### */
