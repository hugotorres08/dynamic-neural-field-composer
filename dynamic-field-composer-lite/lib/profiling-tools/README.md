# Timer Class

The `Timer` class provides a simple way to measure the duration of a code block in C++. It uses C++11's `<chrono>` library to achieve high-resolution timing.

## Usage

To use the `Timer` class, include the `"timer.h"` header file in your C++ source file. Then, create a `Timer` object within the code block you want to measure. When the `Timer` object goes out of scope, it will calculate the duration and print out the function name along with the duration in microseconds.

### Example

```cpp
#include "timer.h"
#include <iostream>

void your_function()
{
    {
        Timer t("your_function"); // Create a Timer object with the function name
        const int a = 1 + 5;
        std::cout << "My integer is: " << a << std::endl;
    }
}

int main()
{
    your_function();
    return 0;
}
```

In the above example, the `Timer` object `t` measures the duration of the code block inside the `your_function` function.


