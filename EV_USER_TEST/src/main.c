#include <avr/io.h>
#include <util/delay.h>

#define IDLE 0
#define START 1
#define PAUSE 2
#define STOP 3

int main() {
    // Set Port C (PC0-PC3) and Port B (PB0-PB3) as outputs
    DDRC |= (1 << 0x0F);  // Set PC0-PC3 as output (for controlling LEDs)
    DDRB |= (1 << 0x0F);  // Set PB0-PB3 as output (for controlling multiplexer selector pins)
    DDRA &= ~(1 << PA0);  // Set PA0 as input (for reading multiplexer output)

    // Initialize state_array for each group (4 groups)
    uint8_t state_array[4] = {IDLE, IDLE, IDLE, IDLE};
    // uint8_t group_id;  // The group currently being processed

    while (1) {

        // Update the LEDs based on the state array
        // Cycle through all 16 channels of the multiplexer
        for (int i = 0; i < 16; i++) {
            // Determine the group_id based on the multiplexer channel
            static uint8_t group_id_input;
            group_id_input = (i / 4);  // There are 4 channels per group (0-3, 4-7, etc.)

            // Set the multiplexer selector pins on Port B (PB0-PB3) to select the current channel
            PORTB = (PORTB & 0xF0) | (i & 0x0F);  // Set PB0-PB3 to the current value of i (0-15)

            _delay_ms(1);  // Small delay to allow the multiplexer's output to stabilize

            // Check the button state on PA0 (multiplexer output)
            if (PINA & (1 << PA0)) {
                // If the button is pressed, update the state for this group
                state_array[group_id_input] = (i % 4) + 1;  // Map button press to state (IDLE, START, PAUSE, STOP)
            }

            for (int j = 0; j < 4; j++) {
                // group_id = i;
                // uint8_t current_state = state_array[group_id];
                switch (state_array[j]) {
                    case IDLE:
                        PORTC = (PORTC & 0xF0) | (0 & 0x0F);  // Set LEDs to IDLE state
                        break;
                    case START:
                        PORTC = (PORTC & 0xF0) | ((1 + (4 * j)) & 0x0F);  // Set LEDs to START state
                        break;
                    case PAUSE:
                        PORTC = (PORTC & 0xF0) | ((2 + (4 * j)) & 0x0F);  // Set LEDs to PAUSE state
                        break;
                    case STOP:
                        PORTC = (PORTC & 0xF0) | ((3 + (4 * j)) & 0x0F);  // Set LEDs to STOP state
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return 0;
}
