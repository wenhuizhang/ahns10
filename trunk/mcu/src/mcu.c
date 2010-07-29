/**
 * \file   mcu.c
 * \author Tim Molloy
 *
 * $Author$
 * $Date$
 * $Rev$
 * $Id$
 *
 * Queensland University of Technology
 *
 * \section DESCRIPTION
 * Main file for the ATMEGA328P Mode Control Unit
 */

#include "avrdefines.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "pwm.h"
#include "pulsecapture.h"
#include "usart.h"
#include "modeindicator.h"
#include "mode.h"

/**
 * @brief Pack of all Initialise Routines
 */
void init();

/**
 * @brief Combine High Level Commands
 *
 * Depending on the selected flight mode:
 * 
 * - Pass the RC Commands without modification
 * - Augment the RC commands with AP commands
 * - Pass the AP Commands without modification 
 */
void CombineCommands();

void main (void)
{
  StopPWM();
  
  init();
  sei();
  _delay_ms(250);

  #ifdef DEBUG
  stdout = &debugOut;
  printf("System Initialised\n");
  #endif

  // Initialise and detect the Inputs
  uint8_t inputsInitialised = 0;
  do 
  {
    if (inputChannel[inputsInitialised].measuredPulseWidth < (PC_PWM_MIN + PULSE_TOLERANCE) )
    {
      //printf("%u\n",inputChannel[inputsInitialised].measuredPulseWidth);
      inputsInitialised++;
    }
    else
    {
      ToggleRed(TOGGLE);
      _delay_ms(250);
    }
  } while (inputsInitialised < NUM_CHANNELS);

  // Minimum Output
  ESC1_COUNTER = esc1Min;
  ESC2_COUNTER = esc2Min;
  ESC3_COUNTER = esc3Min;
  ESC4_COUNTER = esc4Min;
  StartPWM();

  for ( ; ; )
  {
    // RC Pulse Capture
    if(newRC) // new RC values to process
    {
      newRC = 0;
      UpdateRC();
      if(failSafe) // Never Recover from Failsafe 
      {
        static uint8_t toggleEnabled = 0;
        
        // Minimum Commands
        ESC1_COUNTER = esc1Min;
        ESC2_COUNTER = esc2Min;
        ESC3_COUNTER = esc3Min;
        ESC4_COUNTER = esc4Min;
  
        // Toggle Yellow Error at 10Hz
        if (!toggleEnabled)
        {
          toggleEnabled = 1;
          ToggleRed(OFF);
          ToggleGreen(OFF);
	}
        else
        {
	  _delay_ms(100);
          ToggleGreen(TOGGLE);
          ToggleRed(TOGGLE);
        }
        
        /** TODO: Tell Flight Computer */
        
      }
      else
      {	      
        // Combine RC and AP inputs
        // Then Output them
        newAPCommands = 0;
        CombineCommands();
      }
    }
    else if (newAPCommands) // only new autopilot commands
    {
      newAPCommands = 0;
      CombineCommands();
    }
  }
  return;
}


inline void init()
{
  // Setup Clock - prescalar of 2 to 8MHz
  CLKPR = (1<<CLKPCE);
  CLKPR = (1<<CLKPS0);
  
  InitialiseModeIndicator();
  InitialiseTimer0();
  InitialiseTimer1();

  InitialiseUSART();

  InitialiseTimer2();
  InitialisePC();

  return;
}

inline void CombineCommands()
{
  // Flight Mode Choice limited to RC
  flightMode = rcMode;
  
  switch (flightMode)
  {
    case MANUAL_DEBUG:
      IndicateManual();
      MixCommands(&rcThrottle, &rcRoll, &rcPitch, &rcYaw);
      break;
    case AUGMENTED: // Combine AP and RC Commands
      IndicateAugmented();
      /** TODO IMPELEMENT THE AUGMENTATION */
      break;
    case AUTOPILOT: // Pass AP unaltered
      IndicateAutopilot();
      MixCommands(&apThrottle, &apRoll, &apPitch, &apYaw);
  }
  return;
}
