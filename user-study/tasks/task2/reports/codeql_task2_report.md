## Locations and rules (1)

<span style="color:yellow">⚠️</span> 57: May return stack-allocated memory from [```subarrays```](task2_sample.c#L57)

### Info
May return stack-allocated memory from [```subarrays```](task2_sample.c#L57)
- **Rule Id** cpp/return-stack-allocated-memory
- **Rule Name** cpp/return-stack-allocated-memory
- **Rule Description** A function returns a pointer to a stack-allocated region of memory. This memory is deallocated at the end of the function, which may lead the caller to dereference a dangling pointer.
- **Level** warning
- **Kind** --
- **Baseline state** new
- **Locations** [task2_sample.c](task2_sample.c)
- **Log** task2_report.sarif (*For the purpose of this study, this file is not available.*)

### Analysis steps (2)
- ```subarrays``` at task2_sample.c **57:12**
- array to pointer conversion at task2_sample.c **57:12** 

