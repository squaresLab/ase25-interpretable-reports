# SECGENLint

A linting tool that validates security vulnerability reports against the [SECGEN](https://github.com/TQRG/secgen) convention, ensuring they are structured, complete, and interpretable.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
  - [Prerequisites](#prerequisites)
  - [Installation Steps](#installation-steps)
- [Usage](#usage)
  - [Command-line Arguments](#command-line-arguments)
  - [Example Usage](#example-usage)
  - [Compliance Rules](#compliance-rules)
- [Citation](#citation)
- [Publications](#publications)
- [License](#license)

## Overview

SECGENLint helps security researchers and developers ensure their vulnerability reports follow best practices by:
- Validating report structure and completeness
- Checking for required sections and tags
- Measuring compliance against interpretability standards
- Providing actionable feedback for improvements

## Features

SECGENLint validates reports against the following criteria:
- **Header Section Validation**
  - Presence of vulnerability type and CWE info
  - File location and line number verification
  - Severity level checks

- **Summary Section Checks**
  - Completeness of what/where/why/how explanations
  - Length and content quality validation
  - Clear vulnerability description verification

- **Program Analysis Validation**
  - Source and sink identification
  - Data flow documentation checks
  - Code location accuracy

- **Fix Section Requirements**
  - Presence of concrete fix suggestions
  - Quality of fix explanations
  - Code diff format validation

- **Methodology Documentation**
  - Detection method verification
  - Tool usage documentation
  - Analysis approach validation

## Installation

### Prerequisites

1. **Python Environment**
   - Python v3.9.20 or later
   - pip package manager

2. **SECGEN**
   - Required for generating reports
   - Follow installation instructions at [SECGEN repository](https://github.com/TQRG/secgen)

### Installation Steps

1. **Clone the Repository**
   ```bash
   git clone https://github.com/XXX
   cd secgenlint
   ```

2. **Install Dependencies**
   ```bash
   pip install -r requirements.txt
   ```

3. **Install SECGENLint**
   ```bash
   python setup.py install
   ```

## Usage

### Command-line Arguments

| Argument | Short | Description | Default |
|----------|-------|-------------|---------|
| `--help` | `-h` | Show help message | - |
| `--version` | `-v` | Show version | - |
| `--input-file` | `-i` | Report file to validate | Required |
| `--output-file` | `-o` | Compliance report output | Required |

### Example Usage

1. **Sample Vulnerability Report** (`report.txt`):
```
vuln: Use-After-Free (CWE-416) in sample.c lines 11 (severity: High) 

what: The vulnerability arises when the code tries to access memory through a pointer that has been previously freed, resulting in undefined behavior.
where: The vulnerability is located at lines 11 in the file sample.c.
why: If not resolved, this weakness can lead to a variety of problems, including crashes, unexpected behavior, or even code execution by an attacker.
how: An attacker could exploit this vulnerability by manipulating the pointer to access freed memory, potentially leading to memory corruption or arbitrary code execution.

code-sources: The code source is the assignment of `data` and misuse assigning `data` to `invalid_ptr` after `freeMemory(&data)` is called.
code-sinks: The problematic action occurs when `process(invalid_ptr)` is called, which tries to dereference a pointer that points to freed memory.

suggested-fix:
--- a/sample.c
+++ b/fixed_sample.c
@@ -7,8 +7,11 @@ void freeMemory(int **ptr) {
 }
 
 void process(int *ptr) {
-    // Simulate some processing without realizing ptr was freed
-    printf("Processing value: %d\n", *ptr);
+    if (ptr != NULL) {  // Check if the pointer is NULL before accessing
+        printf("Processing value: %d", *ptr);
+    } else {
+        fprintf(stderr, "Error: Trying to access memory through a NULL pointer.");
+    }
 }

explanation-suggested-fix: The suggested fix introduces a check in the `process` function to ensure that it does not attempt to access memory through a NULL pointer. This prevents the program from attempting to dereference a pointer that has been freed and set to NULL, thereby avoiding the use-after-free vulnerability.
```

2. **Run Validation**
```bash
secgenlint --input-file report.txt --output-file compliance-report.txt
```

3. **Compliance Report** (`compliance-report.txt`):
```
❌ Report is missing the ‹method> tag. [reporter_has_method]
⚠️ Summary has more than 500 chars. [summary_max_length]
✅ Header is not empty. [header_is_not_empty]
✅ Header mentions a weakness (CWE/BF) id. [header_has_weakness]
✅ Header indicates severity. [header_has_severity]
✅ Explanation gives information regarding sources. [explanation_has_sources]
✅ Explanation gives information regarding sinks. [explanation_has_sinks]
✅ There is a suggested fix. [fix_is_not_empty]
✅ Suggested fix has actionable items. [fix_has_action]

Found 1 error(s), 1 warning(s)
Report is 86.67% in compliance with interpretability standard.
```


### Compliance Rules
Next, we describe rules and entities. 

## Complete set of entities and their descriptions
| Entity             | Description                                                                                                               | Example                                                                                             |
|--------------------|---------------------------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------|
| CWE ID             | Identifier of the type of software vulnerability according to the CWE standard.                    | CWE-416 (Use After Free)     |
| SEVERITY           | The assessed criticality level of the detected vulnerability.                                      |High, Moderate Low            |
| SECWORD            | Specific security-relevant terminology or keywords identified in prior research as important to understanding the issue.   | Use After Free, Dangling pointer |
| ACTION             | The type of modification or action required to address the identified vulnerability.                                       | Allocating \[heap memory using `malloc`\]|
| TOOLING            | Tools used for static analysis and report generation.                                                                      | CodeQL, OpenAI GPT-4o                                                                               |
| LOCATION           | Program location. Typically in the format <file:line> or <file:function>, it is where the vulnerability is found.        | Function `generate_pwd`:7                                                                     |
| CODE               | Code segments that are either identified as vulnerable or provided as suggested fixes.                                    | `char pwd[PWD_LEN+1];`                                                                              |
| IMPACT             | Consequences of not addressing the vulnerability, describing potential security threats.                                   | Arbitrary code execution, crashes, memory corruption                                                |



## Complete set of guideline rules

### Summary
Let 𝑆 be the set of words in the summary section. The summary is compliant if:

$(∃a∈S:type(a)∈{CWEID, SECWORD})∧(∃b∈S:type(b)=LOCATION)∧(∃c∈S:type(c)=IMPACT)∧(∃d∈S:type(d)=ACTION)$

### Program Analysis section
N/A.

### Fix suggestion
Let 𝐹 be the text and entities in the fix suggestion. Compliance with the convention is evaluated as:

$(∃x∈F:type(x)=CODE)∧(∃y∈F:type(y)=ACTION)∧(∃z∈F:type(z)∈{SECWORD,CWEID})$

### Methodology
Let 𝑀 represent the words/entities in the methodology section. A compliant methodology section satisfies:

$(∃x∈M:type(x)=TOOLING)$


## Citation

If you use SECGENLint in your research, please cite:

```bibtex
@misc{SECGENLintV001,
  author = {TBA},
  title = {{SECGENLint v0.0.1}}ßßß,
  year = {2024},
  publisher = {GitHub},
  url = {https://github.com/}
}
```

For the associated research paper:

```bibtex
@inproceedings{SECGENLint2024,
  author = {TBA},
  booktitle = {TBA},
  title = {{TBA}},
  year = {2024}
}
```

## Publications

- TBA
- [Google Scholar Referß
## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
