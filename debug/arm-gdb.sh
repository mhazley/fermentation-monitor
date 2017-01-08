#!/bin/bash
arm-none-eabi-gdb -ex "target remote localhost:3333" ../build/brewbot-photon.elf
