#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Graphics library context */
Graphics_Context g_sContext;

/* State machine states */
typedef enum {
    STATE_INIT,
    STATE_SECONDARY,
    STATE_PRIMARY,
    NUM_STATES
} State_t;

/* State machine structure */
typedef struct {
    State_t state;
    void (*state_function)(void);
} StateMachine_t;

/* Function prototypes */
void fn_INIT(void);
void fn_SECONDARY(void);
void fn_PRIMARY(void);

/* Current state */
State_t current_state = STATE_INIT;

/* Event */
typedef enum {
    SW_NONE,
    START_BUTTON_PRESSED,
} Event_t;

volatile Event_t event = SW_NONE;

/* State machine */
StateMachine_t fsm[] = {
    {STATE_INIT, fn_INIT},
    {STATE_SECONDARY, fn_SECONDARY},
    {STATE_PRIMARY, fn_PRIMARY},
};

/* Graphics initialization function */
void _graphicsInit() {
    /* Initialize the display */
    Crystalfontz128x128_Init();

    /* Set the default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initialize the graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_clearDisplay(&g_sContext);

    /* Set the font to ensure text is displayed correctly */
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
}

/* Function to draw a simple car */
void drawCar() {
    /* Car body */
    Graphics_drawRectangle(&g_sContext, &(Graphics_Rectangle){20, 50, 100, 70}); // Car body

    /* Car roof */
    Graphics_drawLine(&g_sContext, 30, 50, 50, 30);  // Left slanted line
    Graphics_drawLine(&g_sContext, 50, 30, 80, 30);  // Top horizontal line
    Graphics_drawLine(&g_sContext, 80, 30, 100, 50); // Right slanted line

    /* Car windows */
    Graphics_drawLine(&g_sContext, 50, 30, 50, 50); // Left divider
    Graphics_drawLine(&g_sContext, 80, 30, 80, 50); // Right divider

    /* Car wheels */
    Graphics_drawCircle(&g_sContext, 35, 75, 10);  // Left wheel
    Graphics_drawCircle(&g_sContext, 85, 75, 10);  // Right wheel

    /* Message below the car */
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8); // Ensure the font before drawing text
    Graphics_drawStringCentered(&g_sContext, (int8_t *)"Welcome back!",
                                AUTO_STRING_LENGTH, 64, 110, OPAQUE_TEXT);
}

/* INIT function */
void fn_INIT() {
    if (event == START_BUTTON_PRESSED) {
        Graphics_clearDisplay(&g_sContext);
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

        drawCar();
        current_state = STATE_SECONDARY;
        event = SW_NONE;
    }
}

/* SECONDARY function */
void fn_SECONDARY() {
    if (event == START_BUTTON_PRESSED) {
        Graphics_clearDisplay(&g_sContext);
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        current_state = STATE_PRIMARY;
        event = SW_NONE;

    }
}

/* PRIMARY function */
void fn_PRIMARY() {
    if (event == START_BUTTON_PRESSED) {
        Graphics_clearDisplay(&g_sContext);
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

        current_state = STATE_SECONDARY;
        event = SW_NONE;
    }
}

/* Configure button */
void startButton_interrups() {
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_interruptEdgeSelect(GPIO_PORT_P3, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);
    Interrupt_enableInterrupt(INT_PORT3);
}

/* Button interrupt handler */
void PORT3_IRQHandler(void) {
    if (P3->IFG & BIT5) {
        event = START_BUTTON_PRESSED;
        P3->IFG &= ~BIT5;
    }
}

/* Main function */
void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop the watchdog

    _graphicsInit();
    startButton_interrups();

    while (1) {
        if (current_state < NUM_STATES) {
            (*fsm[current_state].state_function)();
        }
    }
}
