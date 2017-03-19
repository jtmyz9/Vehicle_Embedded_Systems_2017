/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Header file for pid.c.
 *
 * - File:               pid.h
 * - Compiler:           IAR EWAAVR 4.11A
 * - Supported devices:  All AVR devices can be used.
 * - AppNote:            AVR221 - Discrete PID controller
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support email: avr@atmel.com
 *
 * $Name: RELEASE_1_0 $
 * $Revision: 1.1 $
 * $RCSfile: pid.h,v $
 * $Date: 2006/02/16 11:46:13 $
 *****************************************************************************/

#ifndef ETC_H
#define ETC_H

#include "stdint.h"
#include <avr/io.h>


//atmel libraries for AT90CAN128
#include "Includes/adc_drv.h"		//adc functions and macros
#include "Includes/can_lib.h"		//canbus functions
#include "Includes/timer16_drv.h"	//PWM inline functions
#include "Includes/timer8_drv.h"
#include "Includes/uart_lib.h"
#include "Includes/i2cmaster.h"

#define SCALING_FACTOR  128

/*! \brief PID Status
 *
 * Setpoints and data used by the PID control algorithm
 */
typedef struct PID_DATA{
  //! Last process value, used to find derivative of process value.
  int16_t lastProcessValue;
  //! Summation of errors, used for integrate calculations
  int32_t sumError;
  //! The Proportional tuning constant, multiplied with SCALING_FACTOR
  int16_t P_Factor;
  //! The Integral tuning constant, multiplied with SCALING_FACTOR
  int16_t I_Factor;
  //! The Derivative tuning constant, multiplied with SCALING_FACTOR
  int16_t D_Factor;
  //! Maximum allowed error, avoid overflow
  int16_t maxError;
  //! Maximum allowed sumerror, avoid overflow
  int32_t maxSumError;
} pidData_t;

/*! \brief Maximum values
 *
 * Needed to avoid sign/overflow problems
 */
// Maximum value of variables
#define MAX_INT         INT16_MAX
#define MAX_LONG        INT32_MAX
#define MAX_I_TERM      (MAX_LONG / 2)
	

/*! \brief P, I and D parameter values
 *
 * The K_P, K_I and K_D values (P, I and D gains)
 */
#define K_P     2.80
#define K_I     0.00
#define K_D     0.00
	
/*
* Constants for PID operation
*/

#define STICTION_CONSTAST	0		//constant to account for stiction in system, prevent integral windup
#define TIME_INTERVAL   128			//interval to calculate PID value
#define WINDUP_MAX					//MAX PID value, currently not used

void pid_Init(int16_t p_factor, int16_t i_factor, int16_t d_factor, struct PID_DATA *pid);
int16_t pid_Controller(int16_t setPoint, int16_t processValue, struct PID_DATA *pid_st);
void pid_Reset_Integrator(pidData_t *pid_st);
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

#endif
