// PROJECT  : RPN Calculator
// PURPOSE  : To declare the Stack class used to manage stack operations
// COURSE   : ICS3U-E
// AUTHOR   : T. Park
// DATE     : 2025 18 04
// MCU      : 328P (Standalone)
// STATUS   : Working
// REFERENCE: http://darcy.rsgc.on.ca/ACES/TEI3M/2425/Submissions.html

#ifndef Stack_h              // Prevent multiple inclusions of this header file
#define Stack_h

#include "Arduino.h"         // Include Arduino types (e.g., uint8_t)

class Stack {                // Define Stack class
  public:
    Stack();                 // Default constructor
    Stack(uint8_t size);     // Constructor with custom size
    void push(float operand);// Push a value onto the stack
    float pop();             // Pop the top value
    float peek();            // Peek the top value
    boolean isFull();        // Check if stack is full
    boolean isEmpty();       // Check if stack is empty
    int8_t _top;             // Index of the top element (note: usually private)

  private:
    uint8_t _capacity;       // Max number of elements
    float* _stack;           // Pointer to stack array
};

#endif                       // End of include guard
