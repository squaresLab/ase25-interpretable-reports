import os
import tempfile
from abc import ABC, abstractmethod
from pysarif import load_from_file

class Analyser(ABC):
    @abstractmethod
    def analyse(self, input: str, quiet:bool) -> str:
        pass

class CodeQL(Analyser):
    def __init__(self) -> None:
        super().__init__
        self.codeql = os.environ["CODEQL_HOME"]
        self.suites = os.environ["CODEQL_SUITES"]

    # Run and parse codeql results
    def process_results(self, report_path:str) -> tuple :
        if not os.path.isfile(report_path): return ""
        report = load_from_file(report_path)

        rules = {rule.id: rule for rule in report.runs[0].tool.driver.rules}
        found_vulnerabilities = {result.rule.id : result for result in report.runs[0].results}

        content = ""
        for id, value in found_vulnerabilities.items():
            full_description = rules[id].full_description.text
            related_cwes = [cwe.split("/")[-1] for cwe in rules[id].properties.tags if cwe.startswith("external/cwe/")]
            line = value.locations[0].physical_location.region.start_line

            content += f"[ {id} in line {line} ({full_description}). {id} may be related to {related_cwes} ]\n"

        return content, found_vulnerabilities.keys()
            

    def analyse(self, input: str, quiet:bool) -> tuple:
        # Create temporary directory to hold CodeQL db
        tmp_dir = tempfile.TemporaryDirectory()

        # Store database, reports and compilables
        db = f"{tmp_dir.name}/database"
        output_report = f"{tmp_dir.name}/report.sarif"
        output_compilable = f"{tmp_dir.name}/compilable"
        return_output, return_found_vars = "", []

        if not os.path.exists(db):
            os.mkdir(db)
        
        p = os.system(f"{self.codeql}/codeql database create {db} --command=\"gcc {input} -o {output_compilable}\" --language=cpp --overwrite")

        if p == 0: # Success
            try:
                p = os.system(f"{self.codeql}/codeql database analyze {db} {self.suites} --format=sarif-latest --output={output_report}")
                if p == 0: # success
                    return_output, return_found_vars = self.process_results(report_path=output_report)
                    
                    if not quiet:
                        print(f"CodeQL reports: \n {return_output}")
            except:
                print("Failed analyzing database.")

        tmp_dir.cleanup()
        return (return_output, return_found_vars)


def load_analyser(command:str) -> Analyser:
    if command == "codeql":
        return CodeQL()
    else:
        raise ValueError(f"Analyzer {command} is not supported. Please provide a valid analyzer.")
