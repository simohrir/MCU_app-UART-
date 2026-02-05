

#include <xc.h>
#define _XTAL_FREQ 4000000
#define PIN_OUT_IN TRISAbits.RA0
#define PIN_STATE  LATAbits.LA0     // for writing
#define PIN_READ   PORTAbits.RA0  // for reading
char check;

void start_signal(){
    PIN_OUT_IN=0;//PIN output
    PIN_STATE=0;// set value to '0'
    __delay_ms(18);
    PIN_STATE=1;//set value to' 1'
   __delay_us(30);
    PIN_OUT_IN=1;//pin input
   
}
 void check_response(char *check){
 *check=0;                                                    
     __delay_us(40);                                                      
     if(PIN_READ==0){     //                                                                               
        __delay_us(80);    //                                                                         
        if(PIN_READ==1){ //                                                                               
           *check=1;  //                                                        
           __delay_us(40);
        }
     }
}
 uint8_t DATA(){
     uint8_t data=0;
     for (int i = 0; i < 8; i++) {
    while (PIN_READ == 0); // wait for HIGH
    __delay_us(30);          // wait mid-pulse
    if (PIN_READ == 1)
            data |= (1 << (7 - i));   // Write bit = 1
   
    while (PIN_READ == 1); // wait for end of HIGH pulse
}
     return data;
 }
 void data_read(uint8_t *temp,uint8_t *humidity){
     uint8_t bits_data[5];
     start_signal();
     check_response(&check);
     if(check==1){
     for(char i=0;i<5;i++){
         bits_data[i]=DATA();    
     }}
     if(bits_data[4]==(bits_data[0]+bits_data[2]+bits_data[1]+bits_data[3])){
         *temp=bits_data[2];
         *humidity=bits_data[0];
     }
 }
