#include "sensor.h"       // Include sensor header for function declarations
#include "adc.h"               // Include ADC functions
#include "can.h"               // Include CAN functions
#include "msg_id.h"            // Include CAN message ID definitions
#include "uart.h"              // Include UART functions

char str[3];                   // String to store speed as ASCII characters

// Function to read and return speed value
uint16_t get_speed()
{
    unsigned short int speed;

    speed = read_adc(CHANNEL4) / 10.23;    // Read analog value and convert to speed

    str[0] = (speed / 10) + '0';           // Convert tens digit to ASCII
    str[1] = (speed % 10) + '0';           // Convert units digit to ASCII
    str[2] = '\0';                         // Null-terminate the string

    puts("Speed: ");                       // Print label to UART
    puts(str);                             // Print speed to UART
    putch('\n');                           // Print newline for clarity
    puts("\n\r");                         // Move to new line on terminal

    return speed;                          // Return calculated speed value
}

// Function to read and return current gear position
unsigned char get_gear_pos()
{
    static char str[8] = "RN12345C";       // Gear characters: R, N, 1?5, Collision
    static unsigned char index = 1;        // Initial gear index (pointing to 'N')

    unsigned char key = read_digital_keypad(STATE_CHANGE);  // Read key press

    if (key == COLLISION)
    {
        index = 7;                         // Set to collision gear ('C')
    }
    else if (index == 7 && (key == GEAR_UP || key == GEAR_DOWN))
    {
        index = 1;                         // Reset to Neutral if gear key is pressed after collision
    }
    else if (key == GEAR_UP)
    {
        if (index < 6)                     // Increment gear up to '5'
        {
            index++;
        }
    }
    else if (key == GEAR_DOWN)
    {
        if (index > 0)                     // Decrement gear down to 'R'
        {
            index--;
        }
    }

    return str[index];                     // Return the current gear character
}
