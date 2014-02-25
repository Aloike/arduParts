#ifndef ADAFRUIT_BMP180_H
#define ADAFRUIT_BMP180_H

#include <Arduino.h>

/*!
 * @brief Interface for Adafruit's BMP180 sensor board.
 *
 * This class and its implementation is mostly based on Kevin Townsend's work
 * for Adafruit Industries (Adafruit_BMP085_Unified class).
 *
 * @see http://www.adafruit.com/products/1603#Technical_Details
 * @see http://learn.adafruit.com/bmp085/wiring-the-bmp085
 *
 * @author     GOM
 * @copyright  © 2014 GOM
 *
 * @date       <b>2014-02-20   Thu.    15:03:00</b>    Creation.
 */
class Adafruit_BMP180
{

public:

    enum ErrorCode {
        ErrorUnknown    = -1,
        NoError         = 0,
        ErrorBadChipId
    };

    enum Bmp180Mode {
        ModeUltraLowPower       = 0,    /*!< @todo test this */
        ModeStandard            = 1,    /*!< @todo test this */
        ModeHighResolution      = 2,    /*!< @todo test this */
        ModeUltraHighResolution = 3
    };


    Adafruit_BMP180();


    ErrorCode init( Bmp180Mode argMode = ModeUltraHighResolution );


    float   getAltitude_m( float argSeaLevelPressure_Pa=101525 );
    float   getPressure_Pa(void);
    float   getTemperature_c(void);


    int32_t readRawPressure(void);
    int32_t readRawTemperature(void);



private:

    struct  CalibrationData
    {
      int16_t  ac1;
      int16_t  ac2;
      int16_t  ac3;
      uint16_t ac4;
      uint16_t ac5;
      uint16_t ac6;
      int16_t  b1;
      int16_t  b2;
      int16_t  mb;
      int16_t  mc;
      int16_t  md;
    };


    CalibrationData m_calibrationData;
    Bmp180Mode      m_mode;


    static void read_int16(byte argRegister, int16_t *argValue);

    static void read_uint8(byte argRegister, uint8_t *argValue);
    static void read_uint16(byte argRegister, uint16_t *argValue);

    void    readCalibrationData(void);

    static void writeCommand( byte argRegister, byte argValue );

};


#endif  //  ADAFRUIT_BMP180_H
