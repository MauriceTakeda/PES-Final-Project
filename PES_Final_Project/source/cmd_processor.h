/**
 * @file cmd_processor.h
 * @brief UART command processor
 *
 * This h file provides functionality for
 * receiving and handling command strings sent
 * over UART.
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 *
 */
#ifndef CMD_PROCESSOR_H_
#define CMD_PROCESSOR_H_

/**
 * @brief Accumulates characters received over UART into a string.
 *        Calls process_command() upon receiving the '\r' character,
 *        then prints a new line for the user to input another command.
 *
 * @return none
 */
void accumulate_line();

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
void process_command(char *input);

/**
 * @brief Handles the reception of a set color command from the user.
 *
 * @param argc   - Number of tokens contained in argv[]
 * @param argv[] - Array of pointers to the start of the character arrays for each token
 *
 * @return none
 */
void handle_color(int argc, char *argv[]);

/**
 * @brief Handles the reception of a set acceleration command from the user.
 *
 * @param argc   - Number of tokens contained in argv[]
 * @param argv[] - Array of pointers to the start of the character arrays for each token
 *
 * @return none
 */
void handle_acceleration(int argc, char *argv[]);

/**
 * @brief Handles the reception of a print acceleration data command from the user.
 *
 * @param argc   - Number of tokens contained in argv[]
 * @param argv[] - Array of pointers to the start of the character arrays for each token
 *
 * @return none
 */
void handle_print(int argc, char *argv[]);

extern uint8_t target_r_val;
extern uint8_t target_g_val;
extern uint8_t target_b_val;
extern float   target_acceleration;
extern bool    print_acceleration;

#endif /* CMD_PROCESSOR_H_ */
