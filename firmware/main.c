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
	time_init();

	processor_init();
	processor_update();
	processor_start(config_get(CONFIG_KEY_RESOLUTION));

#ifdef DMA_WRITER_BASE
	// do it here to be sure values are stabilized when we'll use them
	dma_writer_slot0_base_write(0x00000000);
	dma_writer_slot0_base_write(0x10000000);
	dma_writer_length_write(1920*1080*32);
#endif

#ifdef DMA_READER_BASE
	// do it here to be sure values are stabilized when we'll use them
	dma_reader_slot0_base_write(0x00000000);
	dma_reader_slot0_base_write(0x10000000);
	dma_reader_length_write(1920*1080*32);
#endif

	ci_prompt();
	while(1) {
		processor_service();
		ci_service();
		pattern_service();
	}

	return 0;
}
