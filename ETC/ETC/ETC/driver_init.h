/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_H_INCLUDED
#define DRIVER_INIT_H_INCLUDED

#define F_CPU 8000000

#include <compiler.h>
#include <port.h>
#include <atmel_start_pins.h>

#include <sysctrl.h>

#ifdef __cplusplus
		extern "C" {
#endif

	/**
	 * \brief System initialization
	 */
	void system_init(void);

#ifdef __cplusplus
		}
#endif

#endif /* DRIVER_INIT_H_INCLUDED */

