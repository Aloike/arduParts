/**
 *  @file   PCA9685.h
 *  @brief  Header file for PCA9685 class.
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
#ifndef PCA9685_H
#define PCA9685_H

#include <Arduino.h>


/**
 * @class   PCA9685
 * @brief The PCA9685 class provides an interface for Arduino devices to
 * access NXP's PCA9685 16-channel, 12-bit PWM Fm+ I2C-bus LED controller.
 * @author      GOM
 * @copyright   Copyright © 2014 GOM - X11 License
 */
class PCA9685
{

public:

    /*
     *  Default values
     */
    static const uint8_t    DEFAULT_ADDRESS             = 0x40;     /*!< @brief The default address of the device when all bits [A0;A5] are set to logic 0. */
    static const float      DEFAULT_CLOCKFREQUENCY_HZ   = 25000000; /*!< @brief The default clock frequency for internal counter. PCA9685's internal oscillator runs at 25MHz. */
    static const uint8_t    OUTPUT_COUNT                = 16;       /*!< @brief The number of outputs the PCA9685 have. */


    /*
     *  Enumerations
     */
    /**
     * @brief The OutputChangeEvent enum represents the different events on
     * which the output will be changed.
     *
     *  Default: ChangeOnStop
     */
    enum OutputChangeEvent {

        ChangeOnStop    = 0,    /*!< Outputs change on STOP command. Allows
        * synchronizing outputs of more than one PCA9685. Applicable to
        * registers from 06h (LED0_ON_L) to 45h (LED15_OFF_H) only. One or more
        * registers can be written, in any order, before STOP. */

        ChangeOnAcknowledge     /*!< Update on ACK requires all 4 PWM channel
        * registers to be loaded before outputs will change on the last ACK. */
    };


    /**
     * @brief The OutputPolicyWhenDisabled enum represents the diffenrent ways
     * to manage outputs behaviour when the /OE pin is set to logical 1 (output
     * disabled).
     */
    enum OutputPolicyWhenDisabled {

        OutputFalse         = 0,    /*!< When /OE pin=1, outputs will be set
        * to logical 0. */

        OutputTrue          = 1,    /*!< When /OE pin=1 (output drivers not
        * enabled), outputs will be set to logical 1. */

        OutputHighImpedance = 2     /*!< When /OE pin=1 (output drivers not
        * enabled), outputs will be set to high impedance. */
    };


    /**
     * @brief The OutputStructure enum represents the different ways to manage
     * output's internal ("electronic") structure.
     *
     * Normal LEDs can be driven directly in either mode. Some newer LEDs
     * include integrated Zener diodes to limit voltage transients, reduce EMI,
     * protect the LEDs and these must be driven only in the open-drain mode to
     * prevent overheating the IC.
     */
    enum OutputStructure {

        OutputStructureOpenDrain    = 0,/*!< The 16 LEDn outputs are configured
        * with an open-drain structure. */

        OutputStructureTotemPole    = 1 /*!< The 16 LEDn outputs are configured
        * with a totem pole structure. Allows minimizing the amount of external
        * components required to control the external driver (N-type or P-type
        * device). */
    };


    /* ##################################################################### */
    /* ##################################################################### */

    /*
     *  Constructor
     */

    PCA9685( uint8_t    argAddress          = DEFAULT_ADDRESS,
             double     argClockFrequency   = DEFAULT_CLOCKFREQUENCY_HZ );





    /*
     *  Initialization methods
     */
    void    init(void);
    void    reset(void);


    /*
     *  Power management
     */
    bool    isSleeping(void);
    void    setSleeping(bool argIsSleeping);
    bool    canRestart(void);
    bool    restart(void);


    /*
     *  Clock management
     */
    double  clockFrequency(void) const; /*!< @brief Returns the value that has been set by the constructor. */

    bool    isUsingExternalClock(void);
    void    setUseExternalClock(void);


    /*
     *  I2C addresses management
     */
    uint8_t address(void) const;    /*!< @brief Returns the value that has been set by the constructor. */

    bool    subAddress1Enabled(void);
    void    setSubAddress1Enabled( bool argEnabled );

    bool    subAddress2Enabled(void);
    void    setSubAddress2Enabled( bool argEnabled );

    bool    subAddress3Enabled(void);
    void    setSubAddress3Enabled( bool argEnabled );

    bool    ledAllCallAddressEnabled(void);
    void    setLedAllCallAddressEnabled(bool argEnabled);


    /*
     *  Output management
     */
    double  outputFrequency(void);
    bool    setOutputFrequency(double argFrequency_Hz );

    uint8_t outputPrescale(void);
    void    setOutputPrescale(uint8_t argPrescale);

    bool    outputStateInverted(void);
    void    setOutputStateInverted(bool argInverted);

    OutputChangeEvent   outputChangeEvent(void);
    void                setOutputChangeEvent( OutputChangeEvent argEvent );

    OutputStructure     outputStructure(void);
    void                setOutputStructure( OutputStructure argStructure);

    OutputPolicyWhenDisabled    disabledOutputPolicy(void);
    void                        setDisabledOutputPolicy(OutputPolicyWhenDisabled argPolicy);


    int8_t  setOutput( uint8_t argOutputNbr, uint16_t argOn, uint16_t argOff );

    bool    setOutputForceFullOn(uint8_t argOutputNbr, bool argForceFullOn );
    bool    setOutputForceFullOff(uint8_t argOutputNbr, bool argForceFullOff );

#if 0
    void    setAllOn(void);
    void    setAllOff(void);
#endif



protected:

    const uint8_t   m_address;          /*!< @brief The PCA9685's I2C slave address, as set by the constructor. */
    const double    m_clockFrequency;   /*!< @brief The clock frequency, to calculate prescales; set by the constructor. */

    uint8_t read_8u( const uint8_t& argAddress );
    void    write_8u(const uint8_t& argAddress, const uint8_t& argData);


    bool    isAutoIncrementingRegister(void);
    void    setAutoIncrementRegister( bool argEnableAutoIncrement );



};

#endif  // PCA9685_H
