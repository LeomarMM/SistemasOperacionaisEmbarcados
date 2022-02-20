#include "io.h"
#include <xc.h>
void adc_init()
{
    ADCON1bits.VCFG = 0;
    ADCON1bits.PCFG = 0b1101;
    ADCON2bits.ADFM = 1;
    ADCON2bits.ACQT = 1;
    ADCON2bits.ADCS = 2;
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;
    ADCON0bits.ADON = 1;
}
int adc_read(int port)
{
    ADCON0bits.CHS = port;
    ADCON0bits.GODONE = 1;
    while(ADCON0bits.GODONE);
    return ADRES;
}