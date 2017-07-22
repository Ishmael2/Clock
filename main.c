#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

#define ctrl PORTD // We are using port D
#define en 2       // enable signal pin 2
#define rw 1       // read/write signal pin 1
#define rs 0       // register select signal pin 0

#define ctrl_lcd PORTC
#define set_hour 3
#define set_minute 4

// Global variables declarations
unsigned char hours = 23;
unsigned char minutes = 59;
unsigned char seconds = 59;
char time[] = "00:00:00";

void lcd_command(unsigned char cmd);
void lcd_init(void);
void lcd_data(unsigned char data);
void lcdCommand(char);
void lcdData(char);
void lcd_print(unsigned char *str);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_update_time(void);

// Timer counter 1 compare match A interrupt service routine/interrupt handler
ISR(TIMER1_COMPA_vect);

int main() {
  DDRD = 0xFF; // Setting DDRD to output // setting for port D
  lcd_init();  // initialization of LCD function
  _delay_ms(30);

  lcd_gotoxy(1, 1);           // Go to the location 1,1 of lcd
  lcd_print("AVR-Tutorials"); // Print the text
  lcd_gotoxy(1, 2);           // Go to the location 1,2 of lcd
  lcd_print("disco");         // Print the text

  // ctrl_lcd = (1 << set_hour | 1 << set_minute);

  TCCR1B = (1 << CS12 | 1 << WGM12);
  OCR1A = 15625 - 1;
  TIMSK1 = 1 << OCIE1A;
  sei();

  while (1) {
    // if (!(ctrl_lcd & (1 << set_hour))) {
    //   hours++;
    //   if (hours > 23) {
    //     /* code */
    //     hours = 0;
    //   }
    //   /* code */
    // }
    // if (!(ctrl_lcd & (1 << set_minute))) {
    //   /* code */
    //   minutes++;
    //   if (minutes > 59) {
    //     minutes = 0;
    //   }
    // }
    // _delay_ms(250);
  };

  return 0;
}

// Function for updating time
void lcd_update_time(void) {
  unsigned char temp;
  lcd_gotoxy(1, 2);

  itoa(hours / 10, temp, 10);
  lcd_print(temp);
  itoa(hours % 10, temp, 10);
  lcd_print(temp);
  lcd_print(":");

  itoa(minutes / 10, temp, 10);
  lcd_print(temp);
  itoa((minutes % 10), temp, 10);
  lcd_print(temp);
  lcd_print(":");

  itoa(seconds / 10, temp, 10);
  lcd_print(temp);
  itoa(seconds % 10, temp, 10);
  lcd_print(temp);
}

ISR(TIMER1_COMPA_vect) {
  seconds--;

  if (seconds == 0) {
    /* code */
    seconds = 59;
    minutes--;
  }

  if (minutes == 0) {
    /* code */
    minutes = 59;
    hours--;
  }

  // if (hours > 23) {
  //   /* code */
  //   hours = 0;
  // }
  lcd_update_time();
}

// Function moving to a given position on the LCD screen
void lcd_gotoxy(unsigned char x, unsigned char y) {
  unsigned char firstCharAdr[] = {0x80, 0xC0, 0x94, 0xD4};
  lcdCommand(firstCharAdr[y - 1] + x - 1);
  _delay_ms(0.1);
}

void lcd_init(void) {
  lcdCommand(0x02); // To initialize LCD in 4-bit mode.
  _delay_ms(1);
  lcdCommand(0x28); // To initialize LCD in 2 lines, 5X7 dots and 4bit mode.
  _delay_ms(1);
  lcdCommand(0x01); // Clear LCD
  _delay_ms(1);
  lcdCommand(0x0E); // Turn on cursor ON
  _delay_ms(1);
  lcdCommand(0x80); // —8 go to first line and –0 is for 0th position
  _delay_ms(1);
  return;
}

void lcdCommand(char cmd_value) {
  char cmd_value1;
  cmd_value1 = cmd_value & 0xF0;          // Mask lower nibble
                                          // because PD4-PD7 pins are used.
  lcd_command(cmd_value1);                // Send to LCD
  cmd_value1 = ((cmd_value << 4) & 0xF0); // Shift 4-bit and mask
  lcd_command(cmd_value1);                // Send to LCD
}

void lcdData(char data_value) {
  char data_value1;
  data_value1 = data_value & 0xF0;          // Mask lower nibble
  lcd_data(data_value1);                    // because PD4-PD7 pins are used.
  data_value1 = ((data_value << 4) & 0xF0); // Shift 4-bit and mask
  lcd_data(data_value1);                    // Send to LCD
}

void lcd_command(unsigned char cmd) {
  ctrl = cmd;
  ctrl &= ~(1 << rs); // RS = 0 for command
  ctrl &= ~(1 << rw); // RW = 0 for write
  ctrl |= (1 << en);  // EN = 1 for High to Low pulse
  _delay_ms(1);
  ctrl &= ~(1 << en); // EN = 0 for High to Low pulse
  _delay_ms(40);
  return;
}

void lcd_data(unsigned char data) {
  ctrl = data;
  ctrl |= (1 << rs);  // RS = 1 for data
  ctrl &= ~(1 << rw); // RW = 0 for write
  ctrl |= (1 << en);  // EN = 1 for High to Low pulse
  _delay_ms(1);
  ctrl &= ~(1 << en); // EN = 0 for High to Low Pulse
  _delay_ms(40);
  return;
}

void lcd_print(
    unsigned char *str) { // store address value of the string in pointer *str
  int i = 0;
  while (str[i] !=
         '\0') {     // loop will go on till the NULL character in the string
    lcdData(str[i]); // sending data on LCD byte by byte
    i++;
  }
  return;
}
