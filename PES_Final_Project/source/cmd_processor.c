/**
 * @file cmd_processor.c
 * @brief UART command processor
 *
 * This c file provides functionality for
 * receiving and handling command strings sent
 * over UART.
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "rgb_led.h"
#include "cmd_processor.h"


typedef void (*command_handler_t)(int, char *argv[]);

typedef struct {
	const char *name;
	command_handler_t handler;
} command_table_t;

// Command table
static const command_table_t commands[] = {
	{ .name="color"       , .handler=handle_color        },
	{ .name="acceleration", .handler=handle_acceleration },
	{ .name="print"       , .handler=handle_print        }
};

static const int num_commands = sizeof(commands) / sizeof(command_table_t);

static char line[256];
static int  line_index = 0;


/**
 * @brief Accumulates characters received over UART into a string.
 *        Calls process_command() upon receiving the '\r' character,
 *        then prints a new line for the user to input another command.
 *
 * @return none
 */
void accumulate_line() {
	// Get character
	char character = getchar();
	if(character == EOF) return;

	if(print_acceleration) {
		print_acceleration = false;
		printf("\n\r");
		printf("> ");
	}
	// If the user pressed enter, process the accumulated line,
	// and then print a new line for the user to enter other commands
	else if(character == '\r') {
		printf("\n\r");
		line[line_index] = '\0';
		line_index = 0;
		process_command(line);
		printf("> ");
	}
	// Else if the user presses backspace, then delete one character
	else if(character == '\b') {
		printf("\b \b");
		line_index--;
	}
	// Else echo back the received character, and append it to the
	// accumulated line
	else {
		printf("%c", character);
		line[line_index] = character;
		line_index++;
	}
} // accumulate_line()

/**
 * @brief Processes the input character array, and calls the necessary
 *        command handler if a valid command is contained in the character
 *        array. Otherwise, notifies the user that the input command is
 *        invalid.
 *
 * @param input - Pointer to the char array to be processed
 *
 * @return none
 */
void process_command(char *input) {
	char *p = input;
	char *end;

	// Find end of string
	for(end = input; *end != '\0'; end++)
		;

	// Tokenize input in place
	bool in_token = false;
	char *argv[10];
	memset(argv, 0, sizeof(argv));
	int argc = 0;
	for(p = input; p < end; p++) {
		// If current char is whitespace
		if(isspace(*p)) {
		    // And if we were previously in a token
			if(in_token) {
				// Replace the whitespace with a null terminator
				// in order to separate the token from remaining characters
				// in the array
				*p = '\0';
				in_token = false;
			}
		}
		// If current char is not whitespace
		else {
			// And if we are not already in a token
			if(!in_token) {
				// This marks the start of a new token, so
				// add this new token to argv[]
				argv[argc] = p;
				argc++;
				in_token = true;
			}
		}
	}
	argv[argc] = NULL;
	if(argc == 0) return; // No non-whitespace characters contained in the input string

	// Dispatch argc/argv to handler
	for(int i = 0; i < num_commands; i++) {
		if(strcasecmp(argv[0], commands[i].name) == 0) {
		    commands[i].handler(argc, argv);
			return;
		}
	}
	// If we reach this line, then there were no valid commands
	// in input string. Notify the user.
	printf("Unknown command: %s\n\r", input);
} // process_command()

/**
 * @brief Handles the reception of a set color command from the user.
 *
 * @param argc   - Number of tokens contained in argv[]
 * @param argv[] - Array of pointers to the start of the character arrays for each token
 *
 * @return none
 */
void handle_color(int argc, char *argv[]) {
	// Color command should have 3 arguments
	if(argc != 4) {
		printf("Invalid input: The color command requires r, g, and b arguments\n\r");
		printf("E.g. color 0 255 150\n\r");
		return;
	}

	int status, r, g, b;

	// Check for validity of r argument
	status = sscanf(argv[1], "%d", &r);
	if(status != 1) {
		printf("Invalid argument: Check for correctness of the r argument\n\r");
		printf("Example: acceleration 0 255 150\n\r");
		return;
	}
	if(r < 0 || r > 255) {
		printf("Invalid argument: The r argument must be greater than or equal to zero and less than or equal to 255\n\r");
		return;
	}
	// Check for validity of g argument
	status = sscanf(argv[2], "%d", &g);
	if(status != 1) {
		printf("Invalid argument: Check for correctness of the g argument\n\r");
		printf("Example: acceleration 0 255 150\n\r");
		return;
	}
	if(g < 0 || g > 255) {
		printf("Invalid argument: The g argument must be greater than or equal to zero and less than or equal to 255\n\r");
		return;
	}
	// Check for validity of b argument
	status = sscanf(argv[3], "%d", &b);
	if(status != 1) {
		printf("Invalid argument: Check for correctness of the b argument\n\r");
		printf("Example: acceleration 0 255 150\n\r");
		return;
	}
	if(b < 0 || b > 255) {
		printf("Invalid argument: The b argument must be greater than or equal to zero and less than or equal to 255\n\r");
		return;
	}

	target_r_val = r;
	target_g_val = g;
	target_b_val = b;
	printf("Target color set to r=%d, g=%d, b=%d\n\r", r, g ,b);
} // handle_color()

/**
 * @brief Handles the reception of a set acceleration command from the user.
 *
 * @param argc   - Number of tokens contained in argv[]
 * @param argv[] - Array of pointers to the start of the character arrays for each token
 *
 * @return none
 */
void handle_acceleration(int argc, char *argv[]) {
	// Acceleration command requires one argument
	if(argc != 2) {
		printf("Invalid argument: The acceleration command requires target acceleration argument\n\r");
		printf("E.g. acceleration <target acceleration value in m/s^2>\n\r");
		return;
	}

	int status;
	float target;

	// Check for validity of target acceleration argument
	status = sscanf(argv[1], "%f", &target);
	if(status != 1) {
		printf("Invalid argument: Check for correctness of the target acceleration argument\n\r");
		printf("Example: acceleration 10.2\n\r");
		return;
	}
	if(target < 0) {
		printf("Invalid argument: The target acceleration argument must be greater than or equal to zero\n\r");
		return;
	}

	target_acceleration = target;
	printf("Target acceleration set to %f m/s^2\n\r", target);
} // handle_acceleration()

/**
 * @brief Handles the reception of a print acceleration data command from the user.
 *
 * @param argc   - Number of tokens contained in argv[]
 * @param argv[] - Array of pointers to the start of the character arrays for each token
 *
 * @return none
 */
void handle_print(int argc, char *argv[]) {
	// Print command requires zero arguments
	if(argc != 1) {
		printf("Invalid argument: The print command does not take any arguments\n\r");
		return;
	}

	print_acceleration = true;
} // handle_print()
