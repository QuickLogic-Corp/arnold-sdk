#ifndef _ARNOLD_UDMA
#define _ARNOLD_UDMA

/* this list from arnold_soc/rtl/components/soc_peripherals.sv */

typedef struct {
  volatile unsigned int rx_saddr;
  volatile unsigned int rx_size; // 0x4
  volatile unsigned int rx_cfg; // 0x08
  volatile unsigned int reserved_0c;
  volatile unsigned int tx_saddr;
  volatile unsigned int tx_size;
  volatile unsigned int tx_cfg;
  volatile unsigned int reserved_1c;
  volatile unsigned int status;
  volatile unsigned int config;
  } udma_uart_typedef ;
#define ARNOLD_UART (0x1A102100)
#define udma_uart ((udma_uart_typedef *) ARNOLD_UART)

#endif /* _ARNOLD_UART */

