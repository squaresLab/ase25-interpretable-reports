import os 
from typing import Dict, List, Optional, Any
from dataclasses import dataclass

from dotenv import load_dotenv
load_dotenv()


@dataclass
class ReportFile:
    """Container for report file information."""
    content: Any  # The actual content of the report
    filename: str  # Original filename with extension

@dataclass
class Vulnerability:
    type: str
    description: str
    rule: str
    location: Dict[str, Any]
    severity:str|None 
    flow: Optional[List[Dict[str, Any]]] = None

@dataclass
class VulnLocation:
    file: str
    start_line: int
    start_column: int
    end_line: int|None
    end_column: int


def get_env_model() -> dict:
    """Get relevant environment variables related to model api key"""
    keys : dict = {}
    keys["api_key"] = os.getenv("API_KEY")
    return keys
    
def read_file(fpath:str) -> str: 
    """Read and     parse report"""
    try:
        with open(fpath, 'r') as f:
            template = f.read()
            return template
    except Exception as exc:
        print (f"An error occurred while reading {fpath}: {exc}")
        return ""
    
def write_file(fpath:str, content:str) -> bool:
    """Write content to file"""
    try:
        with open(fpath, 'w') as f:
            # Remove leading/trailing whitespace/newlines first
            cleaned = content.strip()

            # Then remove backticks
            if cleaned.startswith("```") and cleaned.endswith("```"):
                cleaned = cleaned[3:-3].strip()  # also strip again to remove any newlines inside

            f.write(cleaned)
            return True
    except Exception as exc:
        print (f"😭 An error occurred while writing {fpath}: {exc}")
        return False