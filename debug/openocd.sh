#!/bin/bash
# need to chmod this file
openocd -f ./particle-ftdi.cfg -f ./stm32f2x.cfg -c "gdb_port 3333" -c "\$_TARGETNAME configure -rtos FreeRTOS"
