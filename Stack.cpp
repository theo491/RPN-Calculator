// PROJECT :RPN Calculator
// PURPOSE :To hold the actual implementation of the functions declared in the header file
// COURSE  :ICS3U-E
// AUTHOR  :T. Park
// DATE.   :2025 18 04
// MCU.    :328P (Standalone)
// STATUS  :Working
// REFRENCE:http://darcy.rsgc.on.ca/ACES/TEI3M/2425/Submissions.html

#include "Arduino.h"               // Arduino core library
#include "Stack.h"                 // Stack class header

Stack::Stack() {
  _top = -1;                       // Empty stack
  _capacity = 20;                  // Set capacity to 20
  _stack = new float[_capacity];   // Allocate memory for stack
}

// Constructor with custom size
Stack::Stack(uint8_t size) {
  _top = -1;                       // Empty stack
  _capacity = size;                // Set custom capacity
  _stack = new float[_capacity];   // Allocate memory for stack
}

void Stack::push(float operand) {  // Push value onto the stack
  if (!isFull()) {                 // If stack isn't full
    _stack[++_top] = operand;      // Increment top and push value
  }
}

float Stack::pop() {              // Pop value from the stack
  if (!isEmpty()) {                // If stack isn't empty
    return _stack[_top--];         // Return top value and decrement top
  }
  return 0.0;                      // Return 0 if stack is empty
}

float Stack::peek() {             // Peek top value without popping
  if (!isEmpty()) {                // If stack isn't empty
    return _stack[_top];           // Return top value
  }
  return 0.0;                      // Return 0 if stack is empty
}

boolean Stack::isFull() {         // Check if stack is full
  return _top == _capacity - 1;    // Full if top is at capacity-1
}

boolean Stack::isEmpty() {         // Check if stack is empty
  return _top == -1;               // Empty if top is -1
}
