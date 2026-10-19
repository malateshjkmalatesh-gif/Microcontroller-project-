
#include <xc.h>                       // Include processor-specific header
#include <string.h>                   // Include string functions (optional here)
#include "message_handler.h"         // Include function declarations for message handlers
#include "msg_id.h"                   // Include CAN message ID macros
#include "can.h"                      // Include CAN communication functions
#include "clcd.h"                     // Include CLCD display functions
#include "uart.h"                     // Include UART communication functions

volatile unsigned char led_state = LED_OFF, status = e_ind_off;  // Variables to track LED and indicator status

// Handle received speed data
void handle_speed_data(uint8_t *data, uint8_t len)
{
    clcd_putch(data[0] / 10 + '0', LINE2(0));  // Display tens digit of speed
    clcd_putch(data[0] % 10 + '0', LINE2(1));  // Display units digit of speed
}

// Handle received gear data
void handle_gear_data(uint8_t *data, uint8_t len) 
{
    clcd_putch(*data, LINE2(4));               // Display gear character at position 4 on line 2
}

// Handle received RPM data
void handle_rpm_data(uint8_t *data, uint8_t len) 
{
    for (unsigned int i = 0; i < len; i++) {
        clcd_putch(data[i], LINE2(6 + i));     // Display RPM string starting from position 6
    }
}

// Handle received engine temperature data (not implemented)
void handle_engine_temp_data(uint8_t *data, uint8_t len) 
{
    // Reserved for future use
}

// Handle received indicator status data
void handle_indicator_data(uint8_t *data, uint8_t len) 
{
    if (*data == 1) {
        LEFT_IND_ON();                         // Turn on left indicator
        RIGHT_IND_OFF();                       // Turn off right indicator
        clcd_print("<  ", LINE2(11));          // Display left arrow
    }
    if (*data == 0) {
        LEFT_IND_OFF();                        // Turn off both indicators
        RIGHT_IND_OFF();
        clcd_print("OFF", LINE2(11));          // Display "OFF"
    }
    if (*data == 2) {
        LEFT_IND_OFF();                        // Turn off left indicator
        RIGHT_IND_ON();                        // Turn on right indicator
        clcd_print("  >", LINE2(11));          // Display right arrow
    }
}

// Process all incoming CAN messages
void process_canbus_data() 
{
    uint16_t msg_id;                           // Variable to store received message ID
    uint8_t data[8];                           // Array to store received data
    uint8_t len;                               // Variable to store length of data

    can_receive(&msg_id, data, &len);          // Receive CAN message

    // Handle message based on message ID
    switch (msg_id)
    {
        case SPEED_MSG_ID: 
            handle_speed_data(data, len);      // Call speed handler
            break;

        case GEAR_MSG_ID:   
            handle_gear_data(data, len);       // Call gear handler
            break;

        case RPM_MSG_ID:   
            handle_rpm_data(data, len);        // Call RPM handler
            break;

        case INDICATOR_MSG_ID:    
            handle_indicator_data(data, len);  // Call indicator handler
            break;
    }
}
