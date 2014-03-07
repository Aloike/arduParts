/**
 *  @file   MAX1704X.h
 *  @brief  Header file for MAX1704X class.
 *  @author GOM
 *
 * @copyright    Code: Copyright © 2014 GOM - X11 License\n
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
 * @copyright   Datasheet abstracts: Copyright © 2010 Maxim Integrated Products
 *
 *  @date   2014-03-06  Thu.    18-30-00    Creation.
 */
#ifndef MAX1704X_H
#define MAX1704X_H

#include <Arduino.h>


/**
 * @brief The MAX1704X class provides an interface for Arduino devices to
 * access MAXIM's MAX17043/MAX17044 fuel-gauge systems for lithium-ion (Li+)
 * batteries.
 *
 * \par Abstract from the datasheet:
 * The MAX17043 is configured to operate with a single lithium cell and the
 * MAX17044 is configured for a dual-cell 2S pack.\n
 * The MAX17043/MAX17044 use a sophisticated Li+ battery-modeling scheme,
 * called ModelGauge™ to track the battery’s relative state-of-charge (SOC)
 * continuously over a widely varying charge/discharge profile. Unlike
 * traditional fuel gauges, the ModelGauge algorithm eliminates the need for
 * battery relearn cycles and an external current-sense resistor. Temperature
 * compensation is possible in the application with minimal interaction between
 * a μC and the device.
 *
 * @author GOM
 * @copyright   Code: Copyright © 2014 GOM - X11 License
 * @copyright   Datasheet: Copyright © 2010 Maxim Integrated Products
 */
class MAX1704X
{

public:

    /**
     * @brief The PartType enum represents the chips supported by this driver.
     */
    enum PartType {
        PartMAX17043    = 0,    /*!< The ship we use is a MAX17043 */
        PartMAX17044    = 1     /*!< The ship we use is a MAX17044 */
    };

    /*
     *  Default values
     */
    static const uint8_t    DEFAULT_I2C_ADDRESS = 0x36; /*!< Slave address is 0110110b (7bits)(0x6C to write, 0x6D to read). */


    /*
     *  Constructor
     */

    MAX1704X( const MAX1704X::PartType& argPart );


    /*
     *  Get cell data
     */
    float       cellVoltage_V(void);
    float       stateOfCharge_pc(void);


    /*
     * MAX1704x management
     */
    void    triggerQuickStart(void);
    void    triggerReset(void);

    uint8_t     alertThreshold_pc(void);
    bool        setAlertThreshold_pc( uint8_t argPercents );

    bool        isAlerting(void);
    void        clearAlert(void);

    bool        isSleeping(void);
    void        setSleeping( bool argSleeping );

    uint16_t    version(void);


    
private:

    const uint8_t           m_address;
    const PartType      m_partType;


    uint16_t    read_u16( uint8_t argFirstRegAddress );
    uint8_t     read_u8(const uint8_t &argAddress);

    void    write_u8(const uint8_t &argAddress, const uint8_t &argData);
    void    write_u16(const uint8_t &argAddress, const uint16_t &argData);

};

#endif  // MAX1704X_H
