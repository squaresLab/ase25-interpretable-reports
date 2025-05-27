from abc import ABC, abstractmethod
from typing import Dict, List, Optional, Any
import inspect
import importlib
import pkgutil
import os
from secgen.utils import ReportFile, Vulnerability


class BaseParser(ABC):
    @abstractmethod
    def parse(self, report_content: Any) -> List[Vulnerability]:
        pass
    
    @abstractmethod
    def validate(self, report_file: ReportFile) -> bool:
        """Check if this parser can handle the given report file."""
        pass

class ParserFactory:
    _parsers = []
    
    @classmethod
    def register_parser(cls, parser: BaseParser):
        cls._parsers.append(parser)
    
    @classmethod
    def get_parser(cls, report: ReportFile) -> Optional[BaseParser]:
        for parser in cls._parsers:
            if parser.validate(report):
                return parser
        return None
    
    @classmethod
    def discover_parsers(cls, package_name: str = "secgen.parsers"):
        """
        Automatically discover and register all BaseParser subclasses
        in the specified package.
        """
        try:
            # Import the package
            package = importlib.import_module(package_name)
            
            # Check if it's a package (has __path__ attribute)
            if not hasattr(package, '__path__'):
                print(f"Error: {package_name} is not a package (it's a module)")
                print("Make sure it's a directory with an __init__.py file")
                return
                
            # Find parser classes in all modules within the package
            pkg_path = package.__path__
            prefix = package.__name__ + "."
            
            for _, modname, ispkg in pkgutil.iter_modules(pkg_path, prefix):
                if not ispkg:  # Only process modules, not sub-packages
                    try:
                        module = importlib.import_module(modname)
                        
                        # Find all classes in the module that inherit from BaseParser
                        for _, obj in inspect.getmembers(module, inspect.isclass):
                            if issubclass(obj, BaseParser) and obj != BaseParser:
                                # Create an instance and register it
                                try:
                                    cls.register_parser(obj())
                                    print(f"Registered parser: {obj.__name__}")
                                except Exception as e:
                                    print(f"Failed to register {obj.__name__}: {e}")
                    except ImportError as e:
                        print(f"Could not import module {modname}: {e}")
        except ImportError as e:
            print(f"Could not import package {package_name}: {e}")


def parse_vulnerability_report(parser: BaseParser, report_content: Any, filename:os.PathLike) -> List[Vulnerability]:
    report_file = ReportFile(content=report_content, filename=filename)
    if parser:
        return parser.parse(report_file)
    else:
        raise ValueError(f"Unsupported report format for file: {filename}")