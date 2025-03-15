# Supplementary Material: "Interpretable Vulnerability Detection Reports"

This package contains the user study instruments and prototypes. It is divided into two main components:
- **secgen**: The prototype that generates reports along with a linter module.
- **user-study**: Materials and data collected for the user study.

## Folder Structure
```plaintext
├── secgen
│   ├── secgen.zip            # Zip file for secgen prototype
│   ├── secgenlint.zip        # Zip file for linter module
│
└── user-study
    ├── analysis              # Contains regression data and the regression analysis script
    │   ├── regression_data.numbers  # Regression data file (update file names as needed)
    │   └── analysis.Rmd      # Script to run the regression analysis
    │
    ├── surveys               # Contains survey instruments in PDF format
    │   ├── prestudy_survey.pdf  # Pre-study survey instrument (for eligibility)
    │   └── main_survey.pdf      # Main survey instrument
    │
    └── tasks                 # Contains code samples used in the tasks and all generated reports
        ├── task1             # Folder with code samples and reports for task1
        ├── task2             # Folder with code samples and reports for task2
        └── task3             # Folder with code samples and reports for task3
```


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