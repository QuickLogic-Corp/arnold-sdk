#include <stdio.h>
#include "fc_config.h"
#include "gpio.h"

int main()
{
  int i;
  
  padcfg(); // Configure pads
  
  while(1) {
      setgpio5(1); //write 1 to PAD5
      printf("1\n");
      for(i=0;i<500000;i++) asm volatile("nop");
      setgpio5(0); //write 0 to PAD5
      for(i=0;i<500000;i++) asm volatile("nop");
      printf("0\n");
  }

  return 0;

}