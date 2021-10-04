/* 
 * File: ESP8266    
 * Author: Nestor
 * Comments:
 * Revision history: 
 */
  
#ifndef ESP8266_H
#define	ESP8266_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define RESET_CMD               "AT+RST\r\n"
#define AT_TEST                 "AT\r\n"
#define AT_RESPONSE             "\r\nOK\r\n"
#define AT_RESPONSE_FOR_RESET   "\r\nOK\r\n"
#define WIFI_GOT_IP             "WIFI CONNECTED\r\nWIFI GOT IP"
#define AT_ECHO_ON              "ATE1\r\n"
#define AT_ECHO_OFF             "ATE0\r\n"
#define AT_RESTORE              "AT+RESTORE\r\n"
#define AT_CWMODE_CUR_STA       "AT+CWMODE=1\r\n"
#define AT_CWMODE_CUR_SOFT      "AT+CWMODE=2\r\n"
#define AT_CWMODE_CUR_SOFT_STA  "AT+CWMODE=3\r\n"
#define AT_CWJAP_CUR            "AT+CWJAP=\"Nestor_Wifi\",\"K0lela@9403176274188\"\r\n"
#define AT_CWQAP                "\r\nAT+CWQAP\r\n"
#define AT_CIFSR                "AT+CIFSR\r\n"
#define AT_CIPSERVER            "AT+CIPSERVER=1,80\r\n"
#define AT_SINGLE_CONNECTION    "AT+CIPMUX=0\r\n"
#define AT_MULTIPLE_CONNECTION  "AT+CIPMUX=1\r\n"


void ESP8266_Init(void);
void ESP8266_Reset(void);
bool ESP8266_Test(void);
void ESP8266_ECHO(uint8_t flag);
void ESP8266_Restore(void);
void ESP8266_clear(const uint8_t positon);
bool ESP8266_SelectMode(const uint8_t mode); //select mode
bool ESP8266_ConnectToWIFI(void); //connect to wifi
void ESP8266_DisconnectToWIFI(void);
bool ESP8266_GetIP(unsigned char * data);
bool ESP8266_StartServer(void);
bool ESP8266_SetNumberOfConnection(const uint8_t flag);
void Clear_Buffer(void);
void Send_Server(unsigned char * ptr, size_t link_ID); //send server
void Clean_buffer(void);
extern void Enable_Interrupt(void);
extern void Disable_Interrupt(void);
extern void Disable_WatchDog(void); //disable watch dog timer
extern void Enable_WatchDog(void); //enable watch dog timer
void server_Handle(void);
#endif	/* XC_HEADER_TEMPLATE_H */

