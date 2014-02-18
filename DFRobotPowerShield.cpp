/*!
 *  @file   DFRobotPowerShield.cpp
 *  @brief  Defininitions of methods declared in file DFRobotPowerShield.h
 *
 *  @author MGO
 */

#include "DFRobotPowerShield.h"

#include <Arduino.h>


/* ######################################################################### */
/* ######################################################################### */

#define PIN_AUXILIARYOUTPUT_CONTROL 13  /*!< The digital pin on which the control of the auxiliary output on/off switch is connected - is the n°13 digital pin in normal operating conditions. */
#define PIN_AUXILIARYOUTPUT_VOLTAGE 0   /*!< The analog pin on which the power shield's built-in voltage measuring function is connected - is the n°0 analog pin in normal operating conditions. */

#define AUXILIARYOUTPUT_CONTROL_ON  LOW
#define AUXILIARYOUTPUT_CONTROL_OFF HIGH

/* ######################################################################### */
/* ######################################################################### */

DFRobotPowerShield::DFRobotPowerShield()
    :   m_voltageMin( 0 )
    ,   m_voltageMax( 25.6 )
{
    /* Initialize input/outputs */
    this->init();
}

/* ######################################################################### */
/* ######################################################################### */

/*!
 * \brief DFRobotPowerShield::init  Initialisation of in/out pins.
 */
void    DFRobotPowerShield::init()
{
    pinMode( PIN_AUXILIARYOUTPUT_CONTROL, OUTPUT );
    pinMode( PIN_AUXILIARYOUTPUT_VOLTAGE, INPUT );

    this->setOutputEnabled( false );
}

/* ######################################################################### */
/* ######################################################################### */

/*!
 * \brief DFRobotPowerShield::outputEnabled Returns the state of the auxiliary
 * output as controlled by the PIN_AUXILIARYOUTPUT_CONTROL pin.
 *
 * \return true     if the output is enabled,
 * \return false    otherwise.
 */
bool    DFRobotPowerShield::outputEnabled() const
{
    bool    ret = false;

    if( digitalRead( PIN_AUXILIARYOUTPUT_CONTROL ) == AUXILIARYOUTPUT_CONTROL_ON ) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

/* ######################################################################### */
/* ######################################################################### */

/*!
 * \brief DFRobotPowerShield::setOutputEnabled To set whether the auxiliary
 * power output is enabled or not.
 *
 * \param argEnabled    The new state of the output (true=enabled, false=disabled).
 */
void    DFRobotPowerShield::setOutputEnabled(const bool &argEnabled)
{
    if( argEnabled == true ) {
        digitalWrite( PIN_AUXILIARYOUTPUT_CONTROL, AUXILIARYOUTPUT_CONTROL_ON );
    } else {
        digitalWrite( PIN_AUXILIARYOUTPUT_CONTROL, AUXILIARYOUTPUT_CONTROL_OFF );
    }
}

/* ######################################################################### */
/* ######################################################################### */

/*!
 * \brief DFRobotPowerShield::setVoltageRange To modify the voltage range
 * measured by AN0; as the auxyliary output's voltage is measured through a
 * pont diviseur de tension, the exact value may vary depending on the
 * resistors. Use this method to adjust the min/max to best fit to the board.
 *
 * \param argMin    The voltage of the auxiliary output when AN0 value is 0/1024.
 * \param argMax    The voltage of the auxiliary output when AN0 value is 1024/1024.
 */
void    DFRobotPowerShield::setVoltageRange(float argMin, float argMax)
{
    this->m_voltageMin  = argMin;
    this->m_voltageMax  = argMax;
}

/* ######################################################################### */
/* ######################################################################### */

/*!
 * \brief DFRobotPowerShield::voltage
 * @todo    Check with diffenrent input voltages this function's output
 * \return
 */
float   DFRobotPowerShield::voltage() const
{
    float raw   = analogRead( PIN_AUXILIARYOUTPUT_VOLTAGE );

//    Serial.print( "DFRobotPowerShield::voltage::analog == " );
//    Serial.println( raw );

//    return map( raw * 100,
//                0.0, 102400.0,
//                this->m_voltageMin * 100.0, this->m_voltageMax * 100.0 ) / 100.0;

    /* Equivalent to map() but uses float (not that good because of divisions
     * but shall be ok) */
    return  this->m_voltageMin
            + (this->m_voltageMax - this->m_voltageMin )
            * ( raw / 1024.0) ;
}

/* ######################################################################### */
/* ######################################################################### */
