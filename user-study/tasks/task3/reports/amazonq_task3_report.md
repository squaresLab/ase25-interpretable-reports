# **CWE-190 -- Integer Overflow** (<span style="color:red">**HIGH**</span>)


We observed that an integer overflow arising for your program. It happens when the outcome of an arithmetic operation surpasses the maximum representable value for the utilized data type. It can lead to unpredictable behavior, security vulnerabilities, and system crashes. To address this vulnerability, it is essential to perform thorough range checking and validation of input values. Employ data types with adequate size to 
accommodate the anticipated range of values and implement checks to identify and manage potential overflow conditions.

- **Common Weakness Enumeration (CWE)**: CWE-190
- **Code fix available**: No 🚫
- **Detector library**: Integer Overflow
- **File path**: [task3/task3_sample.c [Ln 38]](task3_sample.c#L38)



### Explain 

**PROMPT**: Explain the "CWE-190 -- Integer Overflow" issue in the following code: ```char* sanitized_url = malloc(strlen(url) + 1);```

**TOOL**: The "CWE-190 - Integer Overflow" vulnerability occurs when an arithmetic operation on an integer data type produces a result that exceeds the maximum or minimum value representable by that data type. This can lead to unexpected behavior, security vulnerabilities, and system crashes.

Here's an example in C that demonstrates an integer overflow vulnerability and how to fix it:

```c
#include <stdio.h>
#include <limits.h>

int main () {
    // Vulnerable code
    unsigned int x = UINT_MAX;  // Maximum value for unsigned int 
    printf("x = %u\n", x);
    x = x + 1; // Integer overflow occurs here
    printf("x = %u\n", x); // Prints 0 instead of UINT_MAX + 1

    // Fixed code
    unsigned long long y = UINT_MAX,
    printf("y = %llu\n", y);
    y = y + 1;
    printf("y = %llu\n", y);

    return 0;
}

```

In the vulnerable code, ```x``` is an ```unsigned int``` variable initialized with the maximum value ```UINT_MAX```. When we try to add 1 to ```x```, an integer overflow occurs because the result exceeds the maximum value representable by the ```unsigned int``` data type. As a result, 
the value "wraps around" to 0, which is an unexpected behavior. 

To fix this issue, we can use a larger data type that can accommodate the expected range of 
values. In the fixed code, we use ```unsigned long long``` instead of ```unsigned int```. This 
data type has a larger range, and adding 1 to ```UINT_MAX``` does not cause an overflow. 

The recommended approach to prevent integer overflow vulnerabilities is to: 

1. Use data types with adequate size to accommodate the expected range of values. 
2. Perform range checking and validation of input values before performing arithmetic 
operations. 
3. Implement checks to detect and handle potential overflow conditions. 
4. Consider using safe arithmetic libraries or functions that can detect and handle 
overflows. 


By following these practices, you can mitigate the risk of integer overflow vulnerabilities 
and ensure the security and reliability of your software.  
 
 
 
 
 
 
 
 
 