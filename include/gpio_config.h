#ifndef GPIO_CONFIG_H_
#define GPIO_CONFIG_H_

#include <rtems.h>
#include <stdio.h>
#include <stdlib.h>
#include <bsp.h>

#define CONFIGURE_INIT

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXECUTIVE_RAM_SIZE				(100*1024*1024)

#define CONFIGURE_MAXIMUM_TASKS						1

#define CONFIGURE_MAXIMUM_DRIVERS 					10

#define CONFIGURE_MICROSECONDS_PER_TICK				1000

#define CONFIGURE_INIT_TASK_STACK_SIZE				(10*1024*1024)

#define CONFIGURE_INIT_TASK_PRIORITY				12

#define CONFIGURE_INIT_TASK_INITIAL_MODES 			(RTEMS_NO_PREEMPT | \
                                           	   	   	RTEMS_NO_TIMESLICE | \
                                           	   	   	RTEMS_NO_ASR | \
                                           	   	   	RTEMS_INTERRUPT_LEVEL(0))

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS	40

#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM

#define	CONFIGURE_STACK_CHECKER_ON

#define RTEMS_PCI_CONFIG_LIB

#define CONFIGURE_PCI_LIB 							1


#include <rtems/confdefs.h>

/* Configure Driver manager */
/* if --drvmgr was given to configure  */
#if defined(RTEMS_DRVMGR_STARTUP) && defined(LEON3)
	#ifdef CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
		#define CONFIGURE_DRIVER_AMBAPP_GAISLER_GPTIMER		// Timer driver
	#endif
	/* Add UART Driver for this example */
	#ifdef CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
		#define CONFIGURE_DRIVER_AMBAPP_GAISLER_APBUART
	#endif
#endif

#define CONFIGURE_DRIVER_AMBAPP_GAISLER_GRGPIO
#define CONFIGURE_DRIVER_PCI_GR_RASTA_IO
#define CONFIGURE_DRIVER_AMBAPP_GAISLER_GRPCI2


#include <drvmgr/drvmgr_confdefs.h>

/*
 * Resources
 */
#include <drvmgr/drvmgr.h>
#include <drvmgr/ambapp_bus_grlib.h>
#include <drvmgr/ambapp_bus.h>


#ifdef DEBUG
#define DBG(x...) printf(x)
#else
#define DBG(x...)
#endif


#include <gpiolib.h>

#define GPIO_PORTS		8
#define INPUT_PORT		0
#define OUTPUT_PORT		1


/* Cuidado en estos puertos, verificar
 *
 * Nota: Tener en cuenta que los GPIO de salida del EGSE son analogicos para
 * poder controlar la tension a 3,3V. Porque en la RASTA no se puede poner 5V.
 * La salida del EGSE es la entrada a la RASTA (input_ports) */

static const int input_ports[] = {24, 25, 26, 27, 28, 29, 30, 31};

static const int output_ports[] = {32, 33, 34, 35, 36, 37, 38, 39};

static uint8_t RESULT_SUCCESS = true;

static uint8_t RESULT_FAIL = false;

static const char *UART_DEV = "/dev/rastaio0/apbuart0";

	#define ISR_OK	1

static const char * SYNC = "sync\n";

#define BAUD 38400

#endif /* GPIO_CONFIG_H_ */
