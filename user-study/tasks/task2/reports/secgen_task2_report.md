### **[ vuln-id ]**  cpp/return-stack-allocated-memory in maxSubarraySum (severity: <span style="color:red">high</span>)

<span style="color:magenta">**[ what ]**</span> This vulnerability occurs when a function returns a pointer to a stack-allocated region of memory, which is deallocated at the end of the function. This may lead the caller to dereference a dangling pointer.

<span style="color:magenta">**[ where ]**</span> [Line 57](task2_sample.c#L57) in  [```maxSubarraySum```](task2_sample.c#L23).

<span style="color:magenta">**[ why ]**</span>  If this weakness is not resolved, it may lead to undefined behavior or program crashes when the caller attempts to access the deallocated memory. This could result in system instability or data corruption.

<span style="color:magenta">**[ how ]**</span> : An attacker could exploit this vulnerability by manipulating inputs to the function, causing it to return and use the now invalid pointer, potentially injecting malformed data or causing a crash.

<span style="color:brown">**[ code-sources ]**</span> [```arr```](task2_sample.c#L69) in [```main```](task2_sample.c#L67) function, as it is input for [```maxSubarraySum```](task2_sample.c#L23).


<span style="color:brown">**[ code-sinks ]**</span> Return value of [```maxSubarraySum```](task2_sample.c#L23) which is used by [```printSubarrays```](task2_sample.c#L61).

<span style="color:green">**[ suggested-fix-diff ]**</span> 
```diff
 @@ -1,8 +1,3 @@

-// Task 2
-/*
- * A greedy algorithm to find the maximum sum of non-overlapping subarrays with given constraints
- * Time complexity: O(n)
- */
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

@@ -21,12 +16,16 @@
 
 // Function to find the maximum sum of non-overlapping subarrays
 Subarray* maxSubarraySum(int* arr, int n, int m) {
+    // Allocate memory for the subarrays array dynamically
+    Subarray* subarrays = (Subarray*)malloc(MAX_SUBARRAYS * sizeof(Subarray));
+    if (subarrays == NULL) {
+        perror("Failed to allocate memory");
+        exit(EXIT_FAILURE);
+    }
     // Initialize the maximum sum to 0
     int max_sum = 0;
     // Initialize the number of subarrays to 0
     int num_subarrays = 0;
-    // Initialize the subarrays array
-    Subarray subarrays[MAX_SUBARRAYS];
     // Initialize the current subarray
     Subarray curr_subarray;
     curr_subarray.start = 0;

@@ -71,5 +70,7 @@
     Subarray* subarrays = maxSubarraySum(arr, 10, 3);
     // Print the subarrays array
     printSubarrays(subarrays, 3);
+    // Free the allocated memory
+    free(subarrays);
     return 0;
 }

```


<span style="color:green">**[ explanation-suggested-fix-diff ]**</span> 
The suggested code fix allocates memory for the ```subarrays``` array dynamically using `malloc`. This ensures that the memory remains valid after the function `maxSubarraySum` returns. Additionally, the `main` function now frees the allocated memory using `free` to prevent memory leaks.

<span style="color:purple">**[ method ]**</span>  CODEQL and GPT-4.o
