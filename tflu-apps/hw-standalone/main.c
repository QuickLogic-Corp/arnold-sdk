
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.



#include <stdio.h>
#include "pulp.h"
#include "programFPGA.h"
#include "arnold_apb_ctl.h"
#include "apb_conv2d.h"


#include "result.h"
#include "data9.h"

#define REFCLK 32660
int spow2 (int exp) {
  int i, ret;
  ret = 1;
  if (exp == 0) return ret;
  else
    for (i = 0; i < exp; ret = ret * 2, i++);
  return ret;
}

int prog_fll(int fll, int mult, int div) {
  int mult_out;
  int ret = (REFCLK * (mult+1))/spow2(div?div-1:0);
  int i;
  i = 0x80000000 | 
    (div << 26) | (0xf0 << 16) | mult; // 2x RefClock enable FLL
  switch (fll) {
  case 0:
    *(unsigned int*)0x1a100008 = 0x42004107; // select ref clock as input
    *(unsigned int*)0x1a100004 = i;
    printf("FLL0 m=%d, div= %d %08x %08x %08x %08x\n",mult,div,
	   *(unsigned int*)0x1a100000,
	   *(unsigned int*)0x1a100004,
	   *(unsigned int*)0x1a100008,
	   *(unsigned int*)0x1a10000C);
    mult_out = *(unsigned int*)0x1a100000;
    break;
  case 1:
    *(unsigned int*)0x1a100018 = 0x42004107; // select ref clock as input
    *(unsigned int*)0x1a100014 = i;
    printf("FLL1=1 m=%d, div= %d %08x %08x %08x %08x\n",mult,div,
	   *(unsigned int*)0x1a100010,
	   *(unsigned int*)0x1a100014,
	   *(unsigned int*)0x1a100018,
	   *(unsigned int*)0x1a10001C);
        mult_out = *(unsigned int*)0x1a100010;
    break;
  case 2:
    //   *(unsigned int*)0x1a10002C = 0x00808000;
    *(unsigned int*)0x1a100028 = 0x42004107; // select ref clock as input
    *(unsigned int*)0x1a100024 = i;
    printf("FLL2 m=%d, div= %d %08x %08x %08x %08x\n",mult,div,
	   *(unsigned int*)0x1a100020,
	   *(unsigned int*)0x1a100024,
	   *(unsigned int*)0x1a100028,
	   *(unsigned int*)0x1a10002C);
            mult_out = *(unsigned int*)0x1a100020;
    break;
  }
  ret  = (REFCLK * (mult_out+1))/spow2(div?div-1:0);
  return ret;
}

int main()
{
  int i, j, k, elapsed_clocks;
  fpga_typedef *fpga;
  unsigned int result_len; 
  int pixel;
  signed int f1,f2,f3,f4,f5,f6,f7,f8;
  unsigned int s1,s2,s3,s4,s5,s6,s7,s8;
  int print_on, debug_pixel, skip;
  

  printf("Attempted to program FLL0 %d Hz\n",prog_fll(0,28000,2));  // 402 MHz
  printf("Attempted to program FLL1 %d Hz\n",prog_fll(1,6100,3));   //50 MHz
  printf("Attempted to program FLL2 %d Hz\n",prog_fll(2,1200,3));  // 61 MHs

  printf("\n***Test data from %s\n\n",testcase);
  
  print_on = 0;
  for (i = 0; i < sizeof(result_base)/4; i++)
    result_base[i] = 0xdeadbeef;
  static unsigned int *tmp ;
  if (print_on) {
    printf("Pixels:\n");
    tmp = (unsigned int*)pixel_base;
    for (i = 0; i < 16; i++) {
      printf("%08x %c",tmp[i],(i+1) % 4 ? 0 : 0xA);
    }
    printf("Filters:\n");
    tmp = (unsigned int*)filter_base;
    for (i = 0; i < 16; i++) {
      printf("%08x %c",tmp[i],(i+1) % 4 ? 0 : 0xA);
    }
    printf("Bias:\n");
    tmp = (unsigned int*)bias_base;
    for (i = 0; i < sizeof(bias_base)/4; i++) {
      printf("%08x %c",tmp[i],(i+1) % 4 ? 0 : 0xA);
    }
    printf("Expect:\n");
    tmp = (unsigned int*)expect;
    for (i = 0; i < 16; i++) {
      printf("%08x %c",tmp[i],(i+1) % 4 ? 0 : 0xA);
    }
  }

  
  fpga = (fpga_typedef *)0x1a200000;
  
  apb->fpga_clk = 2; // FLL to CLK0 of FPGA
  programFPGA();
  
  apb->padfunc0 = 0xaaaaaaaa; //0xaaa82aaa;  // gpio 15-00 from efpga .. uart
  apb->padfunc1 = 0xaaaaaaaa;  // gpio 31-16 from efpga

  apb->fpga_reset = 1;
  apb->fpga_reset = 0xF;
  
  apb->fpga_gate  = 0xFFFF;
  fpga->debug_mux = 0x42;
  printf("****Debug Mux = %x\n", fpga->debug_mux);
	 
  result_len = (filters * width * height)/4;
  fpga->width = width;
  fpga->height = height;
  fpga->channels = channels;
  fpga->filters = filters;
  fpga->total_pixels = fpga->width * fpga->height;
  
  fpga->pixel_base = pixel_base;
  fpga->filter_base = filter_base;
  fpga->bias_base = bias_base;
  fpga->result_base = result_base;
  fpga->quant = quant;
  
  printf("Width = %d\n", fpga->width);
  printf("Height = %d\n", fpga->height);
  printf("Channels = %d\n", fpga->channels);
  printf("Filters = %d\n", fpga->filters);
  printf("Quant = %x\n", fpga->quant);
    
  printf("Total_pixels = %d\n", fpga->total_pixels);
  printf("Pixel_base   = 0x%05x (%08x)\n", fpga->pixel_base,pixel_base);
  printf("Filter_base  = 0x%05x (%08x)\n", fpga->filter_base,filter_base);
  printf("Bias_base    = 0x%05x (%08x)\n", fpga->bias_base,bias_base);
  printf("Result_base  = 0x%05x (%08x)\n", fpga->result_base,result_base);
  printf("Expect       = (%08x)\n",expect);  

  
  elapsed_clocks = fpga->clocks;
  fpga->control = 1;  // start 2dconv
    
  while (fpga->control & 0x1) { } // wait till start is cleared

  printf ("Elapsed Clocks = %d - %d\n",fpga->clocks,elapsed_clocks );
  printf ("fll0 = %d, fll1 = %d, fll2= %d\n",
	  *(unsigned int*)0x1a100000,
	  *(unsigned int*)0x1a100010,
	  *(unsigned int*)0x1a100020);
  printf ("0 = %x, 1 = %x, 2= %x\n",
	  *(unsigned int*)0x1a100004,
	  *(unsigned int*)0x1a100014,
	  *(unsigned int*)0x1a100024);

  j = 0;
  tmp = expect;
  for (i = 0; i < result_len ; i++) {
    if (result_base[i] != tmp[i]) {
      j = j + 1;
      if (print_on) {
	printf ("post Result[0x%4x] = 0x%08x exp = 0x%08x\n",
		i,result_base[i],tmp[i]);
      }
    }
  }

  if (j == 0)
    printf("****All data passed!\n");
  else
    printf("**** %d mismatches detected -- Test Failed\n",j);

  return 0;
}

	 
