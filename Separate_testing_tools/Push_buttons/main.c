/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//****************************************************************************
//
// main.c - MSP-EXP432P401R + Educational Boosterpack MkII - Joystick
//
//          Displays raw 14-bit ADC measurements for X/Y axis of Joystick
//
//****************************************************************************


/* What i need to do
 * digitalRead(pin_number) to read the state of this input pins
 *
 * configure PIN P3.5 and P5.1
 *
 *  */




#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>

/* Graphic library context */
Graphics_Context g_sContext;

/* ADC results buffer */
static uint16_t resultsBuffer[2];


int button2Toggle = 0; // Toggles for Button2
int button3Toggle = 0; // Toggles for Button3
int button2PrevState = 0; // Previous state of Button2
int button3PrevState = 0; // Previous state of Button3


void _adcInit(){
    /* Configures Pin 3.5 and 5.1 as ADC input */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

        /* Initializing ADC (ADCOSC/64/8) */
        ADC14_enableModule();
        ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

        /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
             * with internal 2.5v reference */
        ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
        ADC14_configureConversionMemory(ADC_MEM0,
                ADC_VREFPOS_AVCC_VREFNEG_VSS,
                ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

        ADC14_configureConversionMemory(ADC_MEM1,
                ADC_VREFPOS_AVCC_VREFNEG_VSS,
                ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

        /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
         *  is complete and enabling conversions */
        ADC14_enableInterrupt(ADC_INT1);

        /* Enabling Interrupts */
        Interrupt_enableInterrupt(INT_ADC14);
        Interrupt_enableMaster();

        /* Setting up the sample timer to automatically step through the sequence
         * convert.
         */
        ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

        /* Triggering the start of the sample */
        ADC14_enableConversion();
        ADC14_toggleConversionTrigger();
}

void _graphicsInit()
{
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
        Graphics_clearDisplay(&g_sContext);

        Graphics_drawStringCentered(&g_sContext,
                                        (int8_t *)"Joystick:",
                                        AUTO_STRING_LENGTH,
                                        64,
                                        30,
                                        OPAQUE_TEXT);

}


void _hwInit()
{
    /* Halting WDT and disabling master interrupts */
    WDT_A_holdTimer();
    Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    _graphicsInit();
    _adcInit();
}

/*
 * Main function
 */
int main(void)
{
    _hwInit();


    while(1)
    {
        PCM_gotoLPM0();
    }
}



/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    /* ADC_MEM1 conversion completed */
    if(status & ADC_INT1)
    {
        /* Store ADC14 conversion results */
    	resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
    	resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

          char string[10];
        sprintf(string, "X: %5d", resultsBuffer[0]);
        Graphics_drawStringCentered(&g_sContext,
                                        (int8_t *)string,
                                        8,
                                        64,
                                        50,
                                        OPAQUE_TEXT);

        sprintf(string, "Y: %5d", resultsBuffer[1]);
        Graphics_drawStringCentered(&g_sContext,
                                        (int8_t *)string,
                                        8,
                                        64,
                                        60,
                                        OPAQUE_TEXT);

        /* Determine if JoyStick button is pressed */
        int buttonPressed = 0;
        if (!(P4IN & GPIO_PIN1))
            buttonPressed = 1;

        sprintf(string, "Button1: %d", buttonPressed);
        Graphics_drawStringCentered(&g_sContext,
                                        (int8_t *)string,
                                        AUTO_STRING_LENGTH,
                                        64,
                                        80,
                                        OPAQUE_TEXT);




        // **Button2 Toggle (Top button)**
        int button2State = !(P5IN & GPIO_PIN1); // Read Button2 state (active low)
        if (button2State && !button2PrevState) // Detect rising edge
        {
            button2Toggle = !button2Toggle; // Toggle state
        }
        button2PrevState = button2State; // Update previous state

        sprintf(string, "Button2: %d", button2Toggle);
        Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)string,
                                    AUTO_STRING_LENGTH,
                                    64,
                                    90,
                                    OPAQUE_TEXT);



        // **Button3 Toggle (Bottom button)**
        int button3State = !(P3IN & GPIO_PIN5); // Read Button3 state (active low)
        if (button3State && !button3PrevState) // Detect rising edge
        {
            button3Toggle = !button3Toggle; // Toggle state
        }
        button3PrevState = button3State; // Update previous state

        sprintf(string, "Button3: %d", button3Toggle);
        Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)string,
                                    AUTO_STRING_LENGTH,
                                    64,
                                    100,
                                    OPAQUE_TEXT);




//        /* Determine if TOP button is pressed */
//                int buttonPressed_top = 0;
//                while (!(P3IN & GPIO_PIN5))
//                {
//                    buttonPressed_top = 1;
//                }
//
//                sprintf(string, "Button2: %d", buttonPressed_top);
//                Graphics_drawStringCentered(&g_sContext,
//                                                (int8_t *)string,
//                                                AUTO_STRING_LENGTH,
//                                                64,
//                                                90,
//                                                OPAQUE_TEXT);
//
//
//
//         /* Determine if BUTTOM button is pressed */
//                  int buttonPressed_buttom = 0;
//                  while((!(P5IN & GPIO_PIN1)))
//                  {
//                       buttonPressed_buttom = 1;
//                  }
//
//                  sprintf(string, "Button3: %d", buttonPressed_buttom);
//                  Graphics_drawStringCentered(&g_sContext,
//                                                 (int8_t *)string,
//                                                 AUTO_STRING_LENGTH,
//                                                 64,
//                                                 100,
//                                                 OPAQUE_TEXT);

    }

}
