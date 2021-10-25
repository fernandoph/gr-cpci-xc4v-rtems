#include "gpio_config.h"

#define PORT1 0
#define PORT2 1

void print_ports_values(void *port1, void *port2)
{
  int port1val = -1;
  int port2val = -1;
  
  printf("GPIO PORTS Configured:\n");
  gpiolib_show(PORT1, NULL);
  gpiolib_show(PORT2, NULL);
        
  gpiolib_get(port1, &port1val);
  gpiolib_get(port2, &port2val);
  
  printf("Port 1 val: 0x%02X\n", port1val);
  printf("Port 2 val: 0x%02X\n", port2val);
}

void test_one_port(void *port)
{
  int val = -1;
  
  gpiolib_show(PORT1, NULL);
  gpiolib_set(port, OUTPUT_PORT, 0);
  gpiolib_get(port, &val);
  printf("Value: %d\n", val);

  gpiolib_set(port, OUTPUT_PORT, 1);
  gpiolib_get(port, &val);
  printf("Value: %d\n", val);
  gpiolib_get(port, &val);
  printf("Value: %d\n", val);

}

void set_all_ports_zero()
{
  int i;
  void *port;

  for (i = 0; i < 46; i++)
    {
      port = gpiolib_open(i);
      gpiolib_set(port, OUTPUT_PORT, 0);
      gpiolib_close(port);
    }
}

rtems_task Init(rtems_task_argument argument)
{
        void *port1, *port2;
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
        //set_all_ports_zero();
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

        //test_one_port(port1);
        //exit(EXIT_SUCCESS);

        
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

        printf("Antes:\n");
        print_ports_values(port1, port2);
        
        gpiolib_set(port1, OUTPUT_PORT, 0x1);

        rtems_task_wake_after(2);

        printf("Despues:\n");
        print_ports_values(port1, port2);

	exit(EXIT_SUCCESS);
}
