## Locations and rules (2)

1. <span style="color:yellow">⚠️</span> 21: May return stack-allocated memory from [```c```](task3_sample.c#L21)
2. <span style="color:yellow">⚠️</span> 23: May return stack-allocated memory from [```c```](task3_sample.c#L23)

### Info 1
May return stack-allocated memory from [```c```](task3_sample.c#L21)
- **Rule Id** cpp/return-stack-allocated-memory
- **Rule Name** cpp/return-stack-allocated-memory
- **Rule Description** A function returns a pointer to a stack-allocated region of memory. This memory is deallocated at the end of the function, which may lead the caller to dereference a dangling pointer.
- **Level** warning
- **Kind** --
- **Baseline state** new
- **Locations** [task3_sample.c](task3_sample.c)
- **Log** task3_report.sarif (*For the purpose of this study, this file is not available.*)


### Info 2
May return stack-allocated memory from [```c```](task3_sample.c#L23)
- **Rule Id** cpp/return-stack-allocated-memory
- **Rule Name** cpp/return-stack-allocated-memory
- **Rule Description** A function returns a pointer to a stack-allocated region of memory. This memory is deallocated at the end of the function, which may lead the caller to dereference a dangling pointer.
- **Level** warning
- **Kind** --
- **Baseline state** new
- **Locations** [task3_sample.c](task3_sample.c)
- **Log** task3_report.sarif (*For the purpose of this study, this file is not available.*)


### Analysis steps (2)
1.  - ```c``` at task3_sample.c **21:17**
    - ```& ...``` at task3_sample.c **21:16**  

1.  - ```c``` at task3_sample.c **23:17**
    - ```& ...``` at task3_sample.c **23:16**  