#ifndef MPU6050_H
#define MPU6050_H

#include <Arduino.h>



class Mpu6050
{
public:
    Mpu6050();

    bool    init(String *argErrorString=0);

    uint8_t whoAmI(void) const;


    void    clearData(void);
    bool    updateData(void);


    int16_t rawAccelX(void) const;
    int16_t rawAccelY(void) const;
    int16_t rawAccelZ(void) const;

    int16_t rawGyroX(void) const;
    int16_t rawGyroY(void) const;
    int16_t rawGyroZ(void) const;

    int16_t rawTemperature(void) const;



    double  accelX_g(void) const;
    double  accelY_g(void) const;
    double  accelZ_g(void) const;

    double  gyroX_deg_s(void) const;
    double  gyroY_deg_s(void) const;
    double  gyroZ_deg_s(void) const;

    double  temperature_celsius(void) const;


    double  angleX_deg(void) const;
    double  angleY_deg(void) const;
    double  angleZ_deg(void) const;


private:


    enum    AccelerometerRange {
        AccelerometerRange_2g   =   0,
        AccelerometerRange_4g   =   1,
        AccelerometerRange_8g   =   2,
        AccelerometerRange_16g  =   3
    };

    enum    GyroRange {
        GyroRange250deg_s   = 0,
        GyroRange500deg_s   = 1,
        GyroRange1000deg_s  = 2,
        GyroRange2000deg_s  = 3
    };

    // Declaring an union for the registers and the axis values.
    // The byte order does not match the byte order of
    // the compiler and AVR chip.
    // The AVR chip (on the Arduino board) has the Low Byte
    // at the lower address.
    // But the MPU-6050 has a different order: High Byte at
    // lower address, so that has to be corrected.
    // The register part "reg" is only used internally,
    // and are swapped in code.
    typedef union accel_t_gyro_union
    {
      struct
      {
        uint8_t x_accel_h;
        uint8_t x_accel_l;
        uint8_t y_accel_h;
        uint8_t y_accel_l;
        uint8_t z_accel_h;
        uint8_t z_accel_l;
        uint8_t t_h;
        uint8_t t_l;
        uint8_t x_gyro_h;
        uint8_t x_gyro_l;
        uint8_t y_gyro_h;
        uint8_t y_gyro_l;
        uint8_t z_gyro_h;
        uint8_t z_gyro_l;
      } reg;
      struct
      {
        int16_t x_accel;
        int16_t y_accel;
        int16_t z_accel;
        int16_t temperature;
        int16_t x_gyro;
        int16_t y_gyro;
        int16_t z_gyro;
      } value;
    };



    AccelerometerRange  m_accelerometerRange;
    accel_t_gyro_union  m_data;
    double              m_forceVector;
    GyroRange           m_gyroRange;
    uint8_t             m_whoAmI;



    /**
     * @brief accelerometerCountsPerG   Returns the constant corresponding to
     * the number of "counts" there is in a G depending on the accelerometer's
     * range.
     *
     * @return
     */
    int accelerometerCountsPerG(void) const;

    /**
     * @brief gyroscopeCountsPerDeg_s   Returns the constant corresponding to
     * the number of "counts" there is in a deg/s depending on the gyroscope's
     * range.
     *
     * @return
     */
    float gyroscopeCountsPerDeg_s(void) const;


    /**
     * @brief read is a common function to read multiple bytes from an I2C device.
     * @param start
     * @param buffer
     * @param size
     * @return
     *
     * It uses the boolean parameter for Wire.endTransMission() to be able to
     * hold or release the I2C-bus.
     * This is implemented in Arduino 1.0.1.
     * Only this function is used to read.
     * There is no function for a single byte.
     */
    int read(int start, uint8_t *buffer, int size);


    // --------------------------------------------------------
    // MPU6050_write
    //
    // This is a common function to write multiple bytes to an I2C device.
    //
    // If only a single register is written,
    // use the function MPU_6050_write_reg().
    //
    // Parameters:
    //   start : Start address, use a define for the register
    //   pData : A pointer to the data to write.
    //   size  : The number of bytes to write.
    //
    // If only a single register is written, a pointer
    // to the data has to be used, and the size is
    // a single byte:
    //   int data = 0;        // the data to write
    //   MPU6050_write (MPU6050_PWR_MGMT_1, &c, 1);
    //
    int write(int start, const uint8_t *pData, int size);


    // --------------------------------------------------------
    // MPU6050_write_reg
    //
    // An extra function to write a single register.
    // It is just a wrapper around the MPU_6050_write()
    // function, and it is only a convenient function
    // to make it easier to write a single register.
    //
    int write_reg(int reg, uint8_t data);

};

#endif // MPU6050_H
