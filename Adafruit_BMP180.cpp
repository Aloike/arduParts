/*!
 *  @file   Adafruit_BMP180.cpp
 *  @brief  Defininitions of methods declared in file Adafruit_BMP180.h
 *
 *  @author MGO
 */

#include "Adafruit_BMP180.h"

#include <Wire.h>


static const uint8_t    AdafruitBmp180_Address  = 0x77;
static const uint8_t    BMP180_CHIP_ID          = 0x55;


enum Bmp180Registers
{
  BMP180_REGISTER_CAL_AC1            = 0xAA,  // R   Calibration data (16 bits)
  BMP180_REGISTER_CAL_AC2            = 0xAC,  // R   Calibration data (16 bits)
  BMP180_REGISTER_CAL_AC3            = 0xAE,  // R   Calibration data (16 bits)
  BMP180_REGISTER_CAL_AC4            = 0xB0,  // R   Calibration data (16 bits)
  BMP180_REGISTER_CAL_AC5            = 0xB2,  // R   Calibration data (16 bits)
  BMP180_REGISTER_CAL_AC6            = 0xB4,  // R   Calibration data (16 bits)
  BMP180_REGISTER_CAL_B1             = 0xB6,  // R   Calibration data (16 bits)
  BMP180_REGISTER_CAL_B2             = 0xB8,  // R   Calibration data (16 bits)
  BMP180_REGISTER_CAL_MB             = 0xBA,  // R   Calibration data (16 bits)
  BMP180_REGISTER_CAL_MC             = 0xBC,  // R   Calibration data (16 bits)
  BMP180_REGISTER_CAL_MD             = 0xBE,  // R   Calibration data (16 bits)
  BMP180_REGISTER_CHIPID             = 0xD0,
  BMP180_REGISTER_VERSION            = 0xD1,
  BMP180_REGISTER_SOFTRESET          = 0xE0,
  BMP180_REGISTER_CONTROL            = 0xF4,
  BMP180_REGISTER_TEMPDATA           = 0xF6,
  BMP180_REGISTER_PRESSUREDATA       = 0xF6,
  BMP180_REGISTER_READTEMPCMD        = 0x2E,
  BMP180_REGISTER_READPRESSURECMD    = 0x34
};


/* ######################################################################### */
/* ######################################################################### */

Adafruit_BMP180::Adafruit_BMP180(/*int32_t argSensorId)
    :   m_sensorId( argSensorId */)
    :   m_mode( ModeUltraHighResolution )
{

}

/* ######################################################################### */
/* ######################################################################### */

float    Adafruit_BMP180::getPressure_Pa()
{
    int32_t  rawTemperature = 0, rawPressure = 0, compp = 0;
    int32_t  x1, x2, b5, b6, x3, b3, p;
    uint32_t b4, b7;


    /* Get the raw pressure and temperature values */
    rawTemperature  = readRawTemperature();
    rawPressure     = readRawPressure();


    /* Temperature compensation */
    x1 = (rawTemperature - (int32_t)(this->m_calibrationData.ac6))*((int32_t)(this->m_calibrationData.ac5))/pow(2,15);
    x2 = ((int32_t)(this->m_calibrationData.mc*pow(2,11)))/(x1+(int32_t)(this->m_calibrationData.md));
    b5 = x1 + x2;


    /* Pressure compensation */
    b6 = b5 - 4000;
    x1 = (this->m_calibrationData.b2 * ((b6 * b6) >> 12)) >> 11;
    x2 = (this->m_calibrationData.ac2 * b6) >> 11;
    x3 = x1 + x2;
    b3 = (((((int32_t) this->m_calibrationData.ac1) * 4 + x3) << this->m_mode ) + 2) >> 2;
    x1 = (this->m_calibrationData.ac3 * b6) >> 13;
    x2 = (this->m_calibrationData.b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = (this->m_calibrationData.ac4 * (uint32_t) (x3 + 32768)) >> 15;
    b7 = ((uint32_t) (rawPressure - b3) * (50000 >> this->m_mode));

    if (b7 < 0x80000000)
    {
        p = (b7 << 1) / b4;
    }
    else
    {
        p = (b7 / b4) << 1;
    }

    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    compp = p + ((x1 + x2 + 3791) >> 4);

    /* Return compensated pressure value */
    return compp;
}

/* ######################################################################### */
/* ######################################################################### */

float    Adafruit_BMP180::getTemperature_c()
{
    int32_t UT, X1, X2, B5;
    float ret;

    UT  = this->readRawTemperature();


    X1 = (UT - (int32_t)this->m_calibrationData.ac6)
            * ((int32_t)this->m_calibrationData.ac5) / pow(2,15);

    X2 = ((int32_t)this->m_calibrationData.mc * pow(2,11))
            / (X1+(int32_t)this->m_calibrationData.md);

    B5 = X1 + X2;

    ret = ( B5 + 8 ) / pow( 2, 4 );
    ret /= 10;

    return ret;
}

/* ######################################################################### */
/* ######################################################################### */

Adafruit_BMP180::ErrorCode   Adafruit_BMP180::init(Bmp180Mode argMode)
{
    /* Init the I2C */
    Wire.begin();


    /* Check the chip ID */
    uint8_t id;
    this->read_uint8( BMP180_REGISTER_CHIPID, &id );
    if( id != BMP180_CHIP_ID ) {
        return ErrorBadChipId;
    }


    /* store the selected mode */
    this->m_mode    = argMode;


    /* read the calibration data */
    this->readCalibrationData();


    return NoError;
}

/* ######################################################################### */
/* ######################################################################### */

void    Adafruit_BMP180::readCalibrationData()
{
    this->read_int16(BMP180_REGISTER_CAL_AC1, &m_calibrationData.ac1);
    this->read_int16(BMP180_REGISTER_CAL_AC2, &m_calibrationData.ac2);
    this->read_int16(BMP180_REGISTER_CAL_AC3, &m_calibrationData.ac3);
    this->read_uint16(BMP180_REGISTER_CAL_AC4, &m_calibrationData.ac4);
    this->read_uint16(BMP180_REGISTER_CAL_AC5, &m_calibrationData.ac5);
    this->read_uint16(BMP180_REGISTER_CAL_AC6, &m_calibrationData.ac6);
    this->read_int16(BMP180_REGISTER_CAL_B1, &m_calibrationData.b1);
    this->read_int16(BMP180_REGISTER_CAL_B2, &m_calibrationData.b2);
    this->read_int16(BMP180_REGISTER_CAL_MB, &m_calibrationData.mb);
    this->read_int16(BMP180_REGISTER_CAL_MC, &m_calibrationData.mc);
    this->read_int16(BMP180_REGISTER_CAL_MD, &m_calibrationData.md);
}

/* ######################################################################### */
/* ######################################################################### */

int32_t     Adafruit_BMP180::readRawPressure()
{
    uint8_t  p8;
    uint16_t p16;
    int32_t  p32;

    this->writeCommand(BMP180_REGISTER_CONTROL,
                       BMP180_REGISTER_READPRESSURECMD
                       + (this->m_mode << 6) );

    switch( this->m_mode )
    {
        case ModeUltraLowPower:
            delay(5);
            break;

        case ModeStandard:
            delay(8);
            break;

        case ModeHighResolution:
            delay(14);
            break;

        case ModeUltraHighResolution:
        default:
            delay(26);
            break;
    }

    this->read_uint16( BMP180_REGISTER_PRESSUREDATA, &p16 );

    p32 = (uint32_t)p16 << 8;
    this->read_uint8( BMP180_REGISTER_PRESSUREDATA+2, &p8);
    p32 += p8;
    p32 >>= (8 - this->m_mode);

    return p32;
}

/* ######################################################################### */
/* ######################################################################### */

int32_t     Adafruit_BMP180::readRawTemperature()
{
    int32_t ret = 0;

    this->writeCommand( BMP180_REGISTER_CONTROL, BMP180_REGISTER_READTEMPCMD );

    delay(5);

    uint16_t    val;
    this->read_uint16( BMP180_REGISTER_TEMPDATA, &val );
    ret = val;

    return ret;
}

/* ######################################################################### */
/* ######################################################################### */

void    Adafruit_BMP180::read_int16(byte argRegister, int16_t *argValue)
{
    uint16_t i;

    Adafruit_BMP180::read_uint16( argRegister, &i );

    *argValue = (int16_t)i;
}

/* ######################################################################### */
/* ######################################################################### */

void    Adafruit_BMP180::read_uint8(byte argRegister, uint8_t *argValue)
{
    Wire.beginTransmission( AdafruitBmp180_Address );
//#if ARDUINO >= 100
    Wire.write( (uint8_t)argRegister );
//#else
//    Wire.send(argRegister);
//#endif
    Wire.endTransmission();

    Wire.requestFrom( (uint8_t)AdafruitBmp180_Address, (byte)1 );
//#if ARDUINO >= 100
    *argValue = Wire.read();
//#else
//    *argValue = Wire.receive();
//#endif
    Wire.endTransmission();
}

/* ######################################################################### */
/* ######################################################################### */

void    Adafruit_BMP180::read_uint16(byte argRegister, uint16_t *argValue)
{
    Wire.beginTransmission((uint8_t)AdafruitBmp180_Address);
//#if ARDUINO >= 100
    Wire.write((uint8_t)argRegister);
//#else
//    Wire.send(reg);
//#endif
    Wire.endTransmission();

    Wire.requestFrom( (uint8_t)AdafruitBmp180_Address, (byte)2 );
//#if ARDUINO >= 100
    *argValue = (Wire.read() << 8) | Wire.read();
//#else
//    *value = (Wire.receive() << 8) | Wire.receive();
//#endif
    Wire.endTransmission();
}

/* ######################################################################### */
/* ######################################################################### */

void    Adafruit_BMP180::writeCommand(byte argRegister, byte argValue)
{
    Wire.beginTransmission((uint8_t)AdafruitBmp180_Address);

    Wire.write((uint8_t)argRegister);
    Wire.write((uint8_t)argValue);

    Wire.endTransmission();
}

/* ######################################################################### */
/* ######################################################################### */
