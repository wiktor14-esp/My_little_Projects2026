CPB = arm-none-eabi-objcopy
CPO = arm-none-eabi-gcc 

CFLAG = -c -mcpu=cortex-m3 -mthumb -std=gnu11 $(INC_DIRS)
OFLAG = -O binary 
EFLAG = -nostdlib -T linker.ld $(OBJS) -o 

TTY = /dev/ttyUSB0
FFLAG = stm32flash -w $^

INC_DIRS = 	-I. \
			-IDRIVERS/GPIO
SRCS = $(wildcard *.c) $(wildcard DRIVERS/*/*.c) 
OBJS = $(SRCS:.c=.o)

flash:stm32.bin
	$(FFLAG) $(TTY)

stm32.bin:stm32.elf
	$(CPB) $(OFLAG) $^ $@

stm32.elf:$(OBJS)
	$(CPO) $(EFLAG) $@	

%.o: %.c
	$(CPO) $(CFLAG) $^ -o $@

clean:
	rm -rf *.o *.elf *.bin

