/**
 *  @file   MAX1704X.cpp
 *  @brief  Implementation file for MAX1704X class.
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
 *  @date   2014-03-06  Thu.    18-30-00    Creation.
 */

#include "MAX1704X.h"

#include <Wire.h>


/* ######################################################################### */
/* ######################################################################### */

/* Macros to manage bits */
/** Convenience macro to clear bits from a \p mask into a \p byte */
#define M_BIT_CLEAR( byte, mask )    ( byte   &=  ~(mask) )

/** Convenience macro to set bits from a \p mask into a \p byte */
#define M_BIT_SET( byte, mask )    ( byte   |=  (mask) )

/* ######################################################################### */
/* ######################################################################### */

/* Defines to easily select bits of a byte */
#define BIT_0           (0x01)
#define BIT_1           (0x02)
#define BIT_2           (0x04)
#define BIT_3           (0x08)
#define BIT_4           (0x10)
#define BIT_5           (0x20)
#define BIT_6           (0x40)
#define BIT_7           (0x80)
#define MASK_BITS_ALL8  (0xFF)  /*!< All 8 bits */
#define BIT_8           (0x0100)
#define BIT_9           (0x0200)
#define BIT_10          (0x0400)
#define BIT_11          (0x0800)
#define BIT_12          (0x1000)
#define BIT_13          (0x2000)
#define BIT_14          (0x4000)
#define BIT_15          (0x8000)
#define MASK_BITS_ALL16 (0xFFFF)  /*!< All 16 bits */
#define MASK_BITS_MSB16 (0xFF00)  /*!< Only the 8 MSBs of a 16bits value */



#define MAX1704X_VCELL_H_REGADDR    (0x02)
// #define MAX1704X_VCELL_L_REGADDR    (0x03)


#define MAX1704X_SOC_H_REGADDR      (0x04)  /*!< Address of the register containing the MSBs of the 16bits value representing the state of charge (in percents). This register contains the integer part of the value. */
#define MAX1704X_SOC_L_REGADDR      (0x05)  /*!< Address of the register containing the LSBs of the 16bits value representing the state of charge (in percents). This register contains the decimal part of the value. */
#   define MAX1704X_SOC_REGADDR_INT MAX1704X_SOC_H_REGADDR
#   define MAX1704X_SOC_REGADDR_DEC MAX1704X_SOC_L_REGADDR


#define MAX1704X_MODE_H_REGADDR     (0x06)  /*!< Address of the register containing the MODE MSBs */
// #define MAX1704X_MODE_L_REGADDR     (0x07)  /*!< Address of the register containing the MODE LSBs */
#   define MAX1704X_MODE_CMD_QUICKSTART (BIT_14)     /*!< 4000h is the command to write in MODE register to perform a quick-start. */


#define MAX1704X_VERSION_H_REGADDR    (0x08)    /*!< Address of the register containing the MSBs of the IC's version. */
// #define MAX1704X_VERSION_L_REGADDR    (0x09)    /*!< Address of the register containing the LSBs of the IC's version. */


#define MAX1704X_CONFIG_REGADDR    (0x0C)  /*!< Address of the register containing the RCOMP value of the IC's configuration. */
#   define MAX1704X_CONFIG_MASK_RCOMP   (MASK_BITS_MSB16)
//#define MAX1704X_CONFIG_REGADDR         (0x0D)  /*!< Address of the register containing the LSBs of the IC's configuration. */
#   define MAX1704X_CONFIG_MASK_SLEEP   (BIT_7)
#   define MAX1704X_CONFIG_MASK_UNUSED  (BIT_6)
#   define MAX1704X_CONFIG_MASK_ALRT    (BIT_5)
#   define MAX1704X_CONFIG_MASK_ATHD    (BIT_4|BIT_3|BIT_2|BIT_1|BIT_0) /*!< Mask for CONFIG register's Alert Threshold. */


#define MAX1704X_COMMAND_H_REGADDR  (0xFE)  /*!< Address of the register containing the COMMAND MSBs */
//#define MAX1704X_COMMAND_L_REGADDR  (0xFF)  /*!< Address of the register containing the COMMAND LSBs */
#   define MAX1704X_COMMAND_CMD_POR     (BIT_14|BIT_12|BIT_10)  /*!< 5400h is the command to write in COMMAND register to reset the chip. */


/* ######################################################################### */
/* ######################################################################### */
/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Constructor.
 *
 * @param argPart The chip's type (MAX17043/MAX17044). Has an influence on cell
 * voltage value.
 */
MAX1704X::MAX1704X(const MAX1704X::PartType &argPart)
    :   m_address( MAX1704X::DEFAULT_I2C_ADDRESS )
    ,   m_partType( argPart )
{

}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Returns the battery level at which the alert interrupt output will
 * be set and the alert flag will be raised.
 *
 * @return  An @c uint8_t value representing the cell's percentage of charge at
 *          which the alert will be raised. \n
 *          @em Range: [0;32].
 */
uint8_t MAX1704X::alertThreshold_pc()
{
    /* The value is stored as Two's complement, so we init the return value
     * with the mask to let it be at the maximum value. */
    uint8_t ret = 32U;

//    Serial.print( "mask is 0b" );
//    Serial.println( MAX1704X_CONFIG_MASK_ATHD, BIN );

    uint16_t reg_content = this->read_u16( MAX1704X_CONFIG_REGADDR );
//    Serial.print( "reg_content is 0b" );
//    Serial.println( reg_content, BIN );

    ret -= ( (reg_content) & MAX1704X_CONFIG_MASK_ATHD );
//    Serial.print( "threshold is 0b" );
//    Serial.println( ret, BIN );
//    Serial.print( "threshold is " );
//    Serial.println( ret );

    return ret;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To get the voltage of the cell.
 *
 * @return The cell's voltage, Volts value.
 */
float   MAX1704X::cellVoltage_V()
{
    /** The resolution of the chip's ADC, millivolts. */
    float   resolution_mV   = 0;

    /* Select the resolution depending on the part type */
    switch( this->m_partType )
    {
        case PartMAX17043:
            resolution_mV  = 1.25; /*< the MAX17043's ADC has a 1.25mV resolution */
            break;

        case PartMAX17044:
            resolution_mV  = 2.50; /*< the MAX17044's ADC has a 2.50mV resolution */
            break;

        default:
            break;
    }

    /* Calculate and return the cell voltage */
    return ( ( this->read_u16( MAX1704X_VCELL_H_REGADDR )
               >> 4 ) /*< The last 4 bits of the 16bits registers are unused */
             * resolution_mV )
            / 1000.0;   /*< conversion mV to Volts */
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Clears the alert flag.
 *
 * @see MAX1704::isAlerting()
 */
void    MAX1704X::clearAlert()
{
    uint16_t reg_content = this->read_u16( MAX1704X_CONFIG_REGADDR );

    M_BIT_CLEAR( reg_content, MAX1704X_CONFIG_MASK_ALRT );

    this->write_u16( MAX1704X_CONFIG_REGADDR, reg_content );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To know whether the ALERT flag is raised or not.
 *
 * The ALERT flag is set by the IC when the SOC register value falls below the
 * alert threshold setting and an interrupt is generated. This bit can only be
 * cleared by software. The power-up default value for ALRT is logic 0.
 *
 * @return  @c **true**    if state of charge is below the alert theshold.
 * @return  @c **false**   otherwise.
 *
 * @see MAX1704X::clearAlert()
 */
bool    MAX1704X::isAlerting()
{
    if( this->read_u8( MAX1704X_CONFIG_REGADDR ) & MAX1704X_CONFIG_MASK_ALRT )
        return true;
    else
        return false;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief MAX1704X::isSleeping To know whether the chip is in SLEEP mode or not.
 *
 * Holding both SDA and SCL logic-low forces the MAX17043/MAX17044 into Sleep
 * mode. While in Sleep mode, all IC operations are halted and power drain of
 * the IC is greatly reduced. After exiting Sleep mode, fuel-gauge operation
 * continues from the point it was halted. SDA and SCL must be held low for at
 * least 2.5s to guarantee transition into Sleep mode. Afterwards, a rising
 * edge on either SDA or SCL immediately transitions the IC out of Sleep mode.
 * \n Alternatively, Sleep mode can be entered by setting the SLEEP bit in the
 * CONFIG register to logic 1 through I2C communication. If the SLEEP bit is
 * set to logic 1, the only way to exit Sleep mode is to write SLEEP to logic 0
 * or power-on reset the IC.
 *
 * @return  @c **true**    if the chip is sleeping
 * @return  @c **false**   otherwise.
 */
bool    MAX1704X::isSleeping()
{
    if( this->read_u16( MAX1704X_CONFIG_REGADDR ) & MAX1704X_CONFIG_MASK_SLEEP )
        return true;
    else
        return false;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Convenience function to read a 16-bits unisgned data from the register
 * at \p argAddress of the device at address MAX1704X::m_address.
 *
 * @param argAddress    The @c uint8_t address of the register we want to read.
 * @return  An @c uint16_t containing the read byte.
 */
uint16_t    MAX1704X::read_u16(uint8_t argFirstRegAddress)
{
    uint16_t ret    = 0x0000;

    /* Write register's address */
    Wire.beginTransmission( this->m_address );
    Wire.write( argFirstRegAddress );
    Wire.endTransmission();

    /* Read two bytes */
    Wire.requestFrom( this->m_address, (uint8_t) 2U );

    ret =   Wire.read() << 8;
    ret +=  Wire.read();

    Wire.endTransmission();

    return ret;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Convenience function to read a 8-bits unisgned data from the register
 * at \p argAddress of the device at address MAX1704X::m_address.
 *
 * @param argAddress    The @c uint8_t address of the register we want to read.
 * @return  An @c uint8_t containing the read byte.
 */
uint8_t MAX1704X::read_u8(const uint8_t &argAddress)
{
    Wire.beginTransmission( this->m_address );
    Wire.write( argAddress );
    Wire.endTransmission();

    Wire.requestFrom( this->m_address, (uint8_t) 1U );
    uint8_t ret = Wire.read();
    Wire.endTransmission();

    return ret;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To define the battery level at which the alert interrupt output will
 * be set and the alert flag will be raised.
 *
 * @param argPercents Percentage of the cell's capacity at which the alert will
 *          be triggered.\n
 *          @em Range: [0;32].
 *
 * @return  @c **false**    If \p argPercents is out of range;
 * @return  @c **true**     otherwise.
 */
bool    MAX1704X::setAlertThreshold_pc(uint8_t argPercents)
{
    if( argPercents > 32U ) {
        return false;
    }


    /* Calculate two's complement for threshold new value */
    uint16_t threshold_tc   = 32U - argPercents;
//    Serial.print( String( "threshold_tc==" ) + threshold_tc );

    /* Read CONFIG register's content */
    uint16_t reg_content = this->read_u16( MAX1704X_CONFIG_REGADDR );
//    Serial.print( String( "reg_content==" )  );
//    Serial.println( reg_content, BIN );

    /* Remove previous threshold config from register content */
    M_BIT_CLEAR( reg_content, MAX1704X_CONFIG_MASK_ATHD );
//    Serial.print( String( "reg_content==" )  );
//    Serial.println( reg_content, BIN );

    /* Add the new threshold to the register's value */
    reg_content |= threshold_tc;
//    Serial.print( String( "reg_content==" )  );
//    Serial.println( reg_content, BIN );

    /* write register's new value */
    this->write_u16( MAX1704X_CONFIG_REGADDR, reg_content );

    return true;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To let the chip enter/exit sleep mode.
 *
 * @param argSleeping
 *          + @c **true**   The chip will go to sleep;
 *          + @c **false**  The chip will wake up.
 *
 * @see MAX1704X::isSleeping()
 */
void    MAX1704X::setSleeping(bool argSleeping)
{
    uint16_t reg_content = this->read_u16( MAX1704X_CONFIG_REGADDR );

//    Serial.print( "register address: " );
//    Serial.println( MAX1704X_CONFIGRCOMP_REGADDR, HEX );
//    Serial.print( "+-- reg_content == 0x" );
//    Serial.println( reg_content, HEX );

    if( argSleeping ) {
        M_BIT_SET( reg_content, MAX1704X_CONFIG_MASK_SLEEP );
    } else {
        M_BIT_CLEAR( reg_content, MAX1704X_CONFIG_MASK_SLEEP );
    }
//    Serial.print( "+-- reg_content to write == 0x" );
//    Serial.println( reg_content, HEX );


    this->write_u16( MAX1704X_CONFIG_REGADDR, reg_content );

//    reg_content = this->read_u16( MAX1704X_CONFIGRCOMP_REGADDR );
//    Serial.print( "+-- reg_content read == 0x" );
//    Serial.println( reg_content, HEX );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief To get the state of charge of the cell (percents).
 *
 * Calculated by the chip's ModelGauge algorithm.
 *
 * @return Percentage of the cell's full capacity.
 */
float   MAX1704X::stateOfCharge_pc()
{
    float decimals = this->read_u8( MAX1704X_SOC_REGADDR_DEC ) / 256.0;

    return this->read_u8( MAX1704X_SOC_REGADDR_INT ) + decimals;
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Triggers a "quick-start" operation.
 *
 * A quick-start allows the MAX17043/MAX17044 to restart fuel-gauge
 * calculations in the same manner as initial power-up of the IC. For example,
 * if an application’s power-up sequence is exceedingly noisy such that excess
 * error is introduced into the IC’s “first guess” of SOC, the host can issue a
 * quick-start to reduce the error. A quick-start is initiated by a rising edge
 * on the QSTRT pin, or through software by writing 4000h to the MODE register.
 */
void    MAX1704X::triggerQuickStart(void)
{
    this->write_u16( MAX1704X_MODE_H_REGADDR,
                     MAX1704X_MODE_CMD_QUICKSTART );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief completely resets the MAX1704x as if power had been removed.
 */
void    MAX1704X::triggerReset()
{
    this->write_u16( MAX1704X_COMMAND_H_REGADDR,
                     MAX1704X_COMMAND_CMD_POR );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Returns the chip's production version.
 *
 * The VERSION register is a value indicating the production version of the
 * MAX17043/MAX17044.
 *
 * @return The chip's production version.
 */
uint16_t MAX1704X::version()
{
    return this->read_u16( MAX1704X_VERSION_H_REGADDR );
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Convenience function to write a 8-bits
 * unisgned \p argData into the register \p argAddress of the device at address
 * MAX1704X::m_address.
 *
 * @param argAddress    The @c uint8_t register's address where we want to write.
 * @param argData  The @c uint8_t data we want to write to \p argAddress register.
 */
void    MAX1704X::write_u8(const uint8_t &argAddress, const uint8_t &argData)
{
    Wire.beginTransmission( this->m_address );
    Wire.write( argAddress );
    Wire.write( argData );
    Wire.endTransmission();
}

/* ######################################################################### */
/* ######################################################################### */
/**
 * @brief Convenience function to write a 16-bits
 * unisgned \p argData into the register \p argAddress of the device at address
 * MAX1704X::m_address.
 *
 * @param argAddress    The @c uint8_t register's address where we want to write.
 * @param argData  The @c uint16_t data we want to write to \p argAddress register.
 */
void    MAX1704X::write_u16(const uint8_t &argAddress, const uint16_t &argData)
{
    Wire.beginTransmission( this->m_address );
    Wire.write( argAddress );
    Wire.write( (uint8_t)(argData >> 8) );
    Wire.write( (uint8_t)(argData & 0x00FF) );
    Wire.endTransmission();
}

/* ######################################################################### */
/* ######################################################################### */
