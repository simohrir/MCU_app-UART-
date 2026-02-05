#include "LCD.h"


#define DS3232_WRITE 0xD0//address + WRITE(0)
#define DS3232_READ  0xD1//DS3232 address + READ(1)

uint8_t sec_BCD(uint8_t val){
    uint8_t sec=0;
    if(val<=59){
        unsigned char tens=val/10;
        unsigned char ones=val%10;
        sec=tens<<4;
        sec|=ones; 
    }
    return sec;
}
uint8_t min_BCD(uint8_t val){
    uint8_t min=0;
    if(val<=59){
        unsigned char tens=val/10;
        unsigned char ones=val%10;
        min=tens<<4;
        min|=ones; 
    }
    return min;
}
uint8_t hour_BCD(uint8_t val){
    uint8_t heur=0;
    if(val<=23){
        unsigned char tens=val/10;
        unsigned char ones=val%10;
        heur=tens<<4;
        heur|=ones; 
        heur&=0x3F;
    }
    return heur;
}
uint8_t day_BCD(uint8_t val){
    if(val<=7)
    return val;
    else 
    return 0;   
}
uint8_t date_BCD(uint8_t val){
    uint8_t date=0;
    if(val<=31){
        unsigned char tens=val/10;
        unsigned char ones=val%10;
        date=tens<<4;
        date|=ones; 
    }
    return date;
}
uint8_t month_BCD(uint8_t val){
    uint8_t month=0;
    if(val<=12){
        unsigned char tens=val/10;
        unsigned char ones=val%10;
        month=tens<<4;
        month|=ones;   
    }
    return month;
}
uint8_t year_BCD(uint8_t val){
    uint8_t year=0;
    if(val<=99){
        unsigned char tens=val/10;
        unsigned char ones=val%10;
       year=tens<<4;
        year|=ones;
        
    }
    return year;
}
uint8_t BCD_dicimal(uint8_t BCD){
    unsigned char tens=((BCD&0XF0)>>4)*10;
    unsigned char ones=(BCD&0X0F);
    return tens+ones;
}

    void DS3232_SetTime_OnlySimple(uint8_t sec,uint8_t min,uint8_t hour,uint8_t day,uint8_t date,uint8_t month,uint8_t year){
    I2C_Start(); // START
    I2C_Write(DS3232_WRITE);           // 
    I2C_Write(0x00);// Start at SECONDS register

    I2C_Write(sec);  // sec 
    I2C_Write(min); // min
    I2C_Write(hour);// Hours(24-hour mode)
    I2C_Write(day);
     I2C_Write(date);
     I2C_Write(month);
      I2C_Write(year);
    I2C_Stop();// STOP
}

    void DS3232_SetTime_Only(uint8_t sec,uint8_t min,uint8_t hour,uint8_t day,uint8_t date,uint8_t month,uint8_t year){
    I2C_Start();              
    I2C_Write(DS3232_WRITE); 
    I2C_Write(0x00); // start at SECONDS register

    I2C_Write(sec_BCD(sec)); // sec
    I2C_Write(min_BCD(min)); // min 
    I2C_Write(hour_BCD(hour));//hours(24-hour mode)
    I2C_Write(day_BCD(day));
     I2C_Write(date_BCD(date));
     I2C_Write(month_BCD(month));
      I2C_Write(year_BCD(year));
    I2C_Stop();                
}
        void DS3232_ReadTime(uint8_t *sec,uint8_t *min,uint8_t *hour,uint8_t *day,uint8_t *date,uint8_t *month,uint8_t *year)
{
    I2C_Start();
    I2C_Write(DS3232_WRITE);
    I2C_Write(0x00); // Start at seconds register
    I2C_Stop();

    I2C_Start();
    I2C_Write(DS3232_READ);

    *sec  = I2C_Read(0);
    *min  = I2C_Read(0);
    *hour = I2C_Read(0);
    *day  = I2C_Read(0);
    *date = I2C_Read(0);
    *month = I2C_Read(0);
    *year = I2C_Read(1);//NACK on last byte
    I2C_Stop();
}
   void alarm1(){//This function sets the alarm every match in seconds with (00).
    I2C_Start(); // START
    I2C_Write(DS3232_WRITE);  //Write
    I2C_Write(0x0E);
    I2C_Write(0x05);//enable alarm1  INTERRUPT  A1IE=1 &INTCN=1;
    I2C_Stop();
    I2C_Start();
    I2C_Write(DS3232_WRITE);
    I2C_Write(0x07);
    I2C_Write(0x00);
    I2C_Write(0x80);
    I2C_Write(0x80);
    I2C_Write(0x80);
    I2C_Write(0x80);
    I2C_Stop();
     }  
  void alarm_flag_clear(){
     I2C_Start(); 
     I2C_Write(DS3232_WRITE);
     I2C_Write(0x0F);
     I2C_Write(0x00);
     I2C_Stop();
     } 
  uint8_t change(uint8_t row, uint8_t column,uint8_t parameter,uint8_t *i){
      
    while(1){
        while(PORTCbits.RC0==1){
            __delay_ms(200);//Delay to slow down incrementation.
            parameter++;
            if(*i == 1 && parameter > 23)               // If hours > 23 ==> hours = 0
              parameter = 0;
            if(*i == 2 && parameter > 59)               // If minutes > 59 ==> minutes = 0
              parameter = 0;
            if(*i == 3 && parameter > 31)               // If date > 31 ==> date = 1
              parameter = 1;
            if(*i == 4 && parameter > 12)               // If month > 12 ==> month = 1
              parameter = 1;
            if(*i == 5 && parameter > 99)               // If year > 99 ==> year = 0
              parameter = 0;   
            LCD_cursor_set(row,column);//position of the parameter in LCD
            LCD_write_variable(parameter,2);     
            }
        
        LCD_cursor_set(row,column);
        __delay_ms(100);//Delay for blinking 
        LCD_write_string("  ");
        LCD_cursor_set(row,column);
        __delay_ms(100);//Delay for blinking 
        LCD_write_variable(parameter,2);
            
        if(!PORTCbits.RC1)
                continue; 
        else{//if RD2 pressed return the value and increment the change position 
            (*i)++;
            __delay_ms(250);//delay to slow down incrementation.
            return parameter;
            }    
  }
            
  }
  


