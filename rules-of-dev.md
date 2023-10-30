# Rules of development

The goal of following these rules is to create code that is easy to statically analyze and as robust as possible, thereby minimizing extensive debugging time. Adhering to these rules upfront saves valuable time that might otherwise be spent troubleshooting issues in the codebase.

These rules are inspired by "The Power of 10: Rules for Developing Safety-Critical Code" by Gerard J. Holzmann. You can read the original paper https://ieeexplore.ieee.org/document/1642624

---

## Number 1 - Simple control flow statements

Avoid using complex control flow structures. Specifically, do not use goto statements, setjmp, long jump, and recursion. By simplifying control flow, the code becomes more understandable and easier to analyze statically.

## Number 2 - Limit your loops

For loops, always provide a fixed upper bound (an integer `MAX_ITER`). This prevents runaway code and ensures that loops are bounded within a specific range. This upper bound should be easily provable by static analysis tools.

```C++
const int MAX_ITER = 10; // Define the maximum number of iterations

void limitedLoopExample(int array[], int size) {
    std::cout << "Array elements: ";
    for (int i = 0; i < size && i < MAX_ITER; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}
```

## Number 3 - Dynamic Memory Management

Avoid manual dynamic memory allocation from the heap, but use smart pointers for automated memory management. Specifically, prefer `std::unique_ptr` and `std::shared_ptr` over raw pointers for ownership semantics.

## Number 4 - Limit function size

Keep functions concise and focused. A function should have a single responsibility and be limited to less than 60 lines of code. Shorter functions are easier to understand, test, and maintain, improving overall code quality.

## Number 5 - Practive data hiding

Declare variables with the smallest possible scope. This principle supports data hiding, ensuring that variables are only accessible where they are needed. Minimizing the scope of variables reduces the chance of unintended side effects and simplifies fault diagnosis.

## Number 6 - Check return values

Always check the return values of non-void functions. Unused return values should be explicitly cast to void if they are intentionally ignored. For instance:

```C++
(void)printf("Number of msgs: %d", msgs);
```

```C++
int returnValueExample() {
    int value = 42;
    if (value > 0) {
        return value;
    } else {
        // Handle error condition
        return -1;
    }
}
```

## Number 7 - Limit the pre-processor

Restrict the use of the pre-processor to file inclusion and simple conditional macros. Avoid complex pre-processor constructs like token pasting and variable argument lists. Excessive use of conditional compilation directives should be avoided as well, as they significantly increase the testing effort.

```C++
#define MAX_SIZE 100 // Simple macro definition
#include "header.h" // File inclusion
```

## Number 8 - Restrict pointer use

Limit the use of pointers to a single dereference level. Avoid using function pointers, as they can hinder automated checks by code analyzers. Restricting pointers reduces the chances of null pointer dereferences and pointer arithmetic errors, making the code more reliable.

```C++
void pointerViolationExample() {
    int value = 42;
    int* ptr = &value;
    int** doublePtr = &ptr; // Double dereference
    int retrievedValue = **doublePtr; // Accessing the value through double dereferencing
    printf("Retrieved Value: %d\n", retrievedValue);
}
```

```C++
void restrictPointerExample() {
    int variable = 10;
    int* ptr = &variable; // Single dereference level
    // Code using ptr
}
```

## Number 9 - Be pedantic

Compile the code with all compiler warnings enabled at the most pedantic setting available. Additionally, the code should be subjected to static analysis and unit tests. These practices ensure that potential issues are caught early in the development process. Address all warnings, even if they seem erroneous, to maintain code clarity and correctness.

## Number 10 - Notate the unit of measurement

For variables representing quantities, always note the unit of measurement where applicable. This practice enhances code readability and clarifies the context in which variables are used. Clear notation of units improves understanding, especially in safety-critical applications where precise measurements are essential.

## Number 11 - Use a minimum of two runtime assertions per function

Implement at least two runtime assertions per function. Assertions are used to verify assumptions and check for anomalous conditions during runtime. By incorporating assertions, developers can identify issues early and facilitate proper error handling. Assertions serve as an essential tool for ensuring code correctness and robustness.

```C++
#include <assert.h>

int divide(int numerator, int denominator) {
    assert(denominator != 0); // Assertion 1
    assert(numerator >= 0);    // Assertion 2
    return numerator / denominator;
}
```
