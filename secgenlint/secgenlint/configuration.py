import os
import yaml
from typing import Optional

class Config:
    """Configuration manager for secgenlint.
    
    This class handles loading and managing configuration settings from YAML files,
    particularly the rules configuration used for linting security reports.
    
    Attributes:
        rules_config_path (str): Path to the rules configuration file
        default_rules (dict): Loaded rules configuration
    """
    
    def __init__(self, path: str = "config/rules.yml", openai_key: Optional[str] = None) -> None:
        """Initialize the configuration manager.
        
        Args:
            path (str): Path to the rules configuration file, relative to the package
            openai_key (Optional[str]): OpenAI API key if needed
        
        Raises:
            FileNotFoundError: If the rules configuration file cannot be found
            yaml.YAMLError: If the rules configuration file is not valid YAML
        """
        def read_config(file_path: str) -> dict:
            try:
                with open(file_path, "r") as fin:
                    return yaml.safe_load(fin)
            except FileNotFoundError:
                raise FileNotFoundError(f"Rules configuration file not found at: {file_path}")
            except yaml.YAMLError as e:
                raise yaml.YAMLError(f"Invalid YAML in rules configuration: {e}")
            
        # Resolve the rules config path relative to this file's location
        package_dir = os.path.dirname(os.path.abspath(__file__))
        self.rules_config_path = os.path.join(package_dir, path)
        self.default_rules = read_config(self.rules_config_path)
        
        # Store OpenAI key if provided
        self.openai_key = openai_key

    # def save_key(self):
    #     with open(self.openai_key_config_path, 'w') as f:
    #         f.write(f"OPENAI_KEY={self.openai_key}")
            
    # def read_key(self):
    #     with open(self.openai_key_config_path) as f:
    #         self.openai_key = f.readlines()[0].split('=')[1]

