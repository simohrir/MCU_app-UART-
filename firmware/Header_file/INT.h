void INT_config(){
     TRISB=0X02;//unput 
    INTCONbits.GIE=1;
    INTCON3bits.INT2IE=1;//ENABLE INT1
    INTCON3bits.INT2IF=0;
    INTCON2|=0X10;//rasing edge
}

void IOC(){
    TRISB|=0x50;//input 
    INTCONbits.RBIE=1;//ENABLE RBI
    INTCONbits.RBIF=0;
}
