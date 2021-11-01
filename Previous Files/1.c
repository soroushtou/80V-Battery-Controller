/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 12/9/2017
Author  : 
Company : 
Comments: 


Chip type               : ATmega32A
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

#include <mega32a.h>

#include <delay.h>

// Alphanumeric LCD functions
#include <alcd.h>

// Declare your global variables here
int state;          // state of working
int ve;             // ve for engine on or off
int veng;           // v form engine batt
int i=1;
int j=0;
//int k=0;
float cs;            //charge state
float vb;            // v from 80v battery
float io;            // I out  80v battery
char dis[10] ;
char scr[10];

char lcd_refresh_time=0;
char adc_read_time=0;

int time;              // pulse from counter
char *str;

// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void)
{
// Place your code here

}

// External Interrupt 1 service routine
interrupt [EXT_INT1] void ext_int1_isr(void)
{
// Place your code here

}

// External Interrupt 2 service routine
interrupt [EXT_INT2] void ext_int2_isr(void)
{
// Place your code here

}

// Standard Input/Output functions
#include <stdio.h>
#include <stdlib.h>
// Timer1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Reinitialize Timer1 value
TCNT1H=0xF060 >> 8;
TCNT1L=0xF060 & 0xff;
// Place your code here

}

// Timer2 overflow interrupt service routine
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
// Reinitialize Timer2 value
TCNT2=0x64;
// Place your code here

}

// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
// Wait for the AD conversion to complete
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCW;
}




/*
void dimension (float vb,int ve,int veng,float io)
     {
       vb= read_adc(0);           // V BATT
       vb = (vb * 4.8828125)/1000;        
                    
       veng = read_adc(1);           // V OUT GENERATOR
       veng = (veng * 4.8828125)/1000;     
      if (veng <2)     // eng off
         {
         ve=0;
         }
      if (veng >2)
         {
         ve=1;
         }
         
       io= read_adc(2);           // V CURRENT TRANSDUSER
       io= (io * 4.8828125)/1000;   
            return vb,ve,io;
      }
 */
/*
int median (int a [50])           // median function
 {     
 int i, j,temp;
  for(i=0; i++; i<50) 
  {
  for(j=i; j++; j<50)
    {
    if( a[i]>a[j]){
    temp=a[i];
    a[i]=a[j];
    a[j]=temp; }
    }    
  }
    return a[25];
 } 
 
void interupt (int a)             //   interupt function
{
i++;
csp[i]=read_adc(0);
if(i==50) median(csp);

}

*/



void lcd (int state, float vb, int ve, float io)    // lcd function
{
lcd_clear();

switch (state) {
 case 1:  lcd_gotoxy(0,0); lcd_putsf("SYS IS READY"); break;
 case 2:  lcd_gotoxy(0,0); lcd_putsf("ECONOMIC MODE");  break;
 case 3:  lcd_gotoxy(0,0); lcd_putsf("MEDIUM MODE");    break;
 case 4:  lcd_gotoxy(0,0); lcd_putsf("HIGHT POWER MODE");   break;
 case 5:  lcd_gotoxy(0,0); lcd_putsf("END OF WORKING");    break;
 case 6:  lcd_gotoxy(0,0); lcd_putsf("CHARGING MODE");  break;
  }; 

switch (ve) 
   {
 case 0: lcd_gotoxy(0,2); lcd_putsf("ENGINE:OFF"); break;
 case 1: lcd_gotoxy(0,2); lcd_putsf("ENGINE: ON");  break;  
  };                                                     
   cs= vb * 26.881;
   //sprintf(dis,"  SOC=%2.1d ",cs);
   
   lcd_putsf(" SOC="); 
   ftoa(cs,1,dis);
   lcd_puts(dis);
   lcd_putsf("%");
  
     
   lcd_gotoxy(0,3);
   lcd_putsf("VB=");
   ftoa(vb,1,dis);
   lcd_puts(dis);
   lcd_putsf(" V");   
     
     
   lcd_gotoxy(10,3);
   lcd_putsf("IO=");
   ftoa(io,1,dis);
   lcd_puts(dis);
   lcd_putsf(" Amp");
 
   delay_ms(15);  
//      sprintf(dis,"VB=%2d  IO=%2d",vb,io);
//     lcd_puts(dis);
}

void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=P Bit6=P Bit5=P Bit4=P Bit3=P Bit2=P Bit1=P Bit0=P 
PORTB=(1<<PORTB7) | (1<<PORTB6) | (1<<PORTB5) | (1<<PORTB4) | (1<<PORTB3) | (1<<PORTB2) | (1<<PORTB1) | (1<<PORTB0);

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
DDRD=(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (1<<DDD3) | (1<<DDD2) | (1<<DDD1) | (1<<DDD0);
// State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 7.813 kHz
// Mode: Normal top=0xFF
// OC0 output: Disconnected
// Timer Period: 32.768 ms
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);
TCNT0=0x00;
OCR0=0x00;


// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 125.000 kHz
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 0.52429 s
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 31.250 kHz
// Mode: Normal top=0xFF
// OC2 output: Disconnected
// Timer Period: 4.992 ms
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (0<<CS20);
TCNT2=0x64;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Low level
// INT1: On
// INT1 Mode: Low level
// INT2: On
// INT2 Mode: Falling Edge
GICR|=(1<<INT1) | (1<<INT0) | (1<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(1<<INTF1) | (1<<INTF0) | (1<<INTF2);

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 9600
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
UBRRH=0x00;
UBRRL=0x33;

// Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);

// ADC initialization
// ADC Clock frequency: 1000.000 kHz
// ADC Voltage Reference: AREF pin
// ADC Auto Trigger Source: ADC Stopped
ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

// SPI initialization
// SPI disabled
SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTC Bit 0
// RD - PORTC Bit 1
// EN - PORTC Bit 2
// D4 - PORTC Bit 4
// D5 - PORTC Bit 5
// D6 - PORTC Bit 6
// D7 - PORTC Bit 7
// Characters/line: 20
lcd_init(20);

// Global enable interrupts
#asm("sei")

lcd_clear();
lcd_gotoxy(0,5);
lcd_putsf("     WELCOME");


            vb= read_adc(0);              // V BATT
            vb = (vb * 4.8828125)/1000;        
            veng = read_adc(1);           // V OUT GENERATOR
            veng = (veng * 4.8828125)/1000;
             if (veng <2)     // eng off
               {
                ve=0;
               }
             if (veng >2)
               {
                ve=1;
               }
            io= read_adc(2);           // V CURRENT TRANSDUSER
            io= (io * 5000)/1023;      // io in mvol 
            io= io/7.6;                //io in amp
            
            PORTD=0b00000000;
            
            TCNT1 = 0;         // time being equal zero in start    
            
 delay_ms(100);           
            

while (1)
      {
      // Place your code here 
                     
      time= TCNT1;        //how much time spend to one cycle
      TCNT1=0;
      
      itoa(time, str);

      j++;                      // 
       if(j>0){j=0;lcd_refresh_time++;adc_read_time++;}
       if(adc_read_time>0)
      {
       adc_read_time=0;
       vb= read_adc(0);           // V BATT
       veng = read_adc(1);           // V OUT GENERATOR
       io= read_adc(2);           // V CURRENT TRANSDUSER  
       vb = (vb * 4.8828125)/1000.0;
       veng = (veng * 4.8828125)/1000.0;     
      if (veng <2)     // eng off
         {
         ve=0;
         }
      if (veng >2)
         {
         ve=1;
         }
         
       io= (io * 5000)/1023;      // io in mvol 
       io= io/7.6;                //io in amp 
       
       if (PINB.0==0){state=1;}
       if (PINB.1==0){state=2;}
       if (PINB.2==0){state=3;}
       if (PINB.3==0){state=4;}
       if (PINB.4==0){state=5;}
       if (PINB.5==0){state=6;}    
//      scanf("%b%b%b%",vb,ve,io);
//    printf("vb= %b\n ve= %b\n io= %b\n",vb,ve,io);

//      {ftoa(k, 0, scr);      // for state
//      printf("\n\rN=");
//      puts(scr);
//      k=0;              // n=0
//      }




      {ftoa(time, 0, scr);      // for state
      //printf("\n\rtime=");
      printf("\n\r");
      puts(scr);
      }

      {ftoa(state, 0, scr);      // for state
      //printf("state=");   
      printf(",");
      puts(scr);
      }
      
      {ftoa(ve, 0, scr);      // for ve
      //printf("ve=");
      printf(",");
      puts(scr);
      }
      
      {ftoa(vb, 3, scr);      // for vb
      //printf("vb=");    
      printf(",");
      puts(scr);
      }
      {ftoa(io, 3, scr);      // for io
      //printf("io=");
      printf(",");
      puts(scr);
      }
      
      
      } 
       
      if(lcd_refresh_time>100)
      {                            
      lcd_refresh_time=0;
      lcd(state,vb,ve,io);        // lcd function
      }          

       
       
          
      if (PINB.0==0) // step 0      SYS IS READY
      {     state=1;
           PORTD=0b00000000;

      }
            
        
        
      if (PINB.1==0)   // step 1            economic mode
      {      
            i=1; 
            state=2;  

        if ((vb <=3.3) && (ve==0))    // ENGINE OFF
        {
        PORTD=0b01100000;      //  FUEL & START        
        
        }
        
        if (ve==1)             // ENGINE ON
            {
            PORTD=0b10100000;          //  FUEL & GENARATOR
            }
        if (vb >=3.6)
            {
            PORTD=0b00000000;          // FUEL & GENARATOR OFF
            i=0;
            }   
      }  

      
       if (PINB.2==0)   // step 2      MEDIUM MODE
      {      
            i=1;
            state=3;

            
      if ((vb <=3.4) && (ve==0))    // ENGINE OFF
        {
        PORTD=0b01100000;      //  FUEL & START
        }
        if (ve==1)             // ENGINE ON
            {
            PORTD=0b10100000;          //  FUEL & GENARATOR
            }
        if (vb >=3.7)
            {
            PORTD=0b00000000;          // FUEL & GENARATOR OFF
            i=0;
            }   
      }
      
      
        if (PINB.3==0)   // step 2      MEDIUM MODE
      {      
            i=1;
            state=4;

            
      if ((vb <=3.5) && (ve==0))    // ENGINE OFF
        {
        PORTD=0b01100000;      //  FUEL & START
        }
        if (ve==1)             // ENGINE ON
            {
            PORTD=0b10100000;          //  FUEL & GENARATOR
            }
        if (vb >=3.8)
            {
            PORTD=0b00000000;          // FUEL & GENARATOR OFF
            i=0;
            }   
      }

       if (PINB.4==0)   // step 2      MEDIUM MODE
      {      
            i=1;
            state=5;

            
      if ((vb <=3.2) && (ve==0))    // ENGINE OFF
        {
        PORTD=0b01100000;      //  FUEL & START
        }
        if (ve==1)             // ENGINE ON
            {
            PORTD=0b10100000;          //  FUEL & GENARATOR
            }
        if (vb >=3.6)
            {
            PORTD=0b00000000;          // FUEL & GENARATOR OFF
            i=0;
            }   
      }
      
       
      if (PINB.5==0)   // step 5  CHARGING MODE
       {      
            i=1;
            state=6;

            
      if ((vb <=3.2) && (ve=0))    // ENGINE OFF
        {
        PORTD=0b01100000;      //  FUEL & START
        }
        if (ve==1)             // ENGINE ON
            {
            PORTD=0b10100000;          //  FUEL & GENARATOR
            }
        if (vb >=3.924)
            {
            PORTD=0b00000000;          // FUEL & GENARATOR OFF
            i=0;
            lcd_gotoxy(0,1);
            lcd_putsf("CHARGING COMPLETE");
            delay_ms(10);
            
            }   
      }

      
     }
}
        
