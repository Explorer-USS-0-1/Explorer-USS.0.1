#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <string.h>




/* Graphic library context */
Graphics_Context g_sContext;

/* ADC results buffer */
static uint16_t resultsBuffer[2];


int button2Toggle = 0; // Toggles for Button2
int button3Toggle = 0; // Toggles for Button3
int button2PrevState = 0; // Previous state of Button2
int button3PrevState = 0; // Previous state of Button3

/* Position variables for car and camera */
int carPositionX = 0, carPositionY = 0;
int cameraPositionX = 0, cameraPositionY = 0;


void resetCameraPosition() {

    Graphics_drawStringCentered(&g_sContext, (int8_t *)"Camera Reset", AUTO_STRING_LENGTH, 64, 120, OPAQUE_TEXT);

}

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

}


void _hwInit(){

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN1);
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

    while(1){

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

        // Thresholds for joystick movement set limit values for movement and for default camera position
        #define THRESHOLD_HIGH 12000
        #define THRESHOLD_LOW 1000

        if(resultsBuffer[0] < THRESHOLD_HIGH - 2000 && resultsBuffer[0] > THRESHOLD_LOW + 2000 && resultsBuffer[1] < THRESHOLD_HIGH - 2000 && resultsBuffer[1] > THRESHOLD_LOW + 2000 ){
            Graphics_drawStringCentered(&g_sContext, (int8_t *)"Stationary",
                                                             AUTO_STRING_LENGTH,
                                                             64,
                                                             40,
                                                             OPAQUE_TEXT);

            Graphics_drawStringCentered(&g_sContext, (int8_t *)"  Neutral ",
                                                              AUTO_STRING_LENGTH,
                                                              64,
                                                              70,
                                                              OPAQUE_TEXT);
        }


        // **Button2 Toggle (Top button)**
        int button2State = !(P5IN & GPIO_PIN1); // Read Button2 state (active low)
        if (button2State && !button2PrevState) // Detect rising edge
        {
            button2Toggle = !button2Toggle; // Toggle state
        }
        button2PrevState = button2State; // Update previous state



        sprintf(string, "Camera Mode: %d", button2Toggle);
        Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)string,
                                    AUTO_STRING_LENGTH,
                                    64,
                                    100,
                                    OPAQUE_TEXT);



        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Tank movement: ",
                                                 AUTO_STRING_LENGTH,
                                                 64,
                                                 30,
                                                 OPAQUE_TEXT);




        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Camera movement: ",
                                                 AUTO_STRING_LENGTH,
                                                 64,
                                                 60,
                                                 OPAQUE_TEXT);


        //Car movement


        if (button2Toggle==0) {



            if (resultsBuffer[1] > THRESHOLD_HIGH) {
                // If value y is greater than 12000 car moves forward
                //printf("Tank Forwards\n");
                Graphics_drawStringCentered(&g_sContext,
                                                        (int8_t *)"  Forward ",
                                                        AUTO_STRING_LENGTH,
                                                        64,
                                                        40,
                                                        OPAQUE_TEXT);
            } else if (resultsBuffer[1] < THRESHOLD_LOW) {
                // If value y is less than 1000 car moves backwards
                //printf("Tank Backwards\n");
                Graphics_drawStringCentered(&g_sContext,
                                                        (int8_t *)" Backward ",
                                                        AUTO_STRING_LENGTH,
                                                        64,
                                                        40,
                                                        OPAQUE_TEXT);
            }

            if (resultsBuffer[0] > THRESHOLD_HIGH) {
                // If value x is greater than 12000 car moves to the right
                //printf("Tank Right\n");
                Graphics_drawStringCentered(&g_sContext,
                                                                     (int8_t *)"  Right   ",
                                                                     AUTO_STRING_LENGTH,
                                                                     64,
                                                                     40,
                                                                     OPAQUE_TEXT);
            } else if (resultsBuffer[0] < 1000 + THRESHOLD_LOW) { //Adding 1000 because the joystick at x = 1000 is almost at its limit position
                // If value x is less than 2000 car moves to the left
                //printf("Tank Left\n");
                Graphics_drawStringCentered(&g_sContext,
                                                                                  (int8_t *)"  Left    ",
                                                                                  AUTO_STRING_LENGTH,
                                                                                  64,
                                                                                  40,
                                                                                  OPAQUE_TEXT);
            }
        }

        //Camera movement
        if (button2Toggle==1) {

            //if (buttonPressed) {

             while (!(P4IN & GPIO_PIN1)){

                                // While button is being pressed the message Default camera will show on screen
                                Graphics_drawStringCentered(&g_sContext, (int8_t *)"Default Camera", AUTO_STRING_LENGTH, 64, 110, OPAQUE_TEXT);
             }
             if (P4IN & GPIO_PIN1){

                                    Graphics_drawStringCentered(&g_sContext, (int8_t *)"              ", AUTO_STRING_LENGTH, 64, 110, OPAQUE_TEXT);
             }

            if (resultsBuffer[1] > THRESHOLD_HIGH) {
                // Servo motor goes up
                //printf("Camera up\n");
                Graphics_drawStringCentered(&g_sContext,
                                                                                  (int8_t *)"  Upward  ",
                                                                                  AUTO_STRING_LENGTH,
                                                                                  64,
                                                                                  70,
                                                                                  OPAQUE_TEXT);

            } else if (resultsBuffer[1] < THRESHOLD_LOW) {
                // Servo motor goes down
                //printf("Camera down\n");
                Graphics_drawStringCentered(&g_sContext,
                                                                                  (int8_t *)" Downward ",
                                                                                  AUTO_STRING_LENGTH,
                                                                                  64,
                                                                                  70,
                                                                                  OPAQUE_TEXT);

            }

            if (resultsBuffer[0] > THRESHOLD_HIGH) {
                // Servo motor turns right
                //printf("Camera right\n");
                Graphics_drawStringCentered(&g_sContext,
                                                                                  (int8_t *)"   Right  ",
                                                                                  AUTO_STRING_LENGTH,
                                                                                  64,
                                                                                  70,
                                                                                  OPAQUE_TEXT);

            } else if (resultsBuffer[0] < 1000 + THRESHOLD_LOW) {
                // Servo motor turns left
                //printf("Camera left\n");
                Graphics_drawStringCentered(&g_sContext,
                                                                                  (int8_t *)"   Left   ",
                                                                                  AUTO_STRING_LENGTH,
                                                                                  64,
                                                                                  70,
                                                                                  OPAQUE_TEXT);

            }
        }

    }

}



