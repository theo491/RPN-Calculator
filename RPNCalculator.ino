// PROJECT :RPN Calculator
// PURPOSE :The purpose of this project is to create an RPN calculator to perform and display stack-based arithmetic operations.
// COURSE :ICS3U-E
// AUTHOR :T. Park
// DATE :2025 18 04
// MCU :328P (Standalone)
// STATUS :Working
// REFRENCE : http://darcy.rsgc.on.ca/ACES/TEI3M/Tasks.html#RPN 

#include <LiquidCrystal.h> 3         // Include LiquidCrystal library
#include "Stack.h"                   // Include custom Stack.h library

#define LCD_COLUMNS 16               // Number of characters per LCD line
#define LCD_ROWS 2                   // Number of rows on LCD
#define KEYPAD_PIN A5                // Analog pin for reading keypad
#define KEYPAD_IDLE_THRESHOLD 20     // Value below which keypad is considered idle
#define INPUT_BUFFER_SIZE 17         // Max char for input (16+null terminator)
#define VALUE_BUFFER_SIZE 10         // Size of buffer for number conversions
#define DECIMAL_PLACES_RESULT 4      // Precision for calculation results
#define DECIMAL_PLACES_STACK 2       // Precision for stack display values
#define NUMBER_WIDTH_RESULT 7        // Width for formatting result numbers
#define NUMBER_WIDTH_STACK 7         // Width for formatting stack numbers
#define ERROR_DISPLAY_MS 1000        // Error message display time in ms
#define MAX_TEMP_STACK 8             // Size of temporary stack for display
#define KEYPAD_NUM_KEYS 16.          // Number of keys on keypad

LiquidCrystal lcd(9, 8, 7, 6, 5, 4); // Initialize LCD with interface pins

uint16_t thresholds[] = {55, 62, 66, 68, 80, 83, 90, 98, 120, 134, 154, 180, 256, 343, 514, 1024};                     // Keypad analog value thresholds
char keys[] = { '+', 'E', '.', '0', '-', '3', '2', '1', '*', '6', '5', '4', '/', '9', '8', '7' };                     // Corresponding key

Stack stack;                         // Stack for RPN operations
char inputBuffer[INPUT_BUFFER_SIZE]; // Buffer for storing user input
uint8_t inputIndex = 0;              // Current position in input buffer
bool newInput = true;                // Flag for starting new input
bool showResult = false;             // Flag to display calculation result
float resultValue = 0;               // Stores last calculation result

void setup() {
lcd.begin(LCD_COLUMNS, LCD_ROWS);    // Initialize LCD
lcd.print("RPN Calculator");         // Display welcome message
delay(ERROR_DISPLAY_MS);             // Wait before clearing screen
lcd.clear();                         // Clear LCD
updateDisplay();                     // Show initial display
}

void loop() {
char key = getKey();                 // Read keypad input
if (!key) return;                    // If no key pressed, exit loop
if (isdigit(key)) handleDigit(key);  // Process numeric input
else if (key == '.') handleDecimal();// Process decimal point
else if (key == 'E') handleEnter();  // Process enter key
else if (strchr("+-*/", key)) handleOperator(key);      // Process operators
updateDisplay();                      // Update display with changes
while (analogRead(KEYPAD_PIN) > KEYPAD_IDLE_THRESHOLD); // Wait for key release
}

char getKey() {
uint16_t val = analogRead(KEYPAD_PIN);       // Read analog value
if (val < KEYPAD_IDLE_THRESHOLD) return 0;   // Return 0 if no key press
val = analogRead(KEYPAD_PIN);                // Take second reading for stability
for (uint8_t i = 0; i < KEYPAD_NUM_KEYS; i++)// Find key was pressed
if (val <= thresholds[i]) return keys[i];    // Return matching key
return 0;                                    // Default return if no match
}

void handleDigit(char key) {
if (newInput) {                              // Clear buffer for new input
inputIndex = 0;                              // Reset index to start of buffer
newInput = showResult = false;               // Mark input state as active
inputBuffer[0] = '\0';                       // Clear input buffer
}
if (inputIndex < INPUT_BUFFER_SIZE - 1)      // Add digit if space is available
inputBuffer[inputIndex++] = key, inputBuffer[inputIndex] = '\0'; //digit+ null
}

void handleDecimal() {
if (newInput) {                              // Start new input with "0."
strcpy(inputBuffer, "0.");                   // Initialize buffer with 0.
inputIndex = 2;                              // Set index past "0."
newInput = showResult = false;               // Reset input state
return;
}
if (!strchr(inputBuffer, '.') && inputIndex < INPUT_BUFFER_SIZE - 1) // Add decimal
inputBuffer[inputIndex++] = '.', inputBuffer[inputIndex] = '\0';     // dot + null
}

void handleEnter() {
if (inputIndex) {                            // If there's input, push to stack
stack.push(atof(inputBuffer));               // Convert string to float and push
inputIndex = 0;                              // Reset input index
inputBuffer[0] = '\0';                       // Clear input buffer
newInput = true;                             // Mark as ready for new input
showResult = false;                          // Don't show until next operation
}
}

void handleOperator(char op) {
if (inputIndex) handleEnter();               // Push current input if any
if (stack._top < 1) return showError("Need 2 operands");            // Need 2 values
float b = stack.pop(), a = stack.pop(), r;   // Pop operands
if (op == '/' && b == 0) return stack.push(a), showError("Divide by zero"); //Divide by 0 error
switch (op) {                                // Perform operation
case '+': r = a + b; break;
case '-': r = a - b; break;
case '*': r = a * b; break;
case '/': r = a / b; break;
}
stack.push(r);                               // Push result to stack
resultValue = r;                             // Store result for display
showResult = true;                           // Set flag to show result
}
void updateDisplay() {
lcd.clear();                                 // Clear screen for update
if (showResult) {                            // If showing result
char buf[VALUE_BUFFER_SIZE];
dtostrf(resultValue, NUMBER_WIDTH_RESULT, DECIMAL_PLACES_RESULT, buf);
lcd.setCursor(LCD_COLUMNS - NUMBER_WIDTH_RESULT - 1, 1);
lcd.print(buf);
return;
}
char line[17] = "";                          // Buffer for top display line
int count = 0;                               // Character count in line
float temp[MAX_TEMP_STACK];                  // Temporary storage for stack elements
int n = 0;
while (stack._top >= 0 && n < MAX_TEMP_STACK)// Copy stack to temp array
temp[n++] = stack.pop();
for (int i = n - 1; i >= 0; i--) {           // Format and display stack values
char buf[VALUE_BUFFER_SIZE];
dtostrf(temp[i], NUMBER_WIDTH_STACK, DECIMAL_PLACES_STACK, buf);
char *num = buf; while (*num == ' ') num++;  // Skip leading spaces
int len = strlen(num);
if (count + len + (count > 0 ? 1 : 0) > LCD_COLUMNS) break; // Stop if line full
if (count) strcat(line, " "), count++;       // Add space between numbers
strcat(line, num), count += len;             // Add number to line
}
lcd.setCursor(0, 0);                         // Position at start of top line
lcd.print(line);                             // Display stack
for (int i = n - 1; i >= 0; i--)             // Restore stack in correct order
stack.push(temp[i]);
if (!newInput && inputIndex) {               // Display current input if any
lcd.setCursor(0, 1);
lcd.print(">");
lcd.print(inputBuffer);
}
}

void showError(const char* msg) {
lcd.clear();                                 // Clear display
lcd.print(msg);                              // Show error message
delay(ERROR_DISPLAY_MS);                     // Wait
updateDisplay();                             // Restore normal display
}
