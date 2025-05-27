# secgen/parsers/sarif_parser.py
from secgen.parser import BaseParser
from secgen.utils import Vulnerability, VulnLocation, ReportFile
from typing import List
import os
import json

class SarifParser(BaseParser):
    @property
    def supported_extensions(self) -> List[str]:
        return ['.sarif', '.json']
    
    def validate(self, report_file: ReportFile) -> bool:
        """
        Validate if this parser can handle the given file.
        
        Args:
            report_file: The report file to validate
            
        Returns:
            True if this parser can handle the file, False otherwise
        """
        # Check file extension
        _, ext = os.path.splitext(report_file.filename)
        
        # Check if extension matches supported extensions
        return ext in self.supported_extensions
    

    def parse(self, report_file: ReportFile) -> List[Vulnerability]:
        vulnerabilities:list[Vulnerability]= []
        content:dict = json.loads(report_file.content)  
        
        # # SARIF parsing logic
        for run in content.get("runs", []):
            for result in run.get("results", []):
                # Extract vulnerability information
                rule_id = result.get("ruleId")
                rule = next((obj for obj in run['tool']['driver']['rules'] if obj.get("id") == rule_id), None)
                vuln_locations = result.get("locations", []) 
                # TODO : extract flow information

                try: 
                    severity = rule['properties']['security-severity'] 
                except Exception as e:
                    severity = rule['properties']['problem.severity']

                for loc in vuln_locations:
                    vul:Vulnerability = Vulnerability(
                        type=rule_id,
                        description=rule['fullDescription']['text'],
                        location=VulnLocation(
                            file=loc['physicalLocation']['artifactLocation']['uri'],
                            start_line=loc['physicalLocation']['region']['startLine'],
                            end_line=None, # Sarif reports dont have endLine
                            start_column=loc['physicalLocation']['region']['startColumn'],
                            end_column=loc['physicalLocation']['region']['endColumn']
                        ),
                        flow=None,
                        severity=severity,
                        rule=rule,
                    )
                    vulnerabilities.append(vul)
                
        return vulnerabilities
    
    # Implementation of helper methods...