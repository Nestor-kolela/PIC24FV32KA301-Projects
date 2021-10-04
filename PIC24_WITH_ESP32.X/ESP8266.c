/*
 * File:   ESP8266.c
 * Author: Nestor
 *
 * Created on November 8, 2020, 7:43 PM
 */

#include <xc.h>
#include "ESP8266.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcc_generated_files/uart2.h"
#include "mcc_generated_files/uart1.h"
#include <stdbool.h>

#define    FCY    16000000UL    // Instruction cycle frequency, Hz - required for __delayXXX() to work
#include <libpic30.h>
#define DelayMs()               __delay_ms(100)


#if 0
const char * htmlPtr = "<!DOCTYPE html> <html>\n<head><meta name=\"viewport\"\
		content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n\
		<title>LED CONTROL</title>\n<style>html { font-family: Helvetica; \
		display: inline-block; margin: 0px auto; text-align: center;}\n\
		body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\
		h3 {color: #444444;margin-bottom: 50px;}\n.button {display: block;\
		width: 80px;background-color: #1abc9c;border: none;color: white;\
		padding: 13px 30px;text-decoration: none;font-size: 25px;\
		margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n\
		.button-on {background-color: #1abc9c;}\n.button-on:active \
		{background-color: #16a085;}\n.button-off {background-color: #34495e;}\n\
		.button-off:active {background-color: #2c3e50;}\np {font-size: 14px;color: #888;margin-bottom: 10px;}\n\
		</style>\n</head>\n<body>\n<h1>ESP8266 LED CONTROL</h1>\n";
#endif
/*
const char * EISH = "<html><head><title>Nestor IOT DEVELOPMENT</title>\
            </head><h1>IOT Web Development</h1><p><h2>Hello Nestor \
            Kolela Kalambay</h2></p><body></body></html>";
 *
 */

extern unsigned char myIP[17];

extern void UART2_puts(char * ptr);
extern void UART2_putrs(const char * ptr);

extern void UART1_puts(char * ptr);
extern void UART1_putrs(const char * ptr);

const char * KOLELA = "<html><head><title>Remote Control With Push Button</title></head><body><form name=\"input\" method=\"get\"><table align=\"center\" width=\"500\" bgcolor =\"LightGray\" border=\"2\"><tr><td align=\"center\" colspan=\"2\"><font size=\"5\" color=\"RoyalBlue\" face=\"verdana\"><b>Web-Based Control and Monitoring</br></font></td></tr><tr><td height=\"200\" align=\"center\" bgcolor=\"LightGray\"><input name=\"TA\" type=\"submit\" value=\"TOGGLE RELAY A\"/></td></tr></table></form></body></html>";
extern volatile uint8_t flag;

void ESP8266_Init(void) {
    ESP8266_ECHO(0);

    if (ESP8266_Test() == true) {
        if (ESP8266_SelectMode(1) == true) {
            if (ESP8266_ConnectToWIFI() == true) {

                if (ESP8266_GetIP(myIP) == true) {

                    if (ESP8266_SetNumberOfConnection(1) == true) {

                        if (ESP8266_StartServer() == true) {

                        } else {

                        }
                    } else {

                    }
                    //Then do the next thing
                } else {

                }

            } else {
                //return from function

            }
        } else {

        }
    } else {
        //flag it as an error
    }

    //ESP8266_StartServer(); //start server
}

void ESP8266_Reset(void) {

    UART2_putrs(RESET_CMD);
}

bool ESP8266_Test(void) {
    DelayMs();
    ESP8266_ECHO(0);
    uint8_t cnt = 0;
    bool status = false;
    uint8_t tempVar[20];
    do {
        Clear_Buffer(); // read all the bytes
        UART2_putrs(AT_TEST);
        UART1_putrs(AT_TEST);
        DelayMs();
        uint8_t i = 0;
        while ((UART2_ReceiveBufferIsEmpty() == false) && i < 19) {
            tempVar[i++] = (uint8_t) UART2_Read();
        }
        tempVar[i] = '\0';
        if ((strstr((const char *) tempVar, AT_RESPONSE)) != NULL) {
            status = true;
            break;
        }
    } while (++cnt < 3); //


    return status == true ? true : false;

}

void Clear_Buffer(void) {
    uint8_t data;
    while ((UART2_ReceiveBufferIsEmpty() == false)) {
        data = UART2_Read();
    }
}

void ESP8266_ECHO(const uint8_t flag) {
    if (flag == 0) {
        UART2_putrs(AT_ECHO_OFF);
        UART1_putrs(AT_ECHO_OFF);
    } else {
        UART2_putrs(AT_ECHO_ON);
        UART1_putrs(AT_ECHO_OFF);
    }
    DelayMs();
}

void ESP8266_Restore(void) {
    Clear_Buffer(); //all the buffer of all characters
    UART2_putrs(AT_RESTORE);
    UART1_putrs(AT_RESTORE);
    DelayMs();
}

bool ESP8266_SelectMode(const uint8_t mode) {
    uint8_t i = 0;

    char tempBuffer[20];

    bool status = false;
    do {
        Clear_Buffer(); //read all the bytes in the ring buffer
        if (mode == 1) {
            UART2_putrs(AT_CWMODE_CUR_STA);
            UART1_putrs(AT_CWMODE_CUR_STA);
        } else if (mode == 2) //softAP mode
        {
            UART2_putrs(AT_CWMODE_CUR_SOFT);
            UART1_putrs(AT_CWMODE_CUR_SOFT);
        } else if (mode == 3) //softAP + station mode
        {
            UART2_putrs(AT_CWMODE_CUR_SOFT_STA);
            UART1_putrs(AT_CWMODE_CUR_SOFT_STA);
        }//end if for mode==3
        DelayMs(); //wait for 100 milliseconds for a response
        uint8_t cnt = 0;
        while ((UART2_ReceiveBufferIsEmpty() == false) && cnt < 19) {
            tempBuffer[cnt++] = (uint8_t) UART2_Read(); //keep reading
        }
        tempBuffer[cnt] = '\0';
        if ((strstr((const char *) tempBuffer, AT_RESPONSE)) != NULL) {
            status = true;
            break;
        }
    } while (++i < 3);
    return status == true ? true : false;
}//end function

bool ESP8266_ConnectToWIFI(void) {
    // DelayMs();
    //ESP8266_DisconnectToWIFI();
    uint8_t i = 0;
    bool status = false;
    char buffer[50];
    do {
        Clear_Buffer();
        UART2_putrs(AT_CWJAP_CUR);
        UART1_putrs(AT_CWJAP_CUR);
        //Wait for 5 seconds
        uint8_t i = 0;
        for (i = 0; i < 50; i++) {
            DelayMs();
        }
        uint8_t cnt = 0;
        while ((UART2_ReceiveBufferIsEmpty() == false) && cnt < 59) {
            buffer[cnt++] = (uint8_t) UART2_Read();
        }

        buffer[cnt] = '\0';
        cnt = 0;
        if ((strstr((const char *) buffer, WIFI_GOT_IP)) != NULL) {
            status = true;
            break;
        }
    } while (++i < 3);

    return status == true ? true : false;
}

void ESP8266_DisconnectToWIFI(void) {

    Clear_Buffer();
    UART2_putrs(AT_CWQAP);
    UART1_putrs(AT_CWQAP);
    DelayMs();
    DelayMs();
}

bool ESP8266_GetIP(unsigned char * data) {

    unsigned char buffer[40] = {0}; //make a buffer

    DelayMs();
    DelayMs();
    uint8_t cnt = 0;

    bool status = false;
    do {
        Clear_Buffer(); //read all pending data that we are not interested in
        UART2_putrs(AT_CIFSR); //send the command to get the IP
        UART1_putrs(AT_CIFSR); //send the command to get the IP
        uint8_t tempVar;
        for (tempVar = 30; tempVar > 0; tempVar--) {
            __delay_ms(100); //wait for 3 seconds brother
        }

        uint8_t i = 0;
        while ((UART2_ReceiveBufferIsEmpty() == false) && (i < 39)) {
            buffer[i++] = (uint8_t) UART2_Read();
        }

        buffer[i] = '\0'; //LOL

        if ((strstr((const char *) buffer, "192.168.")) != NULL) {
            unsigned char * ptr = (unsigned char *) strstr((const char *) (buffer), "192.168.");
            //remember to send the flag to true
            uint8_t var = 0;

            while ((*(ptr + var) != '\"') && (var < 16)) {
                *(myIP + var) = *(ptr + var);
                var++;
            }
            *(myIP + var) = '\0';
            status = true;
            break;
        }
    } while (++cnt < 3);

    return status == true ? true : false;
}

bool ESP8266_SetNumberOfConnection(const uint8_t flag) {

    uint8_t cnt = 3;

    uint8_t buffer[20];
    //DelayMs();

    bool status = false;
    do {

        Clear_Buffer(); //clear this buffer
        if (flag == 0) {
            //single connection
            UART2_putrs(AT_SINGLE_CONNECTION);
            UART1_putrs(AT_SINGLE_CONNECTION);
        } else if (flag == 1) {
            UART2_puts(AT_MULTIPLE_CONNECTION);
            UART1_puts(AT_MULTIPLE_CONNECTION);
        }//multiple connection

        DelayMs();

        uint8_t i = 0;

        while ((UART2_ReceiveBufferIsEmpty() == false) && (i < 19)) {
            buffer[i++] = (uint8_t) UART2_Read(); //read the data
        }

        buffer[i] = '\0'; //put a null terminator here

        if (strstr((const char *) buffer, AT_RESPONSE) != NULL) {
            status = true;
            break;
        }
    } while (--cnt > 0);

    return status == true ? true : false;
}

bool ESP8266_StartServer(void) {
    uint8_t i = 3;
    //DelayMs(); //wait for 100 milliseconds
    bool status = false;
    uint8_t buffer[20];
    do {
        Clear_Buffer(); //clear this buffer
        UART2_putrs(AT_CIPSERVER);
        UART1_putrs(AT_CIPSERVER);
        DelayMs();
        uint8_t cnt = 0;

        while ((UART2_ReceiveBufferIsEmpty() == false) && (cnt < 19)) {
            buffer[cnt++] = (uint8_t) UART2_Read(); //read the data
        }
        buffer[cnt] = '\0'; //put a null terminator here


        if (strstr((const char *) buffer, AT_RESPONSE) != NULL) {
            status = true;
            break;
        }

    } while (--i > 0);
    return status == true ? true : false;
}

void Send_Server(unsigned char * ptr, size_t link_ID) {
    size_t len = strlen((char *) ptr); //read the length of the string
    unsigned char data[20];
    sprintf((char *) data, "AT+CIPSEND=%u,%u\r\n", link_ID, len);
    char buffer[100];

    Clear_Buffer();
    
    UART2_putrs((char *) data);
    UART1_putrs((char *) data); 
    DelayMs(); //wait for this long
    DelayMs();
    uint8_t tempVar = 0;
    while ((UART2_ReceiveBufferIsEmpty() == false) && tempVar < 99) 
    {
        buffer[tempVar++] = (uint8_t) UART2_Read(); //
    }
    uint8_t nestor = 0;
    buffer[tempVar] = '\0'; // this is fine
    if (strstr((const char *) buffer, ">") != NULL) {
        Clear_Buffer();
        UART2_putrs((char *) ptr); //send THIS
        UART1_putrs((char *) ptr); 
        DelayMs(); //wait for this long
        DelayMs();
        uint8_t tempVar = 0;
        
        while ((UART2_ReceiveBufferIsEmpty() == false) && tempVar < 39) {
            buffer[tempVar++] = (uint8_t) UART2_Read(); //
        }
        buffer[tempVar] = '\0'; // this is fine
        if (strstr((const char *) buffer, "SEND OK") != NULL) {

            sprintf((char *) data, "AT+CIPCLOSE=5\r\n");
            Clear_Buffer(); //clear the buffer
            UART2_putrs((char *) data); //send this string to close connection
            DelayMs();
            DelayMs();
        }
    }
}//send server

void server_HandleTwo(void)
{
    if(UART2_ReceiveBufferIsEmpty() == false)
    {
        uint8_t tempVar = UART2_Read(); // check 
        if(tempVar == '+')
        {
            unsigned char cnt; 
            char buf[4]; 
            for(cnt = 0; cnt < 3; cnt++)
            {
                buf[cnt] = UART2_Read(); 
            }
            buf[3]= '\0'; //append a null terminator
            if(buf[0] == 'I' && buf[1] == 'P' && buf[2] == 'D')
            {
                /*
                   size_t link_ID = 0;
                   unsigned char * ptr = NULL; //point to nothing
                   ptr = (unsigned char *) strstr((const char *) buf, "+IPD");
                   link_ID = *(ptr + 5);
                   link_ID -= 0x30; //take this out to get the number in not ASCII
                   Send_Server((unsigned char *) KOLELA, link_ID); //
                   DelayMs();*/
                DelayMs();
                DelayMs();
                uint8_t link_ID; 
                //unsigned char * ptr = NULL: 
                tempVar = UART2_Read();  // read the ','
                link_ID = UART2_Read(); 
                link_ID -= 0x30; 
                Send_Server((unsigned char * ) KOLELA, link_ID);
                DelayMs(); 
            }
        }else
        {
            return; 
        }
    }
}

