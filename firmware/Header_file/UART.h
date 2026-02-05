void uart_init(){
    TRISCbits.RC6=1;//all so risever if needed
    TRISCbits.RC7=1;//set trasmiter bit to 1 like in the datasheet
    TXSTAbits.BRGH=1;//hight baud rate
     BAUDCONbits.BRG16 = 1; // 16-bit baud rate
     SPBRGH = 0;
    SPBRG = 103;//9600 for 4MHz
    TXSTAbits.SYNC = 0; // asynchronous
    RCSTAbits.SPEN = 1; // Enable USART
    TXSTAbits.TXEN = 1; // Enable transmit
    RCSTAbits.CREN=1; // ENABLE continuous receive
    
    //////
 PIE1bits.RCIE = 1;    // Enable UART receive interrupt
PIR1bits.RCIF = 0;    // Clear RX flag
INTCONbits.PEIE = 1;  // Peripheral interrupts
INTCONbits.GIE = 1;   // Global interrupts
    
    ////
}
void write(unsigned char data){
    while(!PIR1bits.TXIF);//we will with until we have the flag that means that the TXREG is empty and we can writh
    TXREG = data;
}
void write_string(unsigned char *tab){
    for(int i=0;tab[i]!='\0';i++){
        write(tab[i]);

    }
}
//this function sends info via UART every 30sec 
void detail_show(unsigned char sec,unsigned char min,unsigned char hour,unsigned char date,unsigned char month,unsigned char year,unsigned char temp,unsigned char hum,unsigned char *uart_String){
     sprintf(uart_String," %02u : %02u\n%u\n%u\n",BCD_dicimal(hour),BCD_dicimal(min),temp,hum);
     write_string(uart_String);
}
// This function sends only the time to know forme which time the data has been stored (the time in the window title)
void temp_show(unsigned char temp,unsigned char hum,unsigned char *uart_String){
    sprintf(uart_String,"%u\n%u\n",temp,hum);
    write_string(uart_String);
}
//this function sends the stored data of the previous 60min
void send_time_only(unsigned char min,unsigned char hour,unsigned char *uart_String){
    
sprintf(uart_String,"time: %02u : %02u\n",BCD_dicimal(hour),BCD_dicimal(min));
    write_string(uart_String);

}
