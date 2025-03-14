### <span style="color:blue">**[ vuln-id ]**</span> cpp/return-stack-allocated-memory in sanitize (severity: <span style="color:red">high</span>)

<span style="color:magenta">**[ what ]**</span> The function [`sanitize_char`](task3_sample.c#L20) returns a pointer to a stack-allocated memory location.

<span style="color:magenta">**[ where ]**</span> The vulnerability is located at [line 23](task3_sample.c#L23).

<span style="color:magenta">**[ why ]**</span>  This can result in undefined behavior or crashes since the memory location is deallocated at the end of the function, leading the caller to dereference a dangling pointer.

<span style="color:magenta">**[ how ]**</span> : An attacker can exploit this vulnerability by crafting input that causes the program to dereference a dangling pointer, potentially leading to crashes or execution of arbitrary code.


<span style="color:brown">**[ code-sources ]**</span> The entry points in the code is [```url```](task3_sample.c#L64) in [```main```](task3_sample.c#L63) function, as it is input for [```sanitize_url```](task2_sample.c#L36).


<span style="color:brown">**[ code-sinks ]**</span> The actions performed by the application using ```url``` occur in the [`sanitize_url`](task3_sample.c#L36) function and its call to [`sanitize_char`](task3_sample.c#L20).

<span style="color:green">**[ suggested-fix-diff ]**</span> 
```diff
@@ -1,10 +1,10 @@
-char* sanitize_char(char c) {
+char sanitize_char(char c) {
     if (isalnum(c)) {
-        return &c;
+        return c;
     } else if (c == '.' || c == '-' || c == '_') {
-        return &c;
+        return c;
     } else {
-        return NULL;
+        return '\0';
     }
 }
 
@@ -16,15 +16,15 @@
         return NULL;
     }
 
-    for (i = 0, j = 0; url[i]!= '\0'; i++) {
-        char* sanitized_char = sanitize_char(url[i]);
+    for (i = 0, j = 0; url[i] != '\0'; i++) {
+        char sanitized_char = sanitize_char(url[i]);
 
-        if (sanitized_char!= NULL) {
-            sanitized_url[j++] = *sanitized_char;
+        if (sanitized_char != '\0') {
+            sanitized_url[j++] = sanitized_char;
         }
     }
 
     sanitized_url[j] = '\0';
 
     return sanitized_url;
-}
+}
```


<span style="color:green">**[ explanation-suggested-fix-diff ]**</span> 
The suggested fix changes the return type of `sanitize_char` to `char` instead of `char*` and returns characters directly rather than pointers to stack-allocated characters. This prevents returning pointers to stack-allocated memory, thus avoiding the risk of dereferencing dangling pointers.

<span style="color:purple">**[ method ]**</span>  CODEQL and GPT-4.o
