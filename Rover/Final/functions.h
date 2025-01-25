/*
 * functions.h
 *
 *  Created on: 25/01/2025
 *      Author: henri
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

extern const char *WriteDefault;
extern const char *WriteForward;
extern const char *WriteRight;
extern const char *WriteBackward;
extern const char *WriteLeft;
extern const char *WriteCamera;
extern const char *WriteTank;
extern const char *WriteUp;
extern const char *WriteDown;
extern const char *WriteCleft;
extern const char *WriteCright;
extern const char *WriteMiddle;

extern bool commandTank;
extern bool commandCamera;
extern bool commandForward;
extern bool commandBackwards;
extern bool commandRight;
extern bool commandLeft;
extern bool commandDefault;
extern bool commandUp;
extern bool commandDown;
extern bool commandCright;
extern bool commandCleft;
extern bool commandMiddle;

void UART_init();
void UART_SendString(const char *message);
void resetCommandsTank();
void resetCommandsCamera();
void sendOnceTank(const char *message);
void sendOnceCamera(const char *message);
void sendAlternateMessage();
void _graphicsInit();
void _adcInit();
void _hwInit();

#endif /* FUNCTIONS_H_ */
