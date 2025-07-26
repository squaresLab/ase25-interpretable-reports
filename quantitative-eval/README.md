# Quantitative Evaluation

This directory contains the quantitative evaluation data for measuring the effectiveness of our interpretable vulnerability report generation approach.

## Evaluation Overview

We conducted a comprehensive quantitative evaluation focusing on two critical weakness types:

- **CWE-416**: Use After Free vulnerabilities
- **CWE-787**: Out-of-bounds Write vulnerabilities

## Evaluation Metrics

Our evaluation measured three key aspects:

### 1. Report Compliance
Evaluation of how well generated vulnerability reports adhere to our interpretability standards and structured format requirements.

### 2. Patch Success
Assessment of the quality and correctness of automatically generated patches for fixing identified vulnerabilities.

### 3. Applicability
Analysis of how successfully our approach can be applied to different vulnerability instances and code patterns.

## Dataset Structure

### CWE-416 Dataset (`416/`)
- **416.csv**: Main evaluation results for 416 vulnerability samples
- **applicability/**: Applicability analysis results
- **compliance/**: Report compliance evaluation scores
- **orig_samples/**: Original vulnerable code samples
- **patch/**: Generated patches for vulnerability fixes
- **reports/**: Generated interpretable vulnerability reports

### CWE-787 Dataset (`787/`)
- **787.csv**: Main evaluation results for 787 vulnerability samples
- **applicability/**: Applicability analysis results
- **compliance/**: Report compliance evaluation scores
- **orig_samples/**: Original vulnerable code samples
- **patch/**: Generated patches for vulnerability fixes
- **reports/**: Generated interpretable vulnerability reports

## Data Description

Each dataset contains:
- **Original samples**: C programs containing the respective CWE vulnerabilities
- **Generated reports**: Structured vulnerability reports created by our SecGen tool
- **Patches**: Automatic fixes generated for the identified vulnerabilities
- **Evaluation metrics**: Compliance scores, applicability results, and success rates

The evaluation demonstrates the effectiveness of our structured approach to vulnerability report generation across different weakness types and provides quantitative evidence for the improvements in interpretability and actionability of security reports.

## Results Summary

| Vulnerability | Total Samples | Applicable Patches | Successful Patches | Average Compliance Score |
|---------------|---------------|--------------------|--------------------|--------------------------|
| CWE-416       | 652           | 277 (42.5%)        | 199 (30.5%)        | 0.8329 (σ=0.0467)       |
| CWE-787       | 68            | 28 (41.2%)         | 8 (11.8%)          | 0.8304 (σ=0.0468)       |
| **Total**     | **720**       | **305 (42.4%)**    | **207 (28.8%)**    | **0.8317 (σ=0.0468)**   |

*Results for patch applicability, patch success, and compliance scores. We report total patch count for the first two measurements and average compliance scores with standard deviation (σ) in parentheses.* 