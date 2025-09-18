arm-none-eabi-as -mcpu=cortex-m4 -g -mthumb main.s -o main.o
arm-none-eabi-ld -T link.ld main.o -o main.elf
arm-none-eabi-objcopy -O binary main.elf main.bin
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -nostdlib -ffreestanding -c -o main.o main.c