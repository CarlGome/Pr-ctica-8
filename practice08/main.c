#include "device_config.h"

#define _XTAL_FREQ 8000000

#define LCD_DATA_R          PORTD
#define LCD_DATA_W          LATD
#define LCD_DATA_DIR        TRISD
#define LCD_RS              LATCbits.LATC4
#define LCD_RS_DIR          TRISCbits.TRISC4
#define LCD_RW              LATCbits.LATC1
#define LCD_RW_DIR          TRISCbits.TRISC1
#define LCD_E               LATCbits.LATC0
#define LCD_E_DIR           TRISCbits.TRISC0

void send2LCD(char);
void LCD_rdy(void);
void LCD_init(void);
void LCD_cmd(char);
void portsInit(void);
    
void main(void) {
    while(1){
    // Variables que guardarán el valor de los pariodos para calcular la frecuencia
    unsigned int period1 = 0; 
    unsigned int period2 = 0;
    unsigned int total_period = 0;
    unsigned int frequency = 0;
    portsInit();
    
    CCP1CON = 0X05; // Capturar cada transición positiva
    CCPTMRS  = 0X00; //Asignar Timer 1 a CCP1
    //TIMER 1
    T1CONbits.TMR1CS = 0b00; //Fosc/4
    T1CONbits.T1CKPS = 0b01; // Prescala de 2
    T1CONbits.T1SOSCEN = 0b0; // Desactivar osciladores secundarios
    T1CONbits.T1SYNC = 0b1; // Enlace con reloj externo
    T1GCONbits.TMR1GE = 0;
    T1CONbits.TMR1ON = 1;
    
    while (PIR1bits.CCP1IF ==0);//Esperar por la primera transición
    period1 = CCPR1; // Cargar el valor de inicio a period1
    PIR1bits.CCP1IF = 0; // Apagar la bandera
    
    while (PIR1bits.CCP1IF ==0); //Esperar por la segunda transición
    period2 = CCPR1; //Cargar el valor final a period2
    T1CONbits.TMR1ON = 0; // Apagar TIMER 1
    
    total_period = period2 - period1;
    // Frecuencia = Fosc/(8*total_period)
    // Frecuencia = 500,000/(8*total_period)
    // Frecuencia = 62,500/total_period)
    frequency = 62500/total_period;
    LCD_DATA_DIR = 0x00;
    
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_E  = 0;

    LCD_init();

    send2LCD('F');
    send2LCD('R');
    send2LCD('E');
    send2LCD('Q');
    send2LCD('U');
    send2LCD('E');
    send2LCD('N');
    send2LCD('C');
    send2LCD('Y');
    send2LCD(':');
    LCD_cmd(0x02);  // Movemos el cursor a la fila 2 del lcd        
    __delay_ms(250);
    // Cascada de if´s que nos permitirá escribir la frecuencia leída en el lcd.
    // Los números 202, 192, 182, etc. son restas que se le hacen al valor de la frecuencia para ser capaces de 
    // poder escribir los números del 0 al 9 en el display.
    if (frequency>255){
        send2LCD('2'); send2LCD('5'); send2LCD('5');}
    if (frequency>=250&& frequency <=255){
        frequency = frequency-202; send2LCD('2'); send2LCD('5'); send2LCD(frequency);}
    if (frequency>=240&& frequency <=249){
        frequency = frequency-192; send2LCD('2'); send2LCD('4'); send2LCD(frequency);}
    if (frequency>=230&& frequency <=239){
        frequency = frequency-182; send2LCD('2'); send2LCD('3'); send2LCD(frequency);}
    if (frequency>=220&& frequency <=229){
        frequency = frequency-172; send2LCD('2'); send2LCD('2'); send2LCD(frequency);}
    if (frequency>=210&& frequency <=219){
        frequency = frequency-162; send2LCD('2'); send2LCD('1'); send2LCD(frequency);}
    if (frequency>=200&& frequency <=209){
        frequency = frequency-152; send2LCD('2'); send2LCD('0'); send2LCD(frequency);}
    if (frequency>=190&& frequency <=199){
        frequency = frequency-142; send2LCD('1'); send2LCD('9'); send2LCD(frequency);}
    if (frequency>=180&& frequency <=189){
        frequency = frequency-132; send2LCD('1'); send2LCD('8'); send2LCD(frequency);}
    if (frequency>=170&& frequency <=179){
        frequency = frequency-122; send2LCD('1'); send2LCD('7'); send2LCD(frequency);}
    if (frequency>=160&& frequency <=169){
        frequency = frequency-112; send2LCD('1'); send2LCD('6'); send2LCD(frequency);}
    if (frequency>=150&& frequency <=159){
        frequency = frequency-102; send2LCD('1'); send2LCD('5'); send2LCD(frequency);}
    if (frequency>=140&& frequency <=149){
        frequency = frequency-92; send2LCD('1'); send2LCD('4'); send2LCD(frequency);}
    if (frequency>=130&& frequency <=139){
        frequency = frequency-82; send2LCD('1'); send2LCD('3'); send2LCD(frequency);}
    if (frequency>=120&& frequency <=129){
        frequency = frequency-72; send2LCD('1'); send2LCD('2'); send2LCD(frequency);}
    if (frequency>=110&& frequency <=119){
        frequency = frequency-62; send2LCD('1'); send2LCD('1'); send2LCD(frequency);}
    if (frequency>=100&& frequency <=109){
        frequency = frequency-52; send2LCD('1'); send2LCD('0'); send2LCD(frequency);}
    if (frequency>=90&& frequency <=99){
        frequency = frequency-42; send2LCD('0'); send2LCD('9'); send2LCD(frequency);}
    if (frequency>=80&& frequency <=89){
        frequency = frequency-32; send2LCD('0'); send2LCD('8'); send2LCD(frequency);}
    if (frequency>=70&& frequency <=79){
        frequency = frequency-22; send2LCD('0'); send2LCD('7'); send2LCD(frequency);}
    if (frequency>=60&& frequency <=69){
        frequency = frequency-12; send2LCD('0'); send2LCD('6'); send2LCD(frequency);}
    if (frequency>=50&& frequency <=59){
        frequency = frequency-2; send2LCD('0'); send2LCD('5'); send2LCD(frequency);}
    if (frequency>=40&& frequency <=49){
        frequency = frequency+8; send2LCD('0'); send2LCD('4'); send2LCD(frequency);}
    if (frequency>=30&& frequency <=39){
        frequency = frequency+18; send2LCD('0'); send2LCD('3'); send2LCD(frequency);}
    if (frequency>=20&& frequency <=29){
        frequency = frequency+28; send2LCD('0'); send2LCD('2'); send2LCD(frequency);}
    if (frequency>=10&& frequency <=19){
        frequency = frequency+38; send2LCD('0'); send2LCD('1'); send2LCD(frequency);}
    if (frequency>=0&& frequency <=9){
        frequency = frequency+48; send2LCD('0'); send2LCD('0'); send2LCD(frequency);}
    
        
}
}

void send2LCD(char xy){
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_E  = 1;
    LCD_DATA_W = xy;
    Nop();
    Nop();
    LCD_E  = 0;
    __delay_ms(250);
}

void LCD_init(void){
    LATC = 0;               // Make sure LCD control port is low
    LCD_E_DIR = 0;          // Set Enable as output
    LCD_RS_DIR = 0;         // Set RS as output 
    LCD_RW_DIR = 0;         // Set R/W as output
    LCD_cmd(0x38);          // Display to 2x16
    __delay_ms(250);  
    LCD_cmd(0x0F);          // Display on, cursor on and blinking
    __delay_ms(250);  
    LCD_cmd(0x01);          // Clear display and move cursor home
    __delay_ms(250);  
}

void LCD_rdy(void){
    char test;
    // configure LCD data bus for input
    LCD_DATA_DIR = 0xFF;
    test = 0x80;
    while(test){
        LCD_RS = 0;         // select IR register
        LCD_RW = 1;         // set READ mode
        LCD_E  = 1;         // setup to clock data
        test = LCD_DATA_R;
        Nop();
        LCD_E = 0;          // complete the READ cycle
        test &= 0x80;       // check BUSY FLAG 
    }
}

void LCD_cmd(char cx) {
   // LCD_rdy();              // wait until LCD is ready
    LCD_RS = 0;             // select IR register
    LCD_RW = 0;             // set WRITE mode
    LCD_E  = 1;             // set to clock data
    Nop();
    LCD_DATA_W = cx;        // send out command
    Nop();                  // No operation (small delay to lengthen E pulse)
    LCD_E = 0;              // complete external write cycle
}

void portsInit(void){
    OSCCON = 0b00101100;// Set the internal oscillator to 500KHz and stable
    ANSELC = 0;   // Set port D as Digital for 7 segment anodes
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 1; //RC2 como entrada para CCP1
    TRISCbits.TRISC4 = 0;
    ANSELD = 0;   // Set port D as Digital for 7 segment anodes
    TRISD  = 0x00;      // for Port D, set all pins as outputs for 7 segment anodes
}