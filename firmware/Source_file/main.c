/*
 * File:   main.c
 * Author: mohamed
 *
 * Created on January 13, 2026, 9:41 PM
 */
#include "config.h"
#include <stdio.h>
#include<stdbool.h>
#include <xc.h>
#include "i2c.h"
#include "DS3232.h"
#include "INT.h"
#include "LCD.h"
#include "UART.h"
#include "24LC256.h"
#define _XTAL_FREQ 4000000
#include "DHT11.h"
unsigned char String_time[50];//STRING FOR THE TIME ( HH:MM:SS)
unsigned char backtemp;
unsigned char backhum;
unsigned char backhour;
unsigned char backmin;
unsigned char String_test[55];
unsigned char time[15];
unsigned char String_date[55];//STRIGN DATE (DD/MM/YY)
unsigned char uart_String[55];//STRING THAT WILL BE SENDED VIA UART
unsigned char sec, min, hour,day,date,month,year;
unsigned char sec1, min1, hour1,day1,date1,month1,year1;
unsigned char i=1;//(SEC OR MIN HOUR OR DAY OR ...)
uint8_t temp=0;////float temp;
int8_t hum=0;
unsigned char var=0;//we used this var to store the temp every 60s (1 min)
unsigned char address=0;
unsigned char startmin;//the min that clock start with 
unsigned char starthour;//the hour that the clock start with
bool  timeshow=false;
/////
uint8_t read=0;
volatile unsigned char previous_RB4=1;
volatile unsigned char log_flag = 0;
volatile unsigned char show_flag = 0;

unsigned char write_index = 0;     // newest data position
unsigned char samples_count = 0;   // number of valid samples stored

/////
void lcd_display(unsigned char sec,unsigned char min,unsigned char hour,unsigned char  day,unsigned char date,unsigned char month,unsigned char year);
uint8_t Float_to_uint(float temp);
void last_1H();
void Read_show_temp();
////////
char  celsius_dot[] = {0B00111,0B00101,0B00111,0B00000,0B00000,0B00000,0B00000,0B00000};//add the celsius dot 
//////
void __interrupt() ISR(){
      if (PIR1bits.RCIF) { // UART received something
         read = RCREG;  // MUST read RCREG (clears RCIF)
         if(read=='1'){
    LATDbits.LATD7=1;
    }
    else if(read=='0'){
    LATDbits.LATD7=0;
    }
    else if(read=='h'){
    show_flag = 1;// request showing data
    }


}
   if(INTCONbits.RBIF){
       unsigned char current_portB=PORTB;//READ THE PORTB TO AVOID THE MISMATCH
       unsigned char current_RB4=(current_portB &0x10)>>4;
       
       if((current_RB4==1 && previous_RB4==0) || (current_RB4==0 && previous_RB4==1)){
           log_flag = 1;// request logging
         current_portB=PORTB;
         previous_RB4=(current_portB &0x10)>>4;
         
       }

       if(PORTB&0x40){//show the stored data in the EEPROM.
        show_flag = 1;// request showing data
       }
   INTCONbits.RBIF=0;
   }
   if(INTCON3bits.INT2IF){
        i=1;
        //copy the current value of the clock//
        sec1=BCD_dicimal(sec);
        min1=BCD_dicimal(min);
        hour1=BCD_dicimal(hour);
        date1=BCD_dicimal(date);
        month1=BCD_dicimal(month);
        year1=BCD_dicimal(year);
        ///
        //increment the current value 
        while(PORTCbits.RC2!=1){
          switch(i){
                case 1:
                    hour1=change(1,3,hour1,&i);
                 break;
                case 2:
                    min1=change(1,6,min1,&i);
                 break;
                case 3:
                    date1=change(2,2,date1,&i);       
                 break;
                case 4:
                    month1=change(2,5,month1,&i);
                 break;
                case 5:
                    year1=change(2,8,year1,&i);
                 break;
                default:
                  break;
        }
        }
      
    DS3232_SetTime_Only(sec1,min1,hour1,day1,date1,month1,year1);  //send the new time to DS3232
     __delay_ms(20);
        //AFTER CHANGE THE DATA WILL BE STORED IN EEPROM 
    send_data(sec1,min_BCD(min1),hour_BCD(hour1),day1,date_BCD(date1),month_BCD(month1),year_BCD(year1));
        
   LCD_clear();
   INTCON3bits.INT2IF=0;
   }
}


void main(void) {
    //
    TRISDbits.RD7=0;
    LATDbits.LATD7=0;
    //
    WDTCONbits.SWDTEN = 0;     //Force Watchdog OFF
    OSCCONbits.IRCF = 0b110; // 4 MHz
   // OSCCONbits.SCS  = 0b10;
    /////
    ADCON1 = 0x0F;
    TRISD = 0x00;
    TRISC|=0x07;//button for editing 
    LCD_init();
    uart_init();
    INT_config();
    IOC();//INTERRUPT ON CHANGE
    I2C_Init();
    /////////
    LCD_clear();
    LCD_add_character(celsius_dot,1);//add the celsius dot in the address 1
    read_data(&sec,&min,&hour,&day,&date,&month,&year);//read date from EEPROM THE TIME THAT WE STORE                                                 //SO THE TIME WILL START FROM THAT TIME
    __delay_ms(20);
    DS3232_SetTime_OnlySimple(sec_BCD(50),min,hour,day,date,month,year);//SET THE ALARM IN THE DS3232 
     __delay_ms(20);
     alarm1();//SET THE ALARM IN DS3232 EVERY MATCH IN SECANDS WITH 00
     startmin=min;
     starthour=hour;
    while(1){
        CLRWDT(); //restart WDT to avoid reset 
        DS3232_ReadTime(&sec,&min,&hour,&day,&date,&month,&year);// READ THE TIME AND PUT IN THE VARIABLE
        __delay_ms(10);
        lcd_display(sec,min,hour,day,date,month,year);//DISPLAY THE TIME & DAY & DATE IN LCD
        
        if(BCD_dicimal(sec)==30){
        alarm_flag_clear(); //CLEAR THE FLAG OF THE ALARM (HERE AN IOC WILL BE GENERATED)
      }
        __delay_ms(20);
        if(log_flag){
        log_flag = 0;
        Read_show_temp();
        }
        
        if(show_flag){
         show_flag=0;
         last_1H(); 
         }
        
        __delay_ms(300);
    }  
}






////////////////////
void lcd_display(unsigned char sec,unsigned char min,unsigned char hour,unsigned char  day,unsigned char date,unsigned char month,unsigned char year){
    //TIME
    LCD_cursor_set(1,1);
    sprintf(String_time,"T:%02u:%02u:%02u|",BCD_dicimal(hour),BCD_dicimal(min),BCD_dicimal(sec));
    LCD_write_string(String_time);
    
    LCD_write_variable(hum,3);
     LCD_cursor_set(1,15);
     LCD_write_char('%');
    //////
    /////test
   LCD_cursor_set(2,1);
   sprintf(String_date," %02u/%02u/%02u |",BCD_dicimal(date),BCD_dicimal(month),BCD_dicimal(year));
   LCD_write_string(String_date);
   ////////
   //TEMPERATURE
     LCD_write_variable(temp,3);
     LCD_cursor_set(2,15);
     LCD_write_char(1);//write the celsius dot 
     LCD_write_char('C');
     
}
 
    
    void last_1H(){
    unsigned char x=0;
    unsigned char y=0;
    unsigned char M=BCD_dicimal(startmin);
    unsigned char H=BCD_dicimal(starthour);
    unsigned char MM=BCD_dicimal(min);
    unsigned char HH=BCD_dicimal(hour);
    int start_value=M+H*60;
    int now_value=MM+HH*60;
    int interval=now_value-start_value;
    int index=(interval)%60;
       
            if(interval<60){
                while(x<index-1){
                    
               read_stored(&backmin,&backhour,&backtemp,&backhum,x);
               if(timeshow==false){
                    send_time_only(backmin,backhour,time);
                    timeshow=true;
                }
           
           temp_show(backtemp,backhum,String_test);
             x++;
                }
        }
            else{
                x=index;
                while(x<60){
                read_stored(&backmin,&backhour,&backtemp,&backhum,x);
                if(timeshow==false){
                
                    send_time_only(backmin,backhour,time);
                    timeshow=true;
                }
                temp_show(backtemp,backhum,String_test);
                x++;
                }
                while(y<index){
                read_stored(&backmin,&backhour,&backtemp,&backhum,y);
           
               temp_show(backtemp,backhum,String_test);
                y++;
                
                }
            
           }
       
        write_string("end\n");//send "end " to transmiting of the stored data
         timeshow=false;
    }


    

    void Read_show_temp(){
    var++;
         data_read(&temp,&hum);
         detail_show(sec,min,hour,date,month,year,temp,hum,uart_String); //SHOW THE TIME & DATE & TEMP IN TERMINAL VIA UART EVERY 30S
         if(var>=2){
             if(address>=60)
                 address=0;//to store 60 min (1h) 
                 store_temp(temp,hum,address,min,hour);//store temp , min , hour 
                 address++;   
                 var=0; 
         }
    }
