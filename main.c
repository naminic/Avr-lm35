/*****************************************************
Chip type               : ATmega32
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*****************************************************/

#include <mega32.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>
#include <stdlib.h>

float adc_data;
unsigned char init_temp[7],lcd_temp[10];

#define ADC_VREF_TYPE 0x20

// ADC interrupt service routine
interrupt [ADC_INT] void adc_isr(void)
{
  adc_data=ADCH;        //read result of adc conversation.
  ADCSRA |=0x10;        //clear interrupt flag by write 1 to it(bit 4).
}

void main(void)
{

// ADC initialization
ADMUX=0x06 | (ADC_VREF_TYPE & 0xff);     //select channel 6 (pin 6 of port A).
delay_us(10);

ADCSRA=0x8E;           //8 bit mode, 125kHz clock and AREF pin as a voltage reference.
SFIOR &=0x0F;

lcd_init(16); //initiate lcd.

#asm("sei")   //enable Global interrupts

while (1)
      {
         ADCSRA |=0x40;     //start the adc conversation and enable interrupt. 
         delay_ms(400);
         
         adc_data=(adc_data*148)/114;   //these 3 line prepares adc_data to display in lcd.
         ftoa(adc_data,1,init_temp);           
         sprintf(lcd_temp,"%s \xdfC",init_temp);
             
         lcd_clear();
         lcd_gotoxy(0,0);
         lcd_putsf("The temp is:");
         lcd_gotoxy(0,1);
         lcd_puts(lcd_temp);    
      }
}
