#include "gpio_config.h"

#define PORT1 8
#define PORT2 9

void *port1, *port2;

void gpio_isr(int irq, void *arg)
{
  struct gpiolib_config cfg;

  /* Mask away GPIO IRQ */
  cfg.mask = 0;
  cfg.irq_level = GPIOLIB_IRQ_LEVEL;
  cfg.irq_polarity = GPIOLIB_IRQ_POL_HIGH;

  if ( gpiolib_set_config(port2, &cfg) ){
    printf("Failed to configure gpio port %d\n", PORT2);
    exit(0);
  }
  
  printf("GPIO_ISR: %d, GOT %d\n", irq, (int)arg);
}

void setup_interrupt(int portnr)
{
  void *port;
  struct gpiolib_config cfg;
  int val;

  port = gpiolib_open(portnr);
  
  /* Mask away GPIO IRQ */
  cfg.mask = 0;
  cfg.irq_level = GPIOLIB_IRQ_LEVEL;
  cfg.irq_polarity = GPIOLIB_IRQ_POL_HIGH;
  
  if (gpiolib_set_config(port, &cfg) ){
    printf("Failed to configure gpio port %d\n", portnr);
    exit(0);
  }
  
  if ( gpiolib_irq_disable(port)){
    printf("Failed to disable IRQ on gpio port %d, disabling port "
           "while not enabled should fail. OK\n", portnr);
  } else {
      printf("Disable IRQ on port %d should fail\n", portnr);
    exit(0);
  }
  
  if ( gpiolib_irq_clear(port) ){
    printf("Failed to clear IRQ on gpio port %d\n", portnr);
    exit(0);
  }
  
  if ( gpiolib_irq_register(port, &gpio_isr, (void *)portnr) ){
    printf("Failed to register IRQ on gpio port %d\n", portnr);
    exit(0);
  }
  
  if ( gpiolib_irq_enable(port) ){
    printf("Failed to enable IRQ on gpio port %d\n", portnr);
    exit(0);
  }
  
  if ( gpiolib_get(port, &val) ){
    printf("Failed to get value of gpio port %d\n", portnr);
    exit(0);
  }
  printf("Current Value on GPIO: %d\n", val);
  
  /* Enable IRQ by unmasking IRQ */
  cfg.mask = 1;
  if ( gpiolib_set_config(port, &cfg) ){
    printf("Failed to configure gpio port %d\n", portnr);
    exit(0);
  }

  gpiolib_show(0, port);
  port2=port;

  
}

void print_ports_values(void *port1, void *port2)
{
  int port1val = -1;
  int port2val = -1;
  
  printf("GPIO PORTS Configured:\n");
  gpiolib_show(0, port1);
  gpiolib_show(0, port2);
        
  gpiolib_get(port1, &port1val);
  gpiolib_get(port2, &port2val);
  
  printf("Port 1 val: 0x%02X\n", port1val);
  printf("Port 2 val: 0x%02X\n", port2val);
}

void set_all_ports_zero()
{
  int i;
  void *port;

  for (i = 0; i < 46; i++)
    {
      port = gpiolib_open(i);
      rtems_task_wake_after(2);
      gpiolib_set(port, OUTPUT_PORT, 0);
      rtems_task_wake_after(2);
      gpiolib_close(port);
    }
}

rtems_task Init(rtems_task_argument argument)
{
        int portnr = 0;
        int retval = 0;

	setbuf(stdout, NULL);

        /* Print device topology */
        printf("Esta es la topologia del bus \n");
	drvmgr_print_topo();

        printf("Esta es la configuracion del PCI\n");
        pci_print();

        printf("#############################################################\n");
        printf("######### Configuracion de los GPIO #########################\n");

        while (true) {
          retval = gpiolib_show(portnr, NULL);
          if (retval < 0)
            break;
          else
            portnr++;
        }

        printf("Nro GPIOs: %d\n", portnr);

        printf("Seteo todos los puertos OUTPUT y a 0...\n");
        set_all_ports_zero();
        gpiolib_show(-1, NULL);
        printf("Done.");

        printf("\n\n\nConfigurando los GPIO que quiero...\n");
            
        // Abrir puertos
        // 1
        port1 = gpiolib_open(PORT1);
        if (port1 == NULL)
          {
            printf("Failed to open port 1\n");
            exit(EXIT_FAILURE);
          }
        // 2
        port2 = gpiolib_open(PORT2);
        if (port2 == NULL)
        {
          printf("Failed to open port 2\n");
          exit(EXIT_FAILURE);
        }

        // Setear puertos
        // 1
        if (gpiolib_set(port1, OUTPUT_PORT, 0) != 0)
          {
            gpiolib_close(port1);
            printf("Failed to set port1 OUTPUT\n");
            exit(EXIT_FAILURE);
          }
        // 2
        if (gpiolib_set(port2, INPUT_PORT, 0) != 0)
          {
            gpiolib_close(port2);
            printf("Failed to set port2 INPUT\n");
            exit(EXIT_FAILURE);
          }

        printf("\n\nEstado antes de setear HIGH en PORT[0]:\n");
        printf("=======================================\n");
        print_ports_values(port1, port2);
        
        gpiolib_set(port1, OUTPUT_PORT, 1);

        rtems_task_wake_after(2);

        printf("\n\nEstado despues de setear HIGH en PORT[0]:\n");
        printf("=========================================\n");
        print_ports_values(port1, port2);

        gpiolib_close(port1);
        gpiolib_close(port2);
	exit(EXIT_SUCCESS);
}
