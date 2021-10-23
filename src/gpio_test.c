#include "gpio_config.h"

#define PORT1 10
#define PORT2 11

void print_ports_values()
{
  int port1val = -1;
  int port2val = -1;
  
  printf("GPIO PORTS Configured:\n");
  gpiolib_show(PORT1, NULL);
  gpiolib_show(PORT2, NULL);
        
  gpiolib_get(NULL, &port1val);
  gpiolib_get(NULL, &port2val);
  
  printf("Port 1 val: 0x%02X\n", port1val);
  printf("Port 2 val: 0x%02X\n", port2val);
}

rtems_task Init(rtems_task_argument argument)
{
	void *ports[GPIO_PORTS];	/**< GPIO ports reference */
	int level[GPIO_PORTS]; 		/**< to store the read value from the pins */
        void *port1, *port2;

        int port1val = -1;
        int port2val = -1;
        
	setbuf(stdout, NULL);

        /* Print device topology */
        printf("Esta es la topologia del bus \n");
	drvmgr_print_topo();

        printf("Esta es la configuracion del PCI\n");
        pci_print();

        printf("#############################################################\n");
        printf("######### Configuracion de los GPIO #########################\n");

        gpiolib_show(-1, NULL);

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

        printf("Antes:\n");
        print_ports_values();
        
        gpiolib_set(port1, OUTPUT_PORT, 0xABCD);

        printf("Despues:\n");
        print_ports_values();

	exit(EXIT_SUCCESS);
}
