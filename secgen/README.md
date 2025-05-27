# SECGEN

A tool and convention for creating structured, interpretable security vulnerability reports. SECGEN helps developers and security researchers document vulnerabilities in a standardized and comprehensive format.

## Table of Contents
- [Overview](#overview)
- [Convention Structure](#convention-structure)
- [Installation](#installation)
  - [Prerequisites](#prerequisites)
  - [Installation Steps](#installation-steps)
- [Usage](#usage)
  - [Command-line Arguments](#command-line-arguments)
  - [Example Usage](#example-usage)
- [Citation](#citation)
- [Publications](#publications)
- [License](#license)

## Overview

SECGEN addresses the challenge of creating clear, actionable security vulnerability reports by providing:
- A standardized format for vulnerability documentation
- Structured sections that capture all essential vulnerability information

- Validation tools to ensure report completeness

## Convention Structure

SECGEN breaks down vulnerability reports into five main sections:

### 1. Header Section (`vuln`)
```
vuln: <vulnerability-name-here-with-CWE-info> in <file-name-and-line-numbers> (severity: <level-here>)
```
- Identifies the vulnerability type with CWE reference
- Specifies affected file and line numbers
- Indicates severity level

### 2. Summary Section
```
what: <describe the vulnerability>
where: <locate the vulnerability lines and file>
why: <describe one possible consequence of not resolving this weakness>
how: <explain how an attacker would proceed to exploit this vulnerability>
```
- Provides a clear description of the vulnerability
- Pinpoints exact location in code
- Explains potential consequences
- Explains potential exploitation methods

### 3. Program Analysis Section
```
code-sources: <identify entry points in code where user input enters an application>
code-sinks: <identify actions performed by the application, using user input from a source>
```
- Maps data flow from input sources
- Identifies vulnerable code execution points
- Helps understand attack vectors

### 4. Fix Section
```
suggested-fix: <code diff file showing the necessary code changes to fix the vulnerability>
explanation-suggested-fix: <explain how the suggested code diff resolves the vulnerability>
```
- Provides concrete fix recommendations
- Includes explanations of why the fix works
- Shows before/after code comparisons

### 5. Methodology Section
```
method: <write CODEQL if there is any taint information; say UNKNOWN otherwise>
```
- Documents detection methodology
- Specifies tools and techniques used
- Helps validate findings

## Installation

### Prerequisites

1. **Python Environment**
   - Python v3.9.20 or later
   - pip package manager
   
2. **Ollama**
   - Installation guidelines [here](https://ollama.com/).

3. **OpenAI API Key (Optional)**
   - Required for AI-powered report generation
   - Get your key from [OpenAI's platform](https://platform.openai.com/api-keys)


### Installation Steps

1. **Clone the Repository**
  ```bash
  git clone https://github.com/XXX
  cd interpretable-vulnerability-reports/secgen
  ```

2. **Install Dependencies**
  ```bash
  pip install -r requirements.txt
  ```

3. **Install SECGEN**
  ```bash
  python setup.py install
  ```

4. **Install the linter module (SECGENLint)**
  ```bash
   cd interpretable-vulnerability-reports/secgenlint
   pip install -r requirements.txt
   python setup.py install
  ```

5. **Verify Installation**
  ```bash
   secgen --version  # Should output: secgen 0.0.2
  ```

## Usage

### Command-line Arguments

| Argument | Short | Description | Default |
|----------|-------|-------------|---------|
| `--help` | `-h` | Show help message | - |
| `--version` | `-v` | Show version | - |
| `--input-file` | `-i` | Vulnerable program to analyze | Required |
| `--model` | `-m` | AI model to use | gpt-4o-mini |
| `--output-file` | `-o` | Output report file | Required |
| `--quiet` | `-q` | Enable quiet mode (if false, it will run the linter)| false |

### Example Usage

Here's a complete example analyzing a C program with a use-after-free vulnerability:

1. **Sample Vulnerable Code** (at `samples/sample.c`):
```c
#include <stdio.h>
#include <stdlib.h>

void process(int *ptr) {
    // Note: If 'ptr' is a dangling pointer, this will cause undefined behavior
    printf("Processing value: %d\n", *ptr);
}

void main() {
    int *data = (int *)malloc(sizeof(int)); // Allocate memory 
    
    // (...) Check if memory allocation was successful

    *data = 100;   // Initialize the allocated memory with the value 100
    free(data);    // Free the allocated memory
    
    int *invalid_ptr = data;  // Assign 'data' (dangling pointer) to 'invalid_ptr'
    process(invalid_ptr); // Call 'process' with the invalid pointer
}
```

2. **Run Analysis**
```bash
secgen --input-file sample.c --output-file report.txt --model smollm:latest
```

3. **Generated Report** (at `samples/generated/report.txt`):
```
vuln: use-after-free (CWE-416) in main.c:18 (severity: high)

what: This vulnerability occurs when a program continues to use a pointer after the memory it points to has been freed. This can lead to undefined behavior, including memory corruption and crashes.
where: The vulnerability is located in the main function at line 18 of the source code.
why: If this weakness is not resolved, it could lead to arbitrary code execution, data leaks, or application crashes, as the program might access memory that has been reallocated for other purposes.
how: An attacker could exploit this vulnerability by manipulating the program's control flow to trigger the use of the dangling pointer, potentially leading to the execution of malicious code or the modification of critical data.

code-sources: The entry point for user input in this code is not applicable as there is no direct user input; however, the allocation of memory with malloc could be influenced by external factors.
code-sinks: The action performed by the application using user input from a source is the dereferencing of the invalid pointer in the `process` function.

suggested-fix: 
-    free(data);    // Free the allocated memory
-    int *invalid_ptr = data;  // Assign 'data' (dangling pointer) to 'invalid_ptr'
-    process(invalid_ptr); // Call 'process' with the invalid pointer
+    int *invalid_ptr = data;  // Assign 'data' to 'invalid_ptr'
+    process(invalid_ptr); // Call 'process' with the valid pointer
+    free(data);    // Free the allocated memory after processing

explanation-suggested-fix: The suggested code diff prevents the use of a dangling pointer by ensuring that the `process` function is called before the memory is freed. This ensures that the pointer remains valid when accessed.

method: CODEQL
```

## Citation

If you use SECGEN in your research, please cite:

```bibtex
@misc{SECGENV001,
  author = {TBA},
  title = {{SECGEN v0.0.1}},
  year = {2024},
  publisher = {GitHub},
  url = {}
}
```

For the associated research paper:

```bibtex
@inproceedings{SECGEN2024,
  author = {TBA},
  booktitle = {TBA},
  title = {{TBA}},
  year = {2024}
}
```

## Publications

- TBA


## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
