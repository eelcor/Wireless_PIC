/* 
 * File:   nrflib.h
 * Author: Eelco
 *
 * Created on 30 december 2012, 21:29
 */

#ifndef NRFLIB_H
#define	NRFLIB_H

#ifdef	__cplusplus
extern "C" {
#endif

/*********************************************************************
 *
 *       _   _  ___    ___      __    _  _    _       __      _     _
 *      ( ) ( )|  _`\ (  _`\  /'__`\ ( )( )  ( )    /' _`\  /' )   ( )
 *      | `\| || (_) )| (_(_)(_)  ) )| || |  | |    | ( ) |(_, |   | |__
 *      | , ` || ,  / |  _)     /' / | || |_ | |  _ | | | |  | |   |  _ `\
 *      | |`\ || |\ \ | |     /' /( )(__ ,__)| |_( )| (_) |  | | _ | | | |
 *      (_) (_)(_) (_)(_)    (_____/'   (_)  (____/'`\___/'  (_)(_)(_) (_)
 *
 *
 *
 *
 *
 *
 *********************************************************************
 * FileName:        nrf24l01.c
 * Dependencies:
 *
 *
 * Processor:       PIC32
 *
 * Complier:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.

 The user in the program must define

        #define RADIO_CE PORTCbits.RC1
        #define CSN PORTCbits.RC2
        #define IRQ PORTCbits.RC0

 */

        #include <p32xxxx.h>
        #ifndef _P32XXXX_H
        #define _P32XXXX_H
        #endif

        #ifdef __18F2420_H
        #include <p18f2420.h>
        #include <delays.h>

        #define RADIO_CE PORTCbits.RC1
        #define CSN PORTCbits.RC2
        #define IRQ PORTCbits.RC0
        #endif

//#ifdef _P32XXXX_H
        #include <p32xxxx.h>
        #define Delay100TCYx    loopdelay
        #define Delay1TCY()             loopdelay(1)
        #define RADIO_CE PORTBbits.RB15
        #define CSN PORTBbits.RB10
        #define IRQ PORTBbits.RB13
//#endif



 //Radio Defines
#define R_REGISTER 0x00
#define W_REGISTER 0x20
#define R_RX_PAYLOAD 0x61
#define W_TX_PAYLOAD 0xA0
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
#define REUSE_TX_PL 0xE3
#define ACTIVATE 0x50
#define R_RX_PL_WID 0x60
#define W_ACK_PAYLOAD 0xA8
#define W_TX_PAYLOAD_NOACK 0xB0
#define NOP 0xFF

//Radio Registers
#define CONFIG 0x00
#define EN_AA 0x01
#define EN_RXADDR 0x02
#define SETUP_AW 0x03
#define SETUP_RETR 0x04
#define RF_CH 0x05
#define RF_SETUP 0x06
#define STATUS 0x07
#define OBSERVE_TX 0x08
#define CD 0x09
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17
#define DYNPD 0x1C
#define FEATURE 0x1D
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F

//CONFIG Register
#define MASK_RX_DR (1<<6)
#define MASK_TX_DS (1<<5)
#define MASK_MAX_RT (1<<4)
#define EN_CRC (1<<3)
#define CRCO (1<<2)
#define PWR_UP (1<<1)
#define PRIM_RX (1)

//Shockburst AutoAcknowledge
#define ENAA_P5 (1<<5)
#define ENAA_P4 (1<<4)
#define ENAA_P3 (1<<3)
#define ENAA_P2 (1<<2)
#define ENAA_P1 (1<<1)
#define ENAA_P0 (1<<0)

//Enable RX address
#define ERX_P5 (1<<5)
#define ERX_P4 (1<<4)
#define ERX_P3 (1<<3)
#define ERX_P2 (1<<2)
#define ERX_P1 (1<<1)
#define ERX_P0 (1<<0)

//Setup Automatic Retransmission
//
#define wait750 (0x02<<4)
#define reTran15 (0x0F)
//RF channel
//?

//RF Settup
#define PLL_LOCK (1<<4)
#define RF_DR_1MB (0<<3)
#define RF_DR_2MB (1<<3)
#define RF_PWR_0DB      (0x3<<1)
#define RF_PWR_N6DB     (0x02<<1)
#define RF_PWR_N12DB    (0x01<<1)
#define RF_PWR_N18DB    (0x00<1)
#define LNA_HCURR               (1)


//Address Width
#define AW_3BYTES 0x01
#define AW_4BYTES 0x02
#define AW_5BYTES 0x03

//Dynamic Payload Stuff
#define DPL_P5 (1<<5)
#define DPL_P4 (1<<4)
#define DPL_P3 (1<<3)
#define DPL_P2 (1<<2)
#define DPL_P1 (1<<1)

//Feature
#define EN_DPL (1<<2)
#define EN_ACK_PAY (1<<1)
#define EN_DYN_ACK (1)

#define PIPE0 0x01
#define PIPE1 0x02
#define PIPE2 0x04
#define PIPE3 0x08
#define PIPE4 0x10
#define PIPE5 0x20

#define SHOCKBURSTMODE 0x01
#define NORMALMODE      0x00

//char StatusReg;


//Prototypes
void setTransmitter(void);
void setReceiver(void);
void readPayload(char *data,int length);
void setPayload(char *data,int length);
void transmitData(char *data, int length);
void writeRadio(char regWrite,char *data,int numBytes);
void writeRadioByte(char regWrite,char data);
void readRadio(char regRead,char *response,int numBytes);
void sendRadio(char command,char *data,char *response,int lenSend,int lenRecive);
void readRadio(char regRead,char *response,int numBytes);
void writeRadio(char regWrite,char *data,int numBytes);
void writeRadioByte(char regWrite,char data);
void updateStatus(char value);
char getStatus();
void sendSPIChars(char *c,char *response,int lenSend, int lenRecive);
void setTXAddress(char txAddr[5]);
void setRXAddress(char pipe,char rxAddr[5]);
void setPayloadSize(char pipeWidth,int size);
char readPaloadSize(char pipe);
void setShockBurst(char pipes);
void setRegular(void);
void initRadio(void);
char isInShockburstMode(void);
void loopdelay(unsigned long limit);
void sendSPIChars(char *c,char *response,int lenSend, int lenRecive);
void setTransmitterWithDSInt(void);
void clearDSInt(void);

#ifdef	__cplusplus
}
#endif

#endif	/* NRFLIB_H */

