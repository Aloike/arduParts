/**
 *  @file   PCA9685.cpp
 *  @brief  Implementation file for PCA9685 class.
 *  @author GOM
 *
 * @copyright    Copyright © 2014 GOM - X11 License\n
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *  +   The above copyright notice and this permission notice shall be included
 *      in all copies or substantial portions of the Software.
 *  +   The Software is provided "as is", without warranty of any kind, express
 *      or implied, including but not limited to the warranties of
 *      merchantability, fitness for a particular purpose and noninfringement.
 *      In no event shall the authors or copyright holders be liable for any
 *      claim, damages or other liability, whether in an action of contract,
 *      tort or otherwise, arising from, out of or in connection with the
 *      Software or the use or other dealings in the Software.
 *  +   Except as contained in this notice, the name(s) of (the) Author shall
 *      not be used in advertising or otherwise to promote the sale, use or
 *      other dealings in this Software without prior written authorization
 *      from (the )Author.
 *
 *  @date   2014-02-28  Fri.    18-26-00    Creation.
 */

#include "PCA9685.h"

#include <Wire.h>

#if 0
#   define DBG( msg ) Serial.println( String( "[debug] PCA9685:  " ) + __FUNCTION__ + " @line " + String( __LINE__ ) + ":  " + msg )
#   define WARN( msg ) Serial.println( String( "[warning] PCA9685:  " ) + __FUNCTION__ + " @line " + String( __LINE__ ) + ":  " + msg )
#else
#   define DBG( msg )
#   define WARN( msg )
#endif


#define CMD_SOFTWARE_RESET  (0x06)  /*!< datasheet p.8 */s


/* ######################################################################### */
/* ######  REGISTER ADDRESSES ############################################## */
/* ######################################################################### */

#define REG_MODE1           (0x00)  /*!< Mode register 1. */
#define REG_MODE2           (0x01)  /*!< Mode register 2. */
#define REG_SUBADR1         (0x02)  /*!< I2C-bus subaddress 1. */
#define REG_SUBADR2         (0x03)  /*!< I2C-bus subaddress 2. */
#define REG_SUBADR3         (0x04)  /*!< I2C-bus subaddress 3. */
#define REG_ALLCALLADR      (0x05)  /*!< LED All Call I2C-bus address. */

#define REG_LED0_ON_L       (0x06)  /*!< LED0 output and brightness control byte 0. */
#define REG_LED0_ON_H       (0x07)  /*!< LED0 output and brightness control byte 1. */
#define REG_LED0_OFF_L      (0x08)  /*!< LED0 output and brightness control byte 2. */
#define REG_LED0_OFF_H      (0x09)  /*!< LED0 output and brightness control byte 3. */

#define REG_LED1_ON_L       (0x0A)  /*!< LED1 output and brightness control byte 0. */
#define REG_LED1_ON_H       (0x0B)  /*!< LED1 output and brightness control byte 1. */
#define REG_LED1_OFF_L      (0x0C)  /*!< LED1 output and brightness control byte 2. */
#define REG_LED1_OFF_H      (0x0D)  /*!< LED1 output and brightness control byte 3. */

#define REG_LED2_ON_L       (0x0E)  /*!< LED2 output and brightness control byte 0. */
#define REG_LED2_ON_H       (0x0F)  /*!< LED2 output and brightness control byte 1. */
#define REG_LED2_OFF_L      (0x10)  /*!< LED2 output and brightness control byte 2. */
#define REG_LED2_OFF_H      (0x11)  /*!< LED2 output and brightness control byte 3. */

#define REG_LED3_ON_L       (0x12)  /*!< LED3 output and brightness control byte 0. */
#define REG_LED3_ON_H       (0x13)  /*!< LED3 output and brightness control byte 1. */
#define REG_LED3_OFF_L      (0x14)  /*!< LED3 output and brightness control byte 2. */
#define REG_LED3_OFF_H      (0x15)  /*!< LED3 output and brightness control byte 3. */

#define REG_LED4_ON_L       (0x16)  /*!< LED4 output and brightness control byte 0. */
#define REG_LED4_ON_H       (0x17)  /*!< LED4 output and brightness control byte 1. */
#define REG_LED4_OFF_L      (0x18)  /*!< LED4 output and brightness control byte 2. */
#define REG_LED4_OFF_H      (0x19)  /*!< LED4 output and brightness control byte 3. */

#define REG_LED5_ON_L       (0x1A)  /*!< LED5 output and brightness control byte 0. */
#define REG_LED5_ON_H       (0x1B)  /*!< LED5 output and brightness control byte 1. */
#define REG_LED5_OFF_L      (0x1C)  /*!< LED5 output and brightness control byte 2. */
#define REG_LED5_OFF_H      (0x1D)  /*!< LED5 output and brightness control byte 3. */

#define REG_LED6_ON_L       (0x1E)  /*!< LED6 output and brightness control byte 0. */
#define REG_LED6_ON_H       (0x1F)  /*!< LED6 output and brightness control byte 1. */
#define REG_LED6_OFF_L      (0x20)  /*!< LED6 output and brightness control byte 2. */
#define REG_LED6_OFF_H      (0x21)  /*!< LED6 output and brightness control byte 3. */

#define REG_LED7_ON_L       (0x22)  /*!< LED7 output and brightness control byte 0. */
#define REG_LED7_ON_H       (0x23)  /*!< LED7 output and brightness control byte 1. */
#define REG_LED7_OFF_L      (0x24)  /*!< LED7 output and brightness control byte 2. */
#define REG_LED7_OFF_H      (0x25)  /*!< LED7 output and brightness control byte 3. */

#define REG_LED8_ON_L       (0x26)  /*!< LED8 output and brightness control byte 0. */
#define REG_LED8_ON_H       (0x27)  /*!< LED8 output and brightness control byte 1. */
#define REG_LED8_OFF_L      (0x28)  /*!< LED8 output and brightness control byte 2. */
#define REG_LED8_OFF_H      (0x29)  /*!< LED8 output and brightness control byte 3. */

#define REG_LED9_ON_L       (0x2A)  /*!< LED9 output and brightness control byte 0. */
#define REG_LED9_ON_H       (0x2B)  /*!< LED9 output and brightness control byte 1. */
#define REG_LED9_OFF_L      (0x2C)  /*!< LED9 output and brightness control byte 2. */
#define REG_LED9_OFF_H      (0x2D)  /*!< LED9 output and brightness control byte 3. */

#define REG_LED10_ON_L      (0x2E)  /*!< LED10 output and brightness control byte 0. */
#define REG_LED10_ON_H      (0x2F)  /*!< LED10 output and brightness control byte 1. */
#define REG_LED10_OFF_L     (0x30)  /*!< LED10 output and brightness control byte 2. */
#define REG_LED10_OFF_H     (0x31)  /*!< LED10 output and brightness control byte 3. */

#define REG_LED11_ON_L      (0x32)  /*!< LED11 output and brightness control byte 0. */
#define REG_LED11_ON_H      (0x33)  /*!< LED11 output and brightness control byte 1. */
#define REG_LED11_OFF_L     (0x34)  /*!< LED11 output and brightness control byte 2. */
#define REG_LED11_OFF_H     (0x35)  /*!< LED11 output and brightness control byte 3. */

#define REG_LED12_ON_L      (0x36)  /*!< LED12 output and brightness control byte 0. */
#define REG_LED12_ON_H      (0x37)  /*!< LED12 output and brightness control byte 1. */
#define REG_LED12_OFF_L     (0x38)  /*!< LED12 output and brightness control byte 2. */
#define REG_LED12_OFF_H     (0x39)  /*!< LED12 output and brightness control byte 3. */

#define REG_LED13_ON_L      (0x3A)  /*!< LED13 output and brightness control byte 0. */
#define REG_LED13_ON_H      (0x3B)  /*!< LED13 output and brightness control byte 1. */
#define REG_LED13_OFF_L     (0x3C)  /*!< LED13 output and brightness control byte 2. */
#define REG_LED13_OFF_H     (0x3D)  /*!< LED13 output and brightness control byte 3. */

#define REG_LED14_ON_L      (0x3E)  /*!< LED14 output and brightness control byte 0. */
#define REG_LED14_ON_H      (0x3F)  /*!< LED14 output and brightness control byte 1. */
#define REG_LED14_OFF_L     (0x40)  /*!< LED14 output and brightness control byte 2. */
#define REG_LED14_OFF_H     (0x41)  /*!< LED14 output and brightness control byte 3. */

#define REG_LED15_ON_L      (0x42)  /*!< LED15 output and brightness control byte 0. */
#define REG_LED15_ON_H      (0x43)  /*!< LED15 output and brightness control byte 1. */
#define REG_LED15_OFF_L     (0x44)  /*!< LED15 output and brightness control byte 2. */
#define REG_LED15_OFF_H     (0x45)  /*!< LED15 output and brightness control byte 3. */


#define REG_ALL_LED_ON_L    (0xFA)  /*!< Load all the LEDn_ON registers, byte 0. */
#define REG_ALL_LED_ON_H    (0xFB)  /*!< Load all the LEDn_ON registers, byte 1. */
#define REG_ALL_LED_OFF_L   (0xFC)  /*!< Load all the LEDn_OFF registers, byte 0. */
#define REG_ALL_LED_OFF_H   (0xFD)  /*!< Load all the LEDn_OFF registers, byte 1. */


#define REG_PRE_SCALE       (0xFE)  /*!< Prescaler for output frequency. */

#define REG_TESTMODE        (0xFF)  /*!< Defines the test mode to be entered. */

/* ######################################################################### */
/* ######################################################################### */

/* Defines to easily select bits of a byte */
#define BIT_7           (0x80)
#define BIT_6           (0x40)
#define BIT_5           (0x20)
#define BIT_4           (0x10)
#define BIT_3           (0x08)
#define BIT_2           (0x04)
#define BIT_1           (0x02)
#define BIT_0           (0x01)
#define MASK_BITS_ALL   (0xFF)  /*!< All 8 bits */



#define MASK_MODE1_RESTART  BIT_7   /*!< Read: Shows state of RESTART logic. Write: see datasheet section 7.3.1.1. */
#define MASK_MODE1_EXTCLK   BIT_6   /*!< To use the EXTCLK pin. See datasheet. */
#define MASK_MODE1_AI       BIT_5   /*!< Register Auto-Increment. Disabled if value = 0, enabled if 1. */
#define MASK_MODE1_SLEEP    BIT_4   /*!< @value 0 if normal mode, @value 1 if low-power (oscillator off). */
#define MASK_MODE1_SUB1     BIT_3
#define MASK_MODE1_SUB2     BIT_2
#define MASK_MODE1_SUB3     BIT_1
#define MASK_MODE1_ALLCALL  BIT_0   /*!< PCA9685 responds to "LED All Call" I2C Bus Address. */


#define MASK_MODE2_RESERVED (BIT_7|BIT_6|BIT_5) /*!< Reserved bits */
#define MASK_MODE2_INVRT    BIT_4               /*!< To invert output logic's state. Applicable when /OE=0 */
#define MASK_MODE2_OCH      BIT_3               /*!< To set whether output changes on acknowledge (bit set) or on STOP command (bit cleared). */
#define MASK_MODE2_OUTDRV   BIT_2
#define MASK_MODE2_OUTNE1   BIT_1               /*!< If bit is set, when /OE=1 all outputs are high-impedance. */
#define MASK_MODE2_OUTNE0   BIT_0               /*!< If MASK_MODE2_OUTNE1 bit set, no influence on chip's config. Else... see datasheet. :) */


/* The following masks are the same through all the LEDn + ALL_LED registers. */
#define MASK_LEDn_ON_L_LEDON_COUNT_LSB      MASK_BITS_ALL               /*!< LEDn_ON count, 8 LSBs. */
#define MASK_LEDn_ON_H_RESERVED             (BIT_7|BIT_6|BIT5)          /*!< Reserved bits. */
#define MASK_LEDn_ON_H_FULLON               BIT_4                       /*!< To set LEDn to be fully on. Overrides \ref MASK_LEDn_OFF_H_FULLOFF */
#define MASK_LEDn_ON_H_LEDON_COUNT_MSB      (BIT_3|BIT_2|BIT_1|BIT_0)   /*!< LEDn_ON count, 4 MSBs. */
#define MASK_LEDn_OFF_L_LEDOFF_COUNT_LSB    MASK_BITS_ALL               /*!< LEDn_OFF count, 8 LSBs. */
#define MASK_LEDn_OFF_H_RESERVED            (BIT_7|BIT_6|BIT5)          /*!< Reserved bits. */
#define MASK_LEDn_OFF_H_FULLOFF             BIT_4                       /*!< To set LEDn to be fully off. Is overriden by \ref MASK_LEDn_ON_H_FULLON */
#define MASK_LEDn_OFF_H_LEDOFF_COUNT_MSB    (BIT_3|BIT_2|BIT_1|BIT_0)   /*!< LEDn_OFF count, 4 MSBs. */


#define MASK_PRE_SCALE  MASK_BITS_ALL   /*!< Prescaler to program the output frequency. */


/* The following masks are the same for all the SUBADRn registers. */
#define MASK_SUBADRn_ADDRESS    (BIT_7|BIT_6|BIT_5|BIT_4|BIT_3|BIT_2|BIT_1) /*!< I2C subaddress n */
#define MASK_SUBADRn_RESERVED   (BIT_0)                                     /*!< Reserved bit. */


#define MASK_ALLCALLADR_ADDRESS     (BIT_7|BIT_6|BIT_5|BIT_4|BIT_3|BIT_2|BIT_1) /*!<  ALLCALL I2C bus address register. */
#define MASK_ALLCALLADR_RESERVED    (BIT_0)                                     /*!< Reserved bit. */




/* ######################################################################### */
/* ######################################################################### */

/* Macros to manage bits */
/** Convenience macro to clear bits from a \p mask into a \p byte */
#define M_BIT_CLEAR( byte, mask )    ( byte   &=  ~(mask) )

/** Convenience macro to set bits from a \p mask into a \p byte */
#define M_BIT_SET( byte, mask )    ( byte   |=  (mask) )

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Constructor; sets needed parameters.
 * @param argAddress The address of the PCA9685. Range: [0x40;0x7F].
 * @param argClockFrequency The clock frequency of the device counter.
 *
\p argAddress is set to PCA9685::DEFAULT_ADDRESS by default, but it will depend
on the hardware. See datasheet for details.

\p argClockFrequency is not absolutely needed to communicate with the device,
it is used for some of this class' methods to perform operations such as
calculation of the needed prescaler when setting output frequency with
PCA9685::setOutputFrequency() or, the opposite way, to calculate the output's
frequency from the read prescale value.
If using the internal oscillator, frequency is 25MHz
(set in PCA9685::DEFAULT_CLOCKFREQUENCY_HZ); otherwise it shall be set to the
external clock frequency. See datasheet for applyable range.
 */
PCA9685::PCA9685(uint8_t argAddress, double argClockFrequency)
    :   m_address( argAddress )
    ,   m_clockFrequency( argClockFrequency )
{
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To get the address that is used to communicate with
 * the PCA9685. Set by constructor.
 *
 * @return The device's address.
 */
uint8_t PCA9685::address() const
{
    return this->m_address;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To know whether the device can be restored from a
 * previous state, for example after the device has been put to sleep.
 *
 * @return true     if the device can be "restarted".
 * @return false    if no previous state can be restored.
 *
 * @note    Conditions that could make the device not to be restored:
 *          +   Power cycle
 *          +   I2C software reset command
 *          +   If the MODE2 OCH bit is logic 0, write to any PWM register then
 *              issue an I2C-bus STOP.
 *          +   If the MODE2 OCH bit is logic 1, write to all four PWM
 *              registers in any PWM channel.
 */
bool    PCA9685::canRestart()
{
    return (this->read_8u( REG_MODE1 ) & MASK_MODE1_RESTART) != 0;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Returns the PCA9685's outputs state of when its /OE pin is set to
 * logical 1 (==output disabled).
 * @return a value from the PCA9685::OutputPolicyWhenDisabled enum.
 */
PCA9685::OutputPolicyWhenDisabled PCA9685::disabledOutputPolicy()
{
    uint8_t reg_content = this->read_8u( REG_MODE2 );

    if( (reg_content & MASK_MODE2_OUTNE1) == 0 ) {
        if( (reg_content & MASK_MODE2_OUTNE0) == 0 ) {
            return OutputFalse;
        } else {
            return OutputTrue;
        }
    } else {
        return OutputHighImpedance;
    }
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Returns the frequency of the clock as set by
 * the constructor.
 *
 * @return  The frequency of the clock (Hertz).
 */
double  PCA9685::clockFrequency() const
{
    return this->m_clockFrequency;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Sets up the initial conditions to let the device work.
 *
 * Begins Wire (I2C interface) and resets the device.
 */
void PCA9685::init()
{
    Wire.begin();
    this->reset();
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Returns whether the device will auto-increment registers number (when
 * multiple-read/write) or not.
 *
 * @return  true    if auto-increment of register number is enabled;
 * @return  false   if auto-increment is disabled.
 */
bool    PCA9685::isAutoIncrementingRegister()
{
    return (this->read_8u( REG_MODE1 ) & MASK_MODE1_AI) != 0;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Returns the the device's power mode.
 * @return  **true**    if the device is in low-power mode (internal oscillator off)
 * @return  **false**   if the device is in normal mode.
 */
bool    PCA9685::isSleeping()
{
    return (this->read_8u( REG_MODE1 ) & MASK_MODE1_SLEEP) != 0;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Returns the MODE1_EXTCLK bit state.
 * @return  true    if the device is using external clock input.
 * @return  false   if the device is using internal oscillator as clock source.
 *
 * To know whether the internal counter is using internal clock source
 * (oscillator) or external clock (EXTCLK pin).
 */
bool    PCA9685::isUsingExternalClock()
{
    return (this->read_8u( REG_MODE1 ) & MASK_MODE1_EXTCLK) != 0;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief
 * @return  true    if the device will answer to Led All Call address;
 * @return  false   otherwise.
 */
bool    PCA9685::ledAllCallAddressEnabled()
{
    return (this->read_8u( REG_MODE1 ) & MASK_MODE1_ALLCALL) != 0;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Returns the event type on which the output will be changed.
 * @return
 *
 * @see PCA9685::OutputChangeEvent
 */
PCA9685::OutputChangeEvent  PCA9685::outputChangeEvent()
{
    if( (this->read_8u( REG_MODE2 ) & MASK_MODE2_OCH) == 0 ) {
        return ChangeOnStop;
    } else {
        return ChangeOnAcknowledge;
    }
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Returns the PWM output frequency, calculated from the chip's
 * prescaler and the PCA9685::m_clockFrequency.
 *
 * @see PCA9685::setOutputFrequency()
 */
double  PCA9685::outputFrequency()
{
    return (this->clockFrequency() / ((this->outputPrescale() + 1) * 4096.0) );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To get the value of the prescale that will be used to divide internal
 * counter's input clock. Sets the output frequency.
 * @return uint8_t The prescale value.
 */
uint8_t PCA9685::outputPrescale()
{
    return this->read_8u( REG_PRE_SCALE );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To know whether the output logical state is inverted or not.
 *  @return true    if output logical state is inverted.
 *  @return false   otherwise.
 */
bool    PCA9685::outputStateInverted()
{
    return (this->read_8u( REG_MODE2 ) & MASK_MODE2_INVRT) != 0;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief   To get the output driver's internal structure type.
 * @return
 */
PCA9685::OutputStructure    PCA9685::outputStructure()
{
    if( (this->read_8u( REG_MODE2 ) & MASK_MODE2_OUTDRV) == 0 ) {
        return OutputStructureOpenDrain;
    } else {
        return OutputStructureTotemPole;
    }
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Convenience function to read a 8-bits unisgned data from the register
 * \p argAddress of the device at address PCA9685::m_address.
 *
 * @param argAddress    The uint8_t address of the register we want to read.
 * @return  uint8_t the read byte.
 */
uint8_t PCA9685::read_8u(const uint8_t &argAddress)
{
    Wire.beginTransmission( this->m_address );
    Wire.write( argAddress );
    Wire.endTransmission();

    Wire.requestFrom( this->m_address, (uint8_t)CMD_MASK_READ );
    return Wire.read();
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Sends a Software Reset Call (SWRST Call) command on the I2C bus.
 *
 * @warning May interfere with other devices - it looks like a standard I2C
 * command...
 */
void PCA9685::reset()
{
    Wire.beginTransmission( 0x00 ); /*< General Call I2C address */
    Wire.write( CMD_SOFTWARE_RESET );
    Wire.endTransmission();
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Wakes up the device and restarts all of the previously active PWM
 * channels if any.
 *
 * @return  false   If the device can't restart ( see \ref PCA9685::canRestart()
 * for more details);
 * @return  true    otherwise.
 *
 * @see PCA9685::canRestart()
 */
bool    PCA9685::restart()
{

    if( ! this->canRestart() ) {
        /* Can't restart */
        return false;
    }

    /* Clear SLEEP bit */
    this->setSleeping( false );
//    uint8_t reg_mode1_content = this->read_8u( REG_MODE1 );
//    M_BIT_CLEAR( reg_mode1_content, MASK_MODE1_SLEEP );
//    this->write_8u( REG_MODE1, reg_mode1_content );

    /* The SLEEP bit must be logic 0 for at least 500 μs */
    delayMicroseconds( 500 );

    /* Set RESTART bit to logic 1 */
    uint8_t reg_mode1_content = this->read_8u( REG_MODE1 );
    M_BIT_SET( reg_mode1_content, MASK_MODE1_RESTART );
    this->write_8u( REG_MODE1, reg_mode1_content );

    return true;
}

/* ######################################################################### */
/* ######################################################################### */
#if 0

void    PCA9685::setAllOff()
{
    /* set ALL_LED_OFF_H[4] bit */
    uint8_t reg_val = this->read_8u( REG_ALL_LED_OFF_H );
    M_BIT_SET( reg_val, MASK_LEDn_OFF_H_FULLOFF );
    this->write_8u( MASK_LEDn_OFF_H_FULLOFF, reg_val );

    /* Clear ALL_LED_ON_H[4] bit (otherwise it overrides ALL_LED_OFF_H[4]) */
    reg_val = this->read_8u( REG_ALL_LED_ON_H );
    M_BIT_CLEAR( reg_val, MASK_LEDn_ON_H_FULLON );
    this->write_8u( REG_ALL_LED_ON_H, reg_val );
}

/* ######################################################################### */
/* ######################################################################### */

void    PCA9685::setAllOn()
{
    uint8_t reg_val = this->read_8u( REG_ALL_LED_ON_H );

    M_BIT_SET( reg_val, MASK_LEDn_ON_H_FULLON );

    this->write_8u( REG_ALL_LED_ON_H, reg_val );
}
#endif

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To set whether the device will auto-increment registers number (when
 * multiple-read/write) or not.
 *
 * @param   argEnableAutoIncrement The auto-increment status (true: enabled,
 * false:disabled).
 */
void    PCA9685::setAutoIncrementRegister(bool argEnableAutoIncrement)
{
    uint8_t reg_content = this->read_8u( REG_MODE1 );

    if( argEnableAutoIncrement ) {
        M_BIT_SET( reg_content, MASK_MODE1_AI );
    } else {
        M_BIT_CLEAR( reg_content, MASK_MODE1_AI );
    }

    this->write_8u( REG_MODE1, reg_content );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To set whether the device will answer to "Led All Call" address.
 * @param argEnabled
 *          + **true**  The device will answer;
 *          + **false** The device won't answer.
 */
void    PCA9685::setLedAllCallAddressEnabled(bool argEnabled)
{
    uint8_t reg_content = this->read_8u( REG_MODE1 );

    if( argEnabled ) {
        M_BIT_SET( reg_content, MASK_MODE1_ALLCALL );
    } else {
        M_BIT_CLEAR( reg_content, MASK_MODE1_ALLCALL );
    }

    this->write_8u( REG_MODE1, reg_content );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To set the PCA9685's outputs state for when the /OE pin is set to
 * logical 1 (==device's output disabled).
 *
 * @param argPolicy The policy to be set.
 *
 * @see OutputPolicyWhenDisabled
 */
void    PCA9685::setDisabledOutputPolicy(OutputPolicyWhenDisabled argPolicy)
{
    uint8_t reg_content = this->read_8u( REG_MODE2 );

    switch( argPolicy )
    {
        default:
        case OutputFalse:
            M_BIT_CLEAR( reg_content, MASK_MODE2_OUTNE1 );
            M_BIT_CLEAR( reg_content, MASK_MODE2_OUTNE0 );
            break;

        case OutputHighImpedance:
            M_BIT_SET( reg_content, MASK_MODE2_OUTNE1 );
            break;

        case OutputTrue:
            M_BIT_CLEAR( reg_content, MASK_MODE2_OUTNE1 );
            M_BIT_SET(   reg_content, MASK_MODE2_OUTNE0 );
            break;
    }

    this->write_8u( REG_MODE2, reg_content );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * \fn bool PCA9685::setOutput(uint8_t argOutputNbr, uint16_t argOn, uint16_t argOff)
 * \brief To set the cycle & phase of an output.
 * \param argOutputNbr  The output to control. Range: [0;15]
 * \param argOn     The value of the internal timer at which the output will be enabled.  Range: [0;4095].
 * \param argOff    The value of the internal timer at which the output will be disabled. Range: [0;4095].
 * \return  false   if a parameter error has been detected
 * \return  true    otherwise.

Abstract from the datasheet:\n
The turn-on time of each LED driver output and the duty cycle of PWM can be
controlled independently using the LEDn_ON and LEDn_OFF registers.\n
There will be two 12-bit registers per LED output. These registers will be
programmed by the user. Both registers will hold a value from 0 to 4095. One
12-bit register will hold a value for the ON time and the other 12-bit register
will hold the value for the OFF time. The ON and OFF times are compared with
the value of a 12-bit counter that will be running continuously from 0000h to
0FFFh (0 to 4095 decimal).\n
The ON time, which is programmable, will be the time the LED output will be
asserted and the OFF time, which is also programmable, will be the time when
the LED output will be negated. In this way, the phase shift becomes completely
programmable. The resolution for the phase shift is 1⁄4096 of the target
frequency.
 */
bool    PCA9685::setOutput(uint8_t argOutputNbr, uint16_t argOn, uint16_t argOff)
{
    if( argOutputNbr >= OUTPUT_COUNT ) {
        return false;
    }

    if( argOn > 4095 )
        return false;

    if( argOff > 4095 )
        return false;


    this->write_8u( REG_LED0_ON_L  + 4 * argOutputNbr, argOn );         /*< Write the LSBs */
    this->write_8u( REG_LED0_ON_H  + 4 * argOutputNbr, argOn >> 8 );    /*< Write the MSBs */
    this->write_8u( REG_LED0_OFF_L + 4 * argOutputNbr, argOff );        /*< Write the LSBs */
    this->write_8u( REG_LED0_OFF_H + 4 * argOutputNbr, argOff >> 8 );   /*< Write the MSBs */

    return true;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief   To set the event type on which the output will be changed.
 *
 * @param   argEvent    The type of event.
 *
 * @see PCA9685::OutputChangeEvent
 */
void    PCA9685::setOutputChangeEvent(OutputChangeEvent argEvent)
{
    uint8_t reg_content = this->read_8u( REG_MODE2 );

    if( argEvent == ChangeOnStop )
        M_BIT_CLEAR( reg_content, MASK_MODE2_OCH );
    else
        M_BIT_SET( reg_content, MASK_MODE2_OCH );

    this->write_8u( REG_MODE2, reg_content );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Calculates then sets internal counter's prescale value to let the
 * output's frequency be as close as possible to \p argFrequency_Hz.
 *
 * The outputs frequency depends on the internal counter's prescale value and
 * its clock source's frequency.\n
 * This function calculates the reachable frequency range using
 * \ref PCA9685::clockFrequency(), checks if \p argFrequency_Hz is included in
 * it, then calculates and sets the prescale value to be the closest to
 * \p argFrequency_Hz.
 *
 * @param argFrequency_Hz   The target output frequency.
 *
 * @return  **true**    If \p argFrequency_Hz is in the reachable range;
 * @return  **false**   otherwise.
 *
 * @see PCA9685::clockFrequency(), PCA9685::isUsingExternalClock(),
 * PCA9685::outputPrescale()
 */
bool    PCA9685::setOutputFrequency(double argFrequency_Hz)
{
    double maxFrequency  = (this->clockFrequency() / 4096.0 );
    double minFrequency  = (this->clockFrequency() / ((0xFF + 1) * 4096.0) );

    DBG( String( "minFrequency: ") + String( (long) minFrequency ) + "Hz" );
    DBG( String( "maxFrequency: ") + String( (long) maxFrequency ) + "Hz" );


    /* Check that argFrequency_Hz is in a reachable range */
    if( argFrequency_Hz > maxFrequency ) {
        WARN( "given frequency is out of range (higher than max reachable frequency)" );
        WARN( String( (long) argFrequency_Hz )
              + "(given) > " + String( (long)maxFrequency ) + "(max)" );
        return false;
    }

    if( argFrequency_Hz < minFrequency ) {
        WARN( "given frequency is out of range (lower than lowest reachable frequency)" );
        WARN( String( (long) argFrequency_Hz )
              + "(given) < " + String( (long)minFrequency ) + "(min)" );
        return false;
    }

    float prescaleValue
            = ( this->clockFrequency() / (4096.0 * argFrequency_Hz) ) - 1;
    DBG( "Estimated prescale == " + (long)prescaleValue );
    uint8_t prescale = floor( prescaleValue + 0.5 );
    DBG( "Final pre-scale == " + prescale );

    this->setOutputPrescale( prescale );
    return true;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To set the value of the prescale that will be used to divide internal
 * counter's input clock.
 *
 * @param argPrescale   The new prescale value. Range [0x00;0xFF] / [0;255].
 */
void    PCA9685::setOutputPrescale(uint8_t argPrescale)
{
    /* 1- Go to sleep if PCA9685 was awake */
    bool wasSleeping   = this->isSleeping();
    if( ! wasSleeping )
        this->setSleeping( true );

    /* 2- Set the prescale value */
    this->write_8u( REG_PRE_SCALE, argPrescale );

    /* 3- Restore sleep state */
    this->setSleeping( wasSleeping );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To set wether the output's logical state is inverted or not.
 * Applicable when /OE pin==0.
 *
 * @param argInverted
 *          + **true**  The output's logical state will be inverted;
 *          + **false** The output's logical state will NOT be inverted.
 */
void    PCA9685::setOutputStateInverted(bool argInverted)
{
    uint8_t reg_content = this->read_8u( REG_MODE2 );

    if( argInverted ) {
        M_BIT_SET( reg_content, MASK_MODE2_INVRT );
    } else {
        M_BIT_CLEAR( reg_content, MASK_MODE2_INVRT );
    }

    this->write_8u( REG_MODE2, reg_content );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To set the output driver's internal structure type.
 *
 * @param argStructure The structure type.
 *
 * @see PCA9685::OutputStructure
 */
void    PCA9685::setOutputStructure(OutputStructure argStructure)
{
    uint8_t reg_content = this->read_8u( REG_MODE2 );

    if( argStructure == OutputStructureOpenDrain )
        M_BIT_CLEAR( reg_content, MASK_MODE2_OUTDRV );
    else
        M_BIT_SET( reg_content, MASK_MODE2_OUTDRV );

    this->write_8u( REG_MODE2, reg_content );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Sets the device's power mode.
 *
 * When waking up the device (\p argIsSleeping==false) a delay of 500
 * microseconds is applied to avoid erroneous control registers readings.
 *
 * @note
 * It takes the oscillator up to 500µs to be up and running when waking up.
 * Timings on LEDn outputs are not guaranteed if PWM control registers are
 * accessed within the 500 μs window.
 * \par
 * @note There is no start-up delay required when using the EXTCLK pin as the PWM
 * clock.
 * \par
 * @note No PWM control is possible when the oscillator is off. When the
 * oscillator is off (Sleep mode) the LEDn outputs cannot be turned on, off or
 * dimmed/blinked.
 *
 * @param argIsSleeping
 *          +   **true**    Let the device go to sleep.
 *          +   **false**   Wake up the device.
 */
void    PCA9685::setSleeping(bool argIsSleeping)
{
    uint8_t reg_content = this->read_8u( REG_MODE1 );

    if( argIsSleeping ) {
        M_BIT_SET( reg_content, MASK_MODE1_SLEEP );
    } else {
        M_BIT_CLEAR( reg_content, MASK_MODE1_SLEEP );
    }

    /* Write register's content */
    this->write_8u( REG_MODE1, reg_content );

    if( argIsSleeping == false ) {
        /* Because it takes the oscillator up to 500µs to wake up, we apply a
         * delay to be shure we won't do erroneous readings on PWM control
         * registers. */
        delayMicroseconds( 500 );
    }
}

/* ######################################################################### */
/* ######################################################################### */

void    PCA9685::setSubAddress1Enabled(bool argEnabled)
{
    uint8_t reg_content = this->read_8u( REG_MODE1 );

    if( argEnabled ) {
        M_BIT_SET( reg_content, MASK_MODE1_SUB1 );
    } else {
        M_BIT_CLEAR( reg_content, MASK_MODE1_SUB1 );
    }

    this->write_8u( REG_MODE1, reg_content );
}

/* ######################################################################### */
/* ######################################################################### */

void    PCA9685::setSubAddress2Enabled(bool argEnabled)
{
    uint8_t reg_content = this->read_8u( REG_MODE1 );

    if( argEnabled ) {
        M_BIT_SET( reg_content, MASK_MODE1_SUB2 );
    } else {
        M_BIT_CLEAR( reg_content, MASK_MODE1_SUB2 );
    }

    this->write_8u( REG_MODE1, reg_content );
}

/* ######################################################################### */
/* ######################################################################### */

void    PCA9685::setSubAddress3Enabled(bool argEnabled)
{
    uint8_t reg_content = this->read_8u( REG_MODE1 );

    if( argEnabled ) {
        M_BIT_SET( reg_content, MASK_MODE1_SUB3 );
    } else {
        M_BIT_CLEAR( reg_content, MASK_MODE1_SUB3 );
    }

    this->write_8u( REG_MODE1, reg_content );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To use an external clock source instead
 * of the internal one.
 *
 * @warning the EXTCLK bit from MODE1 is a "sticky bit", it can ONLY be cleared
 * by a power cycle or a software reset.
 */
void    PCA9685::setUseExternalClock(void)
{
    uint8_t reg_content = this->read_8u( REG_MODE1 );

    /* 1 - Turn off internal oscillator */
    M_BIT_SET( reg_content, MASK_MODE1_SLEEP );
    this->write_8u( REG_MODE1, reg_content );

    /* 2 - Enable external clock and resume. */
    M_BIT_SET( reg_content, MASK_MODE1_EXTCLK );
    M_BIT_SET( reg_content, MASK_MODE1_SLEEP );
    this->write_8u( REG_MODE1, reg_content );
}

/* ######################################################################### */
/* ######################################################################### */

bool    PCA9685::subAddress1Enabled()
{
    return (this->read_8u( REG_MODE1 ) & MASK_MODE1_SUB1) != 0;
}

/* ######################################################################### */
/* ######################################################################### */

bool    PCA9685::subAddress2Enabled()
{
    return (this->read_8u( REG_MODE1 ) & MASK_MODE1_SUB2) != 0;
}

/* ######################################################################### */
/* ######################################################################### */

bool    PCA9685::subAddress3Enabled()
{
    return (this->read_8u( REG_MODE1 ) & MASK_MODE1_SUB3) != 0;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Convenience function to write a 8-bits
 * unisgned \p argData into the register \p argAddress of the device at address
 * PCA9685::m_address.
 *
 * @param argAddress    The uint8_t register's address where we want to write.
 * @param argData  The uint8_t data we want to write to \p argAddress register.
 */
void PCA9685::write_8u(const uint8_t &argAddress, const uint8_t &argData)
{
    Wire.beginTransmission( this->m_address );
    Wire.write( argAddress );
    Wire.write( argData );
    Wire.endTransmission();
}

/* ######################################################################### */
/* ######################################################################### */
