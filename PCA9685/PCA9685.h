#ifndef PCA9685_H
#define PCA9685_H

#include <Arduino.h>



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
    enum OutputChangeEvent {
        ChangeOnStop    = 0,    /*!< Outputs change on STOP command. Allows
                                    synchronizing outputs of more than one
                                    PCA9685. Applicable to registers from
                                    06h (LED0_ON_L) to 45h (LED15_OFF_H) only.
                                    One or more registers can be written, in
                                    any order, before STOP. */

        ChangeOnAcknowledge     /*!< Update on ACK requires all 4 PWM channel
                                    registers to be loaded before outputs will
                                    change on the last ACK. */

    };

    enum OutputPolicyWhenDisabled {
        OutputFalse         = 0,    /*!< When /OE pin=1 (output drivers not enabled), outputs will be set to 0. */
        OutputTrue          = 1,    /*!< When /OE pin=1 (output drivers not enabled), outputs will be set to 1. */
        OutputHighImpedance = 2     /*!< When /OE pin=1 (output drivers not enabled), outputs will be set to high impedance. */
    };

    /**
     * @brief The OutputStructure enum represents the different ways to manage
     * output's internal structure.
     *
     * Normal LEDs can be driven directly in either
     * mode. Some newer LEDs include integrated Zener diodes to limit voltage
     * transients, reduce EMI, protect the LEDs and these must be
     * driven only in the open-drain mode to prevent overheating the IC.
     */
    enum OutputStructure {
        OutputStructureOpenDrain    = 0,    /*!< The 16 LEDn outputs are configured with an open-drain structure. */
        OutputStructureTotemPole    = 1     /*!< The 16 LEDn outputs are configured with a totem pole structure. */
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
    float   outputFrequency(void);
    bool    setOutputFrequency(float argFrequency_Hz );

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


    bool    setOutput( uint8_t argOutputNbr, uint16_t argOn, uint16_t argOff );

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
