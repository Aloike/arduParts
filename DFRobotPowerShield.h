#ifndef DFROBOTPOWERSHIELD_H
#define DFROBOTPOWERSHIELD_H


/*!
 * @brief Interface for DFRobot's Power Shield (SKU:DFR0105)
 *
 * @see http://www.dfrobot.com/wiki/index.php/Power_Shield_(SKU:DFR0105) for
 * hardware details.
 *
 * By default, the auxiliary ower output is disabled (see implementation of
 * DFRobotPowerShield::init() ).
 *
 * @author     GOM
 * @copyright  © 2014 GOM
 *
 * @date       <b>2014-02-13   Thu.    15:25:16</b>    Creation.
 */
class   DFRobotPowerShield
{
public:
    DFRobotPowerShield();

    bool    outputEnabled(void) const;
    void    setOutputEnabled( const bool& argEnabled );

    float   voltage(void) const;
    void    setVoltageRange( float argMin, float argMax );


private:

    float   m_voltageMin;   /*!< The lower value that is measured (when AN0 value is 0). */
    float   m_voltageMax;   /*!< The higher end of the voltage range that is measured by AN0. */

    void    init(void);
};

#endif  /*< DFROBOTPOWERSHIELD_H */
