/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif

#include <plib.h>            /* Include to use PIC32 peripheral libraries     */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include "user.h"            /* variables/params used by user.c               */

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* TODO Initialize User Ports/Peripherals/Project here */

void InitApp(void)
{
    /* Setup analog functionality and port direction */
    ANSELB = 0x0000;
    ANSELA = 0x0000;
    // Setup SPI peripheral SDI = RPB1, SDO = RPB2, SCK = RPB14
    SDI1R = 0x2;
    RPB2R = 0x3;
    // Setup UART1 TX = RPB3, RX = RPA4
    RPB3R = 0x1;
    U1RXR = 0x2;
    /* Initialize peripherals */
    TRISBbits.TRISB11=0;
    TRISBbits.TRISB10=0;
    TRISBbits.TRISB15=0;
    TRISBbits.TRISB2=0;
    TRISBbits.TRISB1=1;
}

 #define GetSystemClock()       (8000000L)

 //*****************************************************************************
 // DelayMs creates a delay of given miliseconds using the Core Timer
 //
 void DelayMs(WORD delay)
 {
     unsigned int int_status;
     while( delay-- )
     {
         int_status = INTDisableInterrupts();
         OpenCoreTimer(GetSystemClock() / 2000);
         INTRestoreInterrupts(int_status);
         mCTClearIntFlag();
         while( !mCTGetIntFlag() );
     }
     mCTClearIntFlag();
 }
