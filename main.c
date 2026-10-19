#include <xc.h>                         
#include <stdint.h>                      // Include standard integer types
#include "can.h"                         // Include CAN communication functions
#include "clcd.h"                        // Include CLCD display functions
#include "msg_id.h"                      // Include CAN message ID definitions
#include "message_handler.h"            // Include function to handle received messages
#include "timer0.h"                      // Include Timer0 configuration functions

// Function to initialize LEDs
static void init_leds() 
{
    TRISB = 0x08;                        // Set RB3 as input, rest of PORTB as output
    PORTB = 0x00;                        // Clear PORTB initially
}

// Function to initialize all peripherals
static void init_config(void) 
{
    init_clcd();                         // Initialize character LCD
    init_leds();                         // Initialize LED pins
    // PEIE = 1;                          // Enable peripheral interrupts (commented out)
    // GIE = 1;                           // Enable global interrupts (commented out)
    // init_timer0();                    // Initialize Timer0 (commented out)
    init_can();                          // Initialize CAN communication

    clcd_print("spd gear rpm ind", LINE1(0));  // Display headers on line 1 of CLCD
}

// Main function
void main(void) 
{
    init_config();                       // Call peripheral initialization

    while (1) 
    {
        process_canbus_data();          // Continuously receive and process CAN data
    }

    return;                              // Not required for void main, but safe to include
}
