/**
 * @file   MCUCommands.h
 * @author Tim Molloy
 *
 * $Author: tlmolloy@gmail.com $
 * $Date: 2010-08-06 20:12:55 +1000 (Fri, 06 Aug 2010) $
 * $Rev: 282 $
 * $Id: mcuserial.c 282 2010-08-06 10:12:55Z tlmolloy@gmail.com $
 *
 * Queensland University of Technology
 *
 * @section DESCRIPTION
 * MCU Commands Header File
 * For use in the MCU and the FC software to enable shared Serial communication
 * - Defines Framing Characters
 * - MCU Commands
 */

#ifndef MCU_COMMANDS_H
#define MCU_COMMANDS_H

#define FRAME_CHAR '#'

enum {
	GET_MCU_COMMANDS = 0,
	GET_MCU_PERIODIC = 1,
	SEND_MCU_COMMANDS = 2
};

enum FlightModes {
       MANUAL_DEBUG,  /**< Manual Mode with no AP mixing */
       AUGMENTED,     /**< Manual Mode with additional AP mixing */
       AUTOPILOT,     /**< AP Mode only*/
       FAIL_SAFE      /**< MCU Failsafe Engaged*/ 
};

/** CPU Clock Speed in Hz */
#define F_CPU 8000000UL // 16MHz scaled to 8 MHz

/** @name PWM Defines */
#define PWM_PRESCALAR 64.0
#define F_PWM ((double) F_CPU/(PWM_PRESCALAR*510.0)) /**< Pre-scalar of 64 */
#define PWM_DT_US ((double) (1e6 * PWM_PRESCALAR / F_CPU)) /**< Time Resolution of generated PWM signals */

/**
 * @brief Convert micro-seconds of high to timer 8 bit range
 */
#define PWMToCounter(pulseHighTime) ((pulseHighTime) / (2.0 * PWM_DT_US));

/**
 * @brief Convert timer 8 bit range to micro-seconds
 */
#define CounterToPWM(n) (2.0 * PWM_DT_US * (n));




#endif // MCU_COMMANDS_H