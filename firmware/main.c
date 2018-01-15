#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <uart.h>
#include <time.h>
#include <generated/csr.h>
#include <generated/mem.h>
#include "flags.h"
#include <console.h>
#include <system.h>

#include "config.h"
#include "ci.h"
#include "processor.h"
#include "pattern.h"


int main(void)
{
	irq_setmask(0);
	irq_setie(1);
	uart_init();
#ifdef CSR_HDMI_OUT0_I2C_W_ADDR
	hdmi_out0_i2c_init();
#endif

	puts("\nNeTV2 CPU testing software built "__DATE__" "__TIME__);

	config_init();
	puts("here\n");
	time_init();
	puts("here\n");

	processor_init();
	puts("here\n");
	processor_update();
	puts("here\n");
	processor_start(config_get(CONFIG_KEY_RESOLUTION));
	puts("here\n");

	ci_prompt();
	while(1) {
		processor_service();
		ci_service();
		pattern_service();
	}

	return 0;
}
