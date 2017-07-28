# Clock

This is a clock that counts upwards. Built using an atmega328p and lcd 1602A screen. The code runs on C. It shows the days, hours, minutes and seconds

## Getting Started

Basic components required are:

1. Microcontroller (Atmega 328p-pu)
2. Crystal for timing operations (16Mhz crystal)
3. Potentiometer (5 kiloOhm)
4. Power source
5. Wires (male to female and male to male wires)
6. A programmer (usbtinyisp or any other)
7. Board (copper board or bread board)
8. Wires

Set up the circuit as shown in the figure below

<p align="center">
	<img src="./Assets/schematic.png" />
</p>

### Prerequisite

In this case I was using the terminal from linux

Install avr gcc by running the following command
```
sudo apt-get install gcc-avr binutils-avr avr-libc
```
Install avrdude by running
```
sudo apt-get install avrdude
```

### Installing

To upload the code to the chip, one can run these commands:

The following compiles the code to a .obj file
 ```
avr-gcc -g -Os -Wall -mcall-prologues -mmcu=atmega328p -fno-exceptions -o main.obj main.c
```    
The following code compiles the code to a .hex file
```
avr-objcopy -R .eeprom -O ihex main.obj main.hex
```
The following code uploads the .hex file to the chip (in this case I was using a usbtinyisp)
```
avrdude -b 19200 -c usbtiny -p m328p -U flash:w:main.hex para gravar no atmega
```

## Acknowledgements

I would like to thank the following books and sites to help me develop this project

1. http://www.avr-tutorials.com/projects/atmega16-avr-microcontroller-lcd-digital-clock

2. Avr Microcontroller and Embedded Systems The: Using Assembly and C
Book by Muhammad Ali Mazidi, Sarmad Naimi, and Sepehr Naimi
