
//  <||| A1=A2=A0=0 |||>
#define address_write 0xA0
#define  address_read 0xA1
#define PAGE_SIZE 64 //64 bytes per page



//THIS FUNCTION STORE THE DATA IN EEPROM (24LC256)
void send_data(unsigned char sec,unsigned char min,unsigned char hour,unsigned char day,unsigned char date,unsigned char month,unsigned char year){  
     I2C_Stop();//
     I2C_Start();
    I2C_Write(address_write);
    I2C_Write(0X00);//addr high
    I2C_Write(0X00);//addr low
    I2C_Write(sec);
    I2C_Write(min);
    I2C_Write(hour);
    I2C_Write(day);
    I2C_Write(date);
    I2C_Write(month);
    I2C_Write(year);
    I2C_Stop();
}
//THIS FUNCTION TO READ THE DATA STORED
void read_data(unsigned char *sec,unsigned char *min,unsigned char *hour,unsigned char *day,unsigned char *date,unsigned char *month,unsigned char *year){
    I2C_Stop();//
    I2C_Start();
    I2C_Write(address_write);
    I2C_Write(0X00);//addr high
    I2C_Write(0X00);//addr low
    I2C_Stop();
    I2C_Start();
    I2C_Write(address_read);
    *sec=I2C_Read(0);
    *min=I2C_Read(0);
    *hour=I2C_Read(0);
    *day=I2C_Read(0);
    *date=I2C_Read(0);
    *month=I2C_Read(0);
    *year=I2C_Read(1);
    I2C_Stop();
}
void store_temp(uint8_t temp,uint8_t hum,uint8_t address,unsigned char min,unsigned char hour){
     unsigned char high_addr=0x02;
    unsigned char low_addr= address * 4;
    unsigned char page_offset= low_addr % PAGE_SIZE;
    //check if writing 3 bytes will cross the page boundary
    if (page_offset <= PAGE_SIZE - 4) {
        I2C_Start();
        I2C_Write(address_write);
        I2C_Write(high_addr);
        I2C_Write(low_addr);
        I2C_Write(min);
        I2C_Write(hour);
        I2C_Write(temp);
        I2C_Write(hum);
        I2C_Stop();
        __delay_ms(5);//Wait for EEPROM write
    } else {
        //write bytes individually
        I2C_Start();
        I2C_Write(address_write);
        I2C_Write(high_addr);
        I2C_Write(low_addr);
        I2C_Write(min);
        I2C_Stop();
        __delay_ms(5);

        I2C_Start();
        I2C_Write(address_write);
        I2C_Write(high_addr);
        I2C_Write(low_addr + 1);
        I2C_Write(hour);
        I2C_Stop();
        __delay_ms(5);

        I2C_Start();
        I2C_Write(address_write);
        I2C_Write(high_addr);
        I2C_Write(low_addr + 2);
        I2C_Write(temp);
        I2C_Stop();
        __delay_ms(5);
        
        I2C_Start();
        I2C_Write(address_write);
        I2C_Write(high_addr);
        I2C_Write(low_addr + 3);
        I2C_Write(hum);
        I2C_Stop();
        __delay_ms(5);
        
       
    }
}
void read_stored(unsigned char *min,unsigned char *hour,unsigned char *temp,unsigned char *hum,unsigned char address){
   I2C_Stop();//
   I2C_Start();
    I2C_Write(address_write);
    I2C_Write(0X02);//addr high
    I2C_Write(address*4);
    I2C_Stop();
    I2C_Start();
    I2C_Write(address_read);
    *min=I2C_Read(0);
    *hour=I2C_Read(0);
    *temp=I2C_Read(0);//NACK
    *hum=I2C_Read(1);//NACK
     I2C_Stop();

}

