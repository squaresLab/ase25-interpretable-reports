import os
import time
from secgen.analyser import Analyser, load_analyser
from secgen.generator import Generator, Router, improve_report
from secgen.utils import ReportFile, Vulnerability
from secgen.parser import ParserFactory, parse_vulnerability_report
from secgen.linter import linter
from pathlib import Path

from dotenv import load_dotenv
load_dotenv()

def read_report(report:str|Path) -> str:
    """
    Read the report file and return the content.
    """
    with open(report, "r") as f:
        content = f.read()
    
    assert content is not None, f"Failed to read report {report}."
    return content

def main(code_path:str|Path, output_path:str, model:str, report_path:str, quiet:bool):
    """ Run the parsers through the reports and collect information regarding vulnerabilities. """        
    parser = ParserFactory.get_parser(ReportFile(content=None, filename=report_path))
    assert parser is not None, f"No parser found for {report_path}. Report is not supported."

    # Read report content
    content:str = read_report(report_path)

    # Parse report and extract vulnerabilities
    vulnerabilities = parse_vulnerability_report(parser, content, report_path)
    assert len(vulnerabilities) > 0, f"There are no vulnerabilities to report."

    # Improve the report
    results = improve_report(code_path=code_path, vulnerabilities=vulnerabilities, model=model, output_path=output_path)
    
    # TODO: save them in md, json or text

    if quiet==False:
        linter(results['report_path'], show_score=True, quiet=False, out=None)


if __name__ == '__main__':
    main()
    
    
