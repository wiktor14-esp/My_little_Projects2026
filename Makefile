CPB = arm-none-eabi-objcopy
CPO = arm-none-eabi-gcc 

CFLAG = -c -mcpu=cortex-m3 -mthumb -std=gnu11
OFLAG = -O binary 
EFLAG = -nostdlib -T linker.ld *.o -o 

TTY = /dev/ttyUSB0
FFLAG = stm32flash -w $^ -g 0x80000000 
flash:stm32.bin
	$(FFLAG) $(TTY)

stm32.bin:stm32.elf
	$(CPB) $(OFLAG) $^ $@

stm32.elf:main.o startup.o
	$(CPO) $(EFLAG) $@	

main.o : main.c
	$(CPO) $(CFLAG) $^ -o $@

startup.o : startup.c
	$(CPO) $(CFLAG) $^ -o $@



clean:
	rm -rf *.o *.elf *.bin

