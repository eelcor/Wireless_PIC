/* 
 * File:   nrflib.c
 * Author: Eelco
 *
 * Created on 30 december 2012, 21:25
 */

#include <stdio.h>
#include <stdlib.h>
#include <plib.h>           /* Include to use PIC32 peripheral libraries      */
#include "nrflib.h"

/*
 * 
 */




char sendSPI(char c)
{
        char k;
        WriteSPI1(c);
        while(!DataRdySPI1());
        k=ReadSPI1();
        return(k);
}

//Radio commands
void sendSPIChars(char *c,char *response,int lenSend, int lenRecive);
char NULLVAl[32];
char StatusReg;
char TxAddress[6][5]={{0xE7,0xE7,0xE7,0xE7,0xE7},
                                        {0xC2,0xC2,0xC2,0xC2,0xC2},
                                        {0xC3,0xC3,0xC3,0xC3,0xC3},
                                        {0xC4,0xC4,0xC4,0xC4,0xC4},
                                        {0xC5,0xC5,0xC5,0xC5,0xC5},
                                        {0xC6,0xC6,0xC6,0xC6,0xC6}};

char RxAddress[5]={0xE7,0xE7,0xE7,0xE7,0xE7};
char isInShockBurstMode=0;

void initRadio(void)
{
        writeRadioByte(CONFIG, MASK_TX_DS | MASK_MAX_RT | EN_CRC | PWR_UP | PRIM_RX );//PWR_UP = 1
        writeRadioByte(SETUP_AW,AW_5BYTES);//address width = 5 bytes
        writeRadioByte(RF_SETUP, RF_DR_1MB | RF_PWR_0DB | LNA_HCURR);//data rate = 1MB, power 0db
        writeRadioByte(RF_CH,0x02);//set channel 2, this is default but we did it anyway...
        writeRadio(RX_ADDR_P0,TxAddress[0],5);
        writeRadioByte(EN_RXADDR,0x03);//Enable address 0 and 1

        //default to be changed afterword
        writeRadioByte(RX_PW_P0,0x04);//4 byte payload
        writeRadioByte(RX_PW_P1,0x04);//4 byte payload
        writeRadioByte(EN_AA,0x00);
        writeRadio(RX_ADDR_P1,RxAddress,5);
        writeRadio(RX_ADDR_P0,RxAddress,5);
}

void setTransmitter(void)
{
        char temp;
        RADIO_CE=0;
        writeRadioByte(STATUS,0x70); //clear previous ints
        readRadio(CONFIG,&temp,1);
        writeRadioByte(CONFIG,(temp&0xFE)| EN_CRC | PWR_UP);//PWR_UP = 1
}


void setReceiver(void)
{
        char temp;
        readRadio(CONFIG,&temp,1);
        writeRadioByte(CONFIG,(temp&0xFE)| EN_CRC | PWR_UP | PRIM_RX);//PWR_UP = 1
        writeRadioByte(CONFIG,0x39);//PRX, CRC enabled
        writeRadioByte(RF_SETUP,0x07);//data rate = 1MB
        writeRadioByte(RF_CH,0x02);//set channel 2
        writeRadioByte(CONFIG,0x3B);//PWR_UP = 1
        RADIO_CE=1;
}

void readPayload(char *data,int length)
{
        sendRadio(R_RX_PAYLOAD,NULLVAl,data,0,length);
        sendRadio(0xE2,NULLVAl,NULLVAl,0,0);//Flush RX FIF0
        writeRadioByte(0x07,0x40);//reset int
}


void setPayload(char *data,int length)
{
        sendRadio(W_TX_PAYLOAD,data,NULLVAl,length,0);
}

void transmitData(char *data, int length)
{
        char temp;
        writeRadioByte(STATUS,0x70); //clear previous ints
        setPayload(data,length);
        //Get config
        readRadio(CONFIG,&temp,1);
        writeRadioByte(CONFIG,(temp&0xFE) | MASK_MAX_RT | EN_CRC | PWR_UP);//PWR_UP = 1
        RADIO_CE=1;
        Delay100TCYx(11);
}


void writeRadio(char regWrite,char *data,int numBytes)
{
        char regToWrite;
        regToWrite=W_REGISTER|regWrite;
        sendRadio(regToWrite,data,NULLVAl,numBytes,0x00);
}


void writeRadioByte(char regWrite,char data)
{
        char regToWrite;
        regToWrite=W_REGISTER|regWrite;
        sendRadio(regToWrite,&data,NULLVAl,0x01,0x00);
}

void readRadio(char regRead,char *response,int numBytes)
{
        char regToRead;
        regToRead=R_REGISTER|regRead;
        sendRadio(regToRead,NULLVAl,response,0x00,numBytes);
}




void sendRadio(char command,char *data,char *response,int lenSend,int lenRecive)
{
        //Bring CSN low
        CSN=0;
        Delay1TCY();
        StatusReg=sendSPI(command);
        //Send the Data
        sendSPIChars(data,response,lenSend,lenRecive);
        //Bring CSN High
        CSN=1;
}

void updateStatus(char value)
{
        StatusReg=value;
}
char getStatus()
{
        return (StatusReg);
}

void sendSPIChars(char *c,char *response,int lenSend, int lenRecive)
{
int i,j;
        for (i=0;(i<lenSend)||(i<lenRecive);i++)
        {
                if (i<lenSend)
                {
                        if (lenRecive>i)
                        {
                                *(response + i) = sendSPI (*(c+i));
                        }else{
                                sendSPI (*(c+i));
                        }
                }else if(i<lenRecive){
                        //*(response + i)=sendSPI(0xFF);
                        response[i]=sendSPI(0xFF);
                }
        }

}

void setTXAddress(char txAddr[5])
{
        writeRadio(TX_ADDR,txAddr,5);//Set TX Address
        //Dont know why this has to be done this way
}

void setRXAddress(char pipe,char rxAddr[5])
{

        /*
        *
        *       Allowed values for pipe incude RX_ADDR_P0-RX_ADDR_P5
        *
        */
        writeRadio(pipe,rxAddr,5);
}
void setPayloadSize(char pipeWidth,int size)
{
        /*
        *
        *       Allowed values for pipe incude RX_PW_P0-RX_PW_P5
        *
        */
        writeRadioByte(pipeWidth,size);//4 byte payload
}

char readPaloadSize(char pipe)
{
        char temp;
        readRadio(pipe,&temp,1);
        return(temp);
}

void setShockBurst(char pipes)
{
/*
 *
 *                      Uses a pipes character to represenet all of the pipes
 *              e.g.
 *                              pipe 1 is the LSB,
 *                              pipe 2 is the 2nd bit etc....
 *
 *
 */
                //Enable Auto Ack on selected pipes
                writeRadioByte(EN_AA,pipes);

                //setup retransmission
                writeRadioByte(SETUP_RETR, wait750 | reTran15);

                //setup dynamic payload
                writeRadioByte(DYNPD,pipes);

                writeRadioByte(FEATURE,EN_DPL|EN_ACK_PAY);

                isInShockBurstMode=1;

}

void setRegular(void)
{
/*
 *
 *                      Uses a pipes character to represenet all of the pipes
 *              e.g.
 *                              pipe 1 is the LSB,
 *                              pipe 2 is the 2nd bit etc....
 *
 *
 */

                //Enable Auto Ack on selected pipes
                writeRadioByte(EN_AA,0x00);//dissable auto-ack for all channels

                //setup retransmission
                writeRadioByte(SETUP_RETR,0x00);//dissable auto-ack for all channels

                //setup dynamic payload
                writeRadioByte(DYNPD,0x00);

                writeRadioByte(FEATURE,0x00);

                isInShockBurstMode=0;


}
char isInShockburstMode(void)
{
        return(isInShockBurstMode);
}


void loopdelay(unsigned long limit)
{
        unsigned long time;
        for (time=0;time<limit*10;time++);
}

void setTransmitterWithDSInt(void)
{
        char temp;
        RADIO_CE=0;
        writeRadioByte(STATUS,0x70); //clear previous ints
        readRadio(CONFIG,&temp,1);
        writeRadioByte(CONFIG,(temp&0xDE)| EN_CRC | PWR_UP);//PWR_UP = 1
}

void clearDSInt(void)
{
        char temp;
        readRadio(CONFIG,&temp,1);
        writeRadioByte(CONFIG,(temp&0xFF)|MASK_TX_DS);//PWR_UP = 1
}

