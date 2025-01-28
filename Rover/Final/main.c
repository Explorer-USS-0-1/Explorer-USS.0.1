
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "msp.h"

#include "functions.h"

#define THRESHOLD_HIGH 12000
#define THRESHOLD_LOW 1000

// Graphics library context
Graphics_Context g_sContext;

// ADC results buffer
volatile int mode = 0;
volatile uint16_t resultsBuffer[2];

// Initialize buttons state values to 0
volatile int button2Toggle = 0; // Toggles for Button2
volatile int button2PrevState = 0; // Previous state of Button2
volatile int button3Toggle = 0; // Toggles for Button2
volatile int button3PrevState = 0; // Previous state of Button2


// Function prototypes
void fn_IMAGE(void);
void drawCar(void);
void startButtonInit(void);
void ADC14_IRQHandler(void);


// Function to draw a car
void drawCar() {
    //Graphics_clearDisplay(&g_sContext);
    Graphics_drawRectangle(&g_sContext, &(Graphics_Rectangle){20, 50, 100, 70}); // Car body
    Graphics_drawLine(&g_sContext, 30, 50, 50, 30);  // Left slanted line
    Graphics_drawLine(&g_sContext, 50, 30, 80, 30);  // Top horizontal line
    Graphics_drawLine(&g_sContext, 80, 30, 100, 50); // Right slanted line
    Graphics_drawCircle(&g_sContext, 35, 75, 10);  // Left wheel
    Graphics_drawCircle(&g_sContext, 85, 75, 10);  // Right wheel
    Graphics_drawStringCentered(&g_sContext, (int8_t *)"Welcome back!", AUTO_STRING_LENGTH, 64, 110, OPAQUE_TEXT);
}

// IMAGE state
void fn_IMAGE() {
        printf("We are in IMAGE MODE");
        //Graphics_clearDisplay(&g_sContext);
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

        drawCar();
        //event = SW_NONE;
    }



// MAIN FUNCTION ---------------------------------------------------------------------------------------------

int8_t *x = (int8_t *)"Neutral ";
int8_t *y = (int8_t *)"Neutral ";

/* MAIN FUNCTION  */
void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop the watchdog

    _hwInit();
    // startButtonInit();
    UART_init();
    sendOnceTank(WriteTank);


    while (1) {
        PCM_gotoLPM0(); // Enter low-power mode
    }
}

// ADC interrupt handler
    void ADC14_IRQHandler(void) {
            uint64_t status = ADC14_getEnabledInterruptStatus();
            ADC14_clearInterruptFlag(status);

            if (status & ADC_INT1)
            {
                // Store ADC results
                resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
                resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

                char string[10];

        // Switch statement to handle different modes
        switch (mode) {
            case 0: // Image mode
                //Graphics_clearDisplay(&g_sContext);
                fn_IMAGE();
                break;

            case 1: // Tank mode


                Graphics_drawStringCentered(&g_sContext, x ,AUTO_STRING_LENGTH,40,80,OPAQUE_TEXT);
                Graphics_drawStringCentered(&g_sContext, y ,AUTO_STRING_LENGTH,88,80,OPAQUE_TEXT);

                if(resultsBuffer[0] < THRESHOLD_HIGH - 2000 && resultsBuffer[0] > THRESHOLD_LOW + 2000 && resultsBuffer[1] < THRESHOLD_HIGH - 2000 && resultsBuffer[1] > THRESHOLD_LOW + 2000 )
                {
                    Graphics_drawStringCentered(&g_sContext, (int8_t *)"Stationary",AUTO_STRING_LENGTH,64,40,OPAQUE_TEXT);
                    //Graphics_drawStringCentered(&g_sContext, x ,AUTO_STRING_LENGTH,64,70,OPAQUE_TEXT);
                }

                // **Button2 Toggle (Top button)**
                int button2State = !(P5IN & GPIO_PIN1); // Read Button2 state (active low)
                if (button2State && !button2PrevState) // Detect rising edge
                {
                    button2Toggle = !button2Toggle; // Toggle state
                    sendAlternateMessage();
                }
                button2PrevState = button2State; // Update previous state
                sprintf(string, "Camera Mode: %d", button2Toggle);
                Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,100,OPAQUE_TEXT);
                Graphics_drawStringCentered(&g_sContext, (int8_t *)"Tank movement: ",AUTO_STRING_LENGTH,64,30,OPAQUE_TEXT);
                Graphics_drawStringCentered(&g_sContext, (int8_t *)"Camera movement: ",AUTO_STRING_LENGTH,64,60,OPAQUE_TEXT);
                Graphics_drawStringCentered(&g_sContext, (int8_t *)"Y: ",AUTO_STRING_LENGTH,40,70,OPAQUE_TEXT);
                Graphics_drawStringCentered(&g_sContext, (int8_t *)"X: ",AUTO_STRING_LENGTH,88,70,OPAQUE_TEXT);
                //tank movement
                if (button2Toggle==0) {
                    if ((resultsBuffer[1] < THRESHOLD_HIGH) && (resultsBuffer[1] > THRESHOLD_LOW+1000) &&
                                                    (resultsBuffer[0] < THRESHOLD_HIGH) && (resultsBuffer[0] > THRESHOLD_LOW)) {
                                                // If value y is greater than 12000 car moves forward
                                               // printf("Tank Forwards\n");
                                    sendOnceTank(WriteDefault);}

                    else if (resultsBuffer[1] > THRESHOLD_HIGH) {
                    // If value y is greater than 12000 car moves forward
                    //printf("Tank Forwards\n");
                        Graphics_drawStringCentered(&g_sContext,(int8_t *)"  Forward ",AUTO_STRING_LENGTH,64,40,OPAQUE_TEXT);
                        sendOnceTank(WriteForward);
                    } else if (resultsBuffer[1] < THRESHOLD_LOW) {
                    // If value y is less than 1000 car moves backwards
                    //printf("Tank Backwards\n");
                        Graphics_drawStringCentered(&g_sContext,(int8_t *)" Backward ",AUTO_STRING_LENGTH,64,40,OPAQUE_TEXT);
                        sendOnceTank(WriteBackward);
                    }
                    else if (resultsBuffer[0] > THRESHOLD_HIGH) {
                    // If value x is greater than 12000 car moves to the right
                    //printf("Tank Right\n");
                        Graphics_drawStringCentered(&g_sContext,(int8_t *)"  Right   ",AUTO_STRING_LENGTH,64,40,OPAQUE_TEXT);
                        sendOnceTank(WriteRight);
                    // Adding 1000 because the joystick at x = 1000 is almost at its limit position
                    } else if (resultsBuffer[0] < 1000 + THRESHOLD_LOW) {
                    // If value x is less than 2000 car moves to the left
                    //printf("Tank Left\n");
                        Graphics_drawStringCentered(&g_sContext,(int8_t *)"   Left   ",AUTO_STRING_LENGTH,64,40,OPAQUE_TEXT);
                        sendOnceTank(WriteLeft);
                    }
                }
                //Camera movement
                if (button2Toggle==1) {
                    // while (!(P4IN & GPIO_PIN1)){
                    if (!(P4IN & GPIO_PIN1)){
                    // While button is being pressed the message Default camera will show on screen
                        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Default Camera", AUTO_STRING_LENGTH, 64, 110, OPAQUE_TEXT);
                        x = (int8_t *)"Neutral ";
                        y = (int8_t *)"Neutral ";
                        sendOnceCamera(WriteMiddle);
                    }
                    if (P4IN & GPIO_PIN1){
                        Graphics_drawStringCentered(&g_sContext, (int8_t *)"              ", AUTO_STRING_LENGTH, 64, 110, OPAQUE_TEXT);
                    }
                    if (resultsBuffer[1] > THRESHOLD_HIGH) {
                    // Servo motor goes up
                    //printf("Camera up\n");
                        x = (int8_t *)"   Up   ";
                        sendOnceCamera(WriteUp);
                    } else if (resultsBuffer[1] < THRESHOLD_LOW) {
                    // Servo motor goes down
                    //printf("Camera down\n");
                        //Graphics_drawStringCentered(&g_sContext,(int8_t *)" Downward ",AUTO_STRING_LENGTH,64,70,OPAQUE_TEXT);
                        x = (int8_t *)"  Down  ";
                        sendOnceCamera(WriteDown);
                    }
                    if (resultsBuffer[0] > THRESHOLD_HIGH) {
                    // Servo motor turns right
                    //printf("Camera right\n");
                        //Graphics_drawStringCentered(&g_sContext,(int8_t *)"   Right  ",AUTO_STRING_LENGTH,64,70, OPAQUE_TEXT);
                        y = (int8_t *)"  Right ";
                        sendOnceCamera(WriteCright);
                    } else if (resultsBuffer[0] < 1000 + THRESHOLD_LOW) {
                    // Servo motor turns left
                    //printf("Camera left\n");
                        //Graphics_drawStringCentered(&g_sContext, (int8_t *)"   Left   ", AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);
                        y = (int8_t *)"  Left  ";
                        sendOnceCamera(WriteCleft);

                    }
                }
                break;

            default:
                Graphics_drawStringCentered(&g_sContext, (int8_t *)"Invalid Mode", AUTO_STRING_LENGTH, 64, 110, OPAQUE_TEXT);
                break;
            }
                    // **Button3 Toggle (Bottom button)**
                    int button3State = !(P3IN & GPIO_PIN5); // Read Button2 state (active low)

                    if (button3State && !button3PrevState) // Detect rising edge
                    {
                        mode = !mode; // Toggle state
                        Graphics_clearDisplay(&g_sContext);
                        printf("Mode %d:", mode);
                    }
                    button3PrevState = button3State; // Update previous state
            }

    }
