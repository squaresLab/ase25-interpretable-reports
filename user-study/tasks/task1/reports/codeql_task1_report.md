## Locations and rules (2)

1. <span style="color:yellow">⚠️</span> 34: May return stack-allocated memory from [```english_word```](task1_sample.c#L34)
2. <span style="color:yellow">⚠️</span> 59: May return stack-allocated memory from [```english_sentence```](task1_sample.c#L59)

### Info 1
May return stack-allocated memory from [```english_word```](task1_sample.c#L34)
- **Rule Id** cpp/return-stack-allocated-memory
- **Rule Name** cpp/return-stack-allocated-memory
- **Rule Description** A function returns a pointer to a stack-allocated region of memory. This memory is deallocated at the end of the function, which may lead the caller to dereference a dangling pointer.
- **Level** warning
- **Kind** --
- **Baseline state** new
- **Locations** [task1_sample.c](task1_sample.c)
- **Log** task2_report.sarif (*For the purpose of this study, this file is not available.*)


### Info 2
May return stack-allocated memory from [```english_sentence```](task1_sample.c#L59)
- **Rule Id** cpp/return-stack-allocated-memory
- **Rule Name** cpp/return-stack-allocated-memory
- **Rule Description** A function returns a pointer to a stack-allocated region of memory. This memory is deallocated at the end of the function, which may lead the caller to dereference a dangling pointer.
- **Level** warning
- **Kind** --
- **Baseline state** new
- **Locations** [task1_sample.c](task1_sample.c)
- **Log** task2_report.sarif (*For the purpose of this study, this file is not available.*)


### Analysis steps (2)
1.  - ```english_word``` at task1_sample.c **34:12**
    - array to pointer conversion at task2_sample.c **34:12** 

2. - ```english_sentence``` at task1_sample.c **59:12**
    - array to pointer conversion at task2_sample.c **59:12** 

