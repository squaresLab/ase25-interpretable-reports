""" Main entry point to start using SECGEN. """
import os 

from secgen.main import main
from secgen.parser import ParserFactory, parse_vulnerability_report
from secgen.generator import improve_report


class SECGEN:
    """Run SECGEN with the provided arguments."""

    def run(self, code_path:str, output_path:str, report_path:str, model:str, quiet:bool) -> None:
        main(code_path=code_path, output_path=output_path, model=model, report_path=report_path, quiet=quiet)

        
