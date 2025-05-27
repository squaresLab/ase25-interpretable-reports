""" Command Line Interface Module. """

import argparse
import os
import pathlib

from secgen import SECGEN
from secgen.parser import ParserFactory


def main():
    dir = os.path.dirname(os.path.abspath(__file__))
    version = {}
    with open(os.path.join(dir, "version.py")) as fp:
        exec(fp.read(), version)

    cli = CLI(version['__version__'])
    cli.config()
    cli.run()


class CLI:
    """ Class for wrapping the CLI interface.
    
    Attributes:
        args: command line arguments
        version: secgen version
    """
    def __init__(self, version):
        self.args = None
        self.version = version
        self.config()

    def config(self):
        parser = argparse.ArgumentParser(description='A convention for making security vulnerability reports more structured and untimely more interpretable.')
        parser.add_argument('--version', '-v', action='version', version='%(prog)s {}'.format(self.version))
        parser.add_argument('--input-file', '-in', action='store', help="Vulnerable program to analyze", required=True, type=pathlib.Path)
        parser.add_argument('--output-file', '-out', action='store', help="Output file to where the report will be written", required=True, type=pathlib.Path)
        parser.add_argument('--model', '-m', action='store', help="Model to be used. Default: gpt-4o-mini (cheapest)", required=False, type=str, default='gpt-4o-mini')
        
        # Static analysis report info
        parser.add_argument('--report', '-r', action='store', help="Path to the static analysis report. Currently, the CLI supports: [sarif, ] ", required=True, type=pathlib.Path)
        # parser.add_argument('--tool', action='store', help="Path to the static analysis binary. Currently, the CLI supports: [codeql binary, ]", required=False, type=pathlib.Path)
        # parser.add_argument('--rules', action='store', help="Path to the set of rules to be used by the static analysis binary.", required=False, type=pathlib.Path)

        # Activate report linter
        parser.add_argument('--quiet', '-q', action='store', help="If true, quiet mode is ON. [default: false]", required=False, default=False)
        self.args = parser.parse_args()


    def run(self):
        input_file: str  = self.args.input_file.as_posix()
        output_file: str = self.args.output_file.as_posix()
        model: str = self.args.model

        report: str = self.args.report.as_posix() 
        quiet: bool = self.args.quiet


        if report:
            # Register available parsers
            print("Registering available parsers...")
            ParserFactory.discover_parsers('secgen.parsers')   
            assert len(ParserFactory._parsers) > 0, "No parsers found. Please check the parsers directory."
            
            secgen = SECGEN()
            secgen.run(code_path=input_file, output_path=output_file, model=model, quiet=quiet, report_path=report)

