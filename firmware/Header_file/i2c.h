void I2C_Init(void) {
    TRISBbits.TRISB0 = 1;//SCL
    TRISBbits.TRISB1 = 1;//SDA
    SSPSTAT = 0x80;//slew rate off
    SSPCON1 = 0x28;// I2C Master mode SSPEN=1
    //100 kHz for 4 MHz
    SSPADD = 9;
}
void I2C_Wait(void) {
    while ((SSPCON2 & 0x1F) || SSPSTATbits.R_nW);//wait until
}

void I2C_Start(void) {
    I2C_Wait();
    SSPCON2bits.SEN = 1;
}
void I2C_Stop(void) {
    I2C_Wait();
    SSPCON2bits.PEN = 1;
}
void I2C_Write(unsigned char data) {
    I2C_Wait();
    SSPBUF = data;
}
unsigned char I2C_Read(unsigned char ack)
{
    unsigned char data;
    I2C_Wait();
    SSPCON2bits.RCEN = 1;
    I2C_Wait();
    data = SSPBUF;

    I2C_Wait();
    SSPCON2bits.ACKDT = ack;//0= ACK  1= NACK
    SSPCON2bits.ACKEN = 1;

    return data;
}