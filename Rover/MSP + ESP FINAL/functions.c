/*
 * functions.c
 *
 *  Created on: 25/01/2025
 *      Author: henri
 */

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "msp.h"

#include "functions.h"

Graphics_Context g_sContext;
//***************UART*******************************

const char *WriteDefault = "defaul";
const char *WriteForward = "forwar";
const char *WriteRight = "right.";
const char *WriteBackward = "backwa";
const char *WriteLeft = "left..";
const char *WriteCamera = "camera";
const char *WriteTank = "tank..";
const char *WriteUp = "up....";
const char *WriteDown = "down..";
const char *WriteCleft ="cleft.";
const char *WriteCright = "cright";
const char *WriteMiddle = "middle";

bool commandTank = true;
bool commandCamera = false;
bool commandForward = false;
bool commandBackwards = false;
bool commandRight = false;
bool commandLeft = false;
bool commandDefault = false;
bool commandUp = false;
bool commandDown = false;
bool commandCright = false;
bool commandCleft = false;
bool commandMiddle = false;


const eUSCI_UART_ConfigV1 uartConfig = {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // Clock Source: SMCLK  12MHz
        78,                                      // BRDIV = 78
        2,                                       // UCxBRF = 2
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No parity
        EUSCI_A_UART_LSB_FIRST,                  // Least Significante Bit first
        EUSCI_A_UART_ONE_STOP_BIT,               // 1 stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION // Oversampling. Defines de formula used to calculate de baud rate, using the first 4 parameter = 9554 (aprox 9600).
};

void UART_init(){
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
                    GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);///Pin 3.2RX,pin3.3TX    || In ESP32 16RX 17TX
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
    UART_initModule(EUSCI_A2_BASE, &uartConfig);
    UART_enableModule(EUSCI_A2_BASE);
}

void UART_SendString(const char *message) { //Needed because the UART send one character per time
    while (*message != '\0') { // Loop until null terminator is reached
        UART_transmitData(EUSCI_A2_BASE, *message); // Send the current character
        message++; // Move to the next character
    }
}

void resetCommandsTank(){
    commandForward = false;
    commandBackwards = false;
    commandRight = false;
    commandLeft = false;
    commandDefault = false;
}
void resetCommandsCamera(){
    commandUp = false;
    commandDown = false;
    commandCright = false;
    commandCleft = false;
    commandMiddle = false;
}

//strcmp compares 2 strings, avaluates if the string has already been sent. In order to not overflow the UART
void sendOnceTank(const char *message) {
        if (strcmp(message, WriteDefault) == 0) {
            if (!commandDefault) {
                resetCommandsTank();
                UART_SendString(message);
                commandDefault = true;
            }
        } else if (strcmp(message, WriteForward) == 0) {
            if (!commandForward) {
                resetCommandsTank();
                UART_SendString(message);
                commandForward = true;
            }
        } else if (strcmp(message, WriteRight) == 0) {
            if (!commandRight) {
                resetCommandsTank();
                UART_SendString(message);
                commandRight = true;
            }
        } else if (strcmp(message, WriteBackward) == 0) {
            if (!commandBackwards) {
                resetCommandsTank();
                UART_SendString(message);
                commandBackwards = true;
            }
        } else if (strcmp(message, WriteLeft) == 0) {
            if (!commandLeft) {
                resetCommandsTank();
                UART_SendString(message);
                commandLeft = true;
            }
        }
}

void sendOnceCamera(const char *message) {
        if (strcmp(message, WriteMiddle) == 0) {
            if (!commandMiddle) {
                resetCommandsCamera();
                UART_SendString(message);
                commandMiddle = true;
            }
        } else if (strcmp(message, WriteUp) == 0) {
            if (!commandUp) {
                resetCommandsCamera();
                UART_SendString(message);
                commandUp = true;
            }
        } else if (strcmp(message, WriteCright) == 0) {
            if (!commandCright) {
                resetCommandsCamera();
                UART_SendString(message);
                commandCright = true;
            }
        } else if (strcmp(message, WriteDown) == 0) {
            if (!commandDown) {
                resetCommandsCamera();
                UART_SendString(message);
                commandDown = true;
            }
        } else if (strcmp(message, WriteCleft) == 0) {
            if (!commandCleft) {
                resetCommandsCamera();
                UART_SendString(message);
                commandCleft = true;
            }
        }
}

//This is not used because we changed the messages name. Useful for the "future" to switch between modes when the programm gets more complex.
void sendAlternateMessage() {
        if (!commandCamera) {
            UART_SendString(WriteCamera);
            commandCamera = true;
            commandTank = false;
        }
     else {
        if (!commandTank) {
            UART_SendString(WriteTank);
            commandTank = true;
            commandCamera = false;
        }
    }
}


