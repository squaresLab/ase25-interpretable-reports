import os
from abc import ABC, abstractmethod
import ollama
from abc import ABC, abstractmethod

from secgen.utils import read_file, Vulnerability, get_env_model, write_file


class Generator(ABC):
    def __init__(self, api_key:str, model_name:str) -> None:
        self.api_key = api_key
        self.model_name = model_name
        self.is_ready = False

        self.max_attempts = 3
        self.retries = 0

        self.template = """
            vuln: <vulnerability-name-here-with-CWE-info> in <file-name-and-line-numbers> (severity: <level-here>)

            what: <describe the vulnerability>
            where: <locate the vulnerability lines and file>
            why: <describe one possible consequence of not resolving this weakness>
            how: <explain how an attacker would proceed to exploit this vulnerability>

            code-sources: <identify entry points in code where user input enters an application>
            code-sinks: <identify actions performed by the application, using user input from a source>

            suggested-fix: <code diff file showing the necessary code changes to fix the vulnerability while preserving code behaviour>
            explanation-suggested-fix: <explain how the suggested code diff resolves the vulnerability>

            method: <write CODEQL if there is any taint information; say UNKNOWN otherwise>"""
                
        self.system_instruction = f"""You are a software security developer expert in vulnerability detection. Your job is to fill-in fields of a security report based on the given vulnerable code, auxiliary information and your knowledge. Preserve the structure of the report and not add additional fields to it. Your response can only include the report body. The template is the following: ```{self.template}```"""
        self.temperature = 0.6

    @staticmethod
    @abstractmethod
    def supports(self, model:str) -> bool:
        pass

    @abstractmethod
    def generate_report(self, input:str, save_to:str, program_analysis:list[Vulnerability]) -> tuple[dict,bool]:
        pass

    def build_prompt(self, filename:str, code:str, program_analysis:list[Vulnerability]) -> dict:
        prompt = {}
        prompt["body"] = {}
        prompt["body"]["messages"] = []

        prompt["body"]["messages"].append({
                "role": "system", "content": self.system_instruction})

        prompt_user_content = f"""The vulnerable code is the followign: ```{code}``` \n"""
        if program_analysis is not None:
            prompt_user_content += "We used CodeQL to identify the following vulnerabilities: \n"
            for id, vuln in enumerate(program_analysis):
                prompt_user_content += f"""Vuln number {id}: {vuln.type} which is defined as {vuln.description}. It has severity level <{vuln.severity}>. It was located in {vuln.location.file}, starting at line {vuln.location.start_line}, column {vuln.location.start_column}.\n"""
    
        prompt["body"]["messages"].append({"role": "user", "content": prompt_user_content})
        
        return prompt



class OllamaGenerator(Generator):
    def __init__(self, api_key:str, model_name:str) -> int:
        super().__init__(api_key, model_name)
        
    @staticmethod
    def supports(model_name: str) -> bool:
        models = ollama.list() # see if installation succeeded
        all_names = [m.model for m in models['models']]

        if model_name in all_names:
            return True
        else:
            # Ask the user if he wants to install the model
            answer = input("Do you want to install the model? (y/n): ")
            if answer == "y":
                ollama.pull(model_name)
                return True
            else:
                print(f"Install the model < {model_name} > from Ollama (in the terminal) and comeback :) .") 
                return False

    def generate_report(self, input: str, save_to: str, program_analysis:list[Vulnerability]) -> dict:
        result:dict= {
            "report_body": None,
            "report_path": None
        }
    
        filename = os.path.basename(input)
        code = read_file(input)
        prompt = self.build_prompt(filename, code, program_analysis) 

        try:
            print("🧠 Sending prompt to model... This may take a while! (~20s)")
            response = ollama.chat(
                model='llama3.2',
                messages=[
                    prompt["body"]["messages"][0], #system
                    prompt["body"]["messages"][1]  #user
                    ],
                stream=False
            )
        except Exception as e:
            print("😭 Error sending prompt to model.", e)
            return {}
        

    
        # Save report to file
        write_file(save_to, response['message']['content'])

        return {
            "report_body": response['message']['content'],
            "report_path": save_to
        }


class OpenAIGenerator(Generator):
    _openai_models = {
        "gpt-4o", "gpt-4o-2024-08-06", "gpt-4o-2024-11-20", "gpt-4o-2024-05-13",
        "gpt-4o-audio-preview", "gpt-4o-audio-preview-2024-12-17", "gpt-4o-audio-preview-2024-10-01",
        "gpt-4o-realtime-preview", "gpt-4o-realtime-preview-2024-12-17", "gpt-4o-realtime-preview-2024-10-01",
        "gpt-4o-mini", "gpt-4o-mini-2024-07-18", "gpt-4o-mini-audio-preview",
        "gpt-4o-mini-audio-preview-2024-12-17", "gpt-4o-mini-realtime-preview",
        "gpt-4o-mini-realtime-preview-2024-12-17", "o1", "o1-2024-12-17", "o1-preview-2024-09-12",
        "o3-mini", "o3-mini-2025-01-31", "o1-mini", "o1-mini-2024-09-12"
    }
    
    def __init__(self, api_key:str, model_name:str) -> None:
        super().__init__(api_key, model_name)
        
        self.client = openai.OpenAI(
            api_key=api_key,
        )
    
    @staticmethod
    def supports(model_name: str) -> bool:
        return model_name in OpenAIGenerator._openai_models

    def generate_report(self, input:str, program_analysis:list[Vulnerability], save_to: str) -> dict:
        result:dict= {
            "report_body": None,
            "report_path": None
        }
    
        filename = os.path.basename(input)
        code = read_file(input)

        prompt = self.build_prompt(filename, code, program_analysis) 

        # Send prompt
        try:
            completion = self.client.chat.completions.create(
                model = self.model_name,
                messages = prompt["body"]["messages"],
                temperature = self.temperature
            )
        except:
            print("Error sending prompt to model.")
            return result

        write_file(save_to, completion.choices[0].message.content)
        return {
            "report_body": completion.choices[0].message.content,
            "report_path": save_to
        }

  
class Router:
    """Routes requests to the appropriate generator based on model name"""

    @staticmethod
    def get_generator(model_name: str, api_key: str) -> Generator:
        """Returns the best generator for a given model."""
        # Try OpenAI first
        if OpenAIGenerator.supports(model_name):
            return OpenAIGenerator(model_name, api_key)
        
        # Fall back to Ollama
        if OllamaGenerator.supports(model_name):
            return OllamaGenerator(model_name, api_key)
                
        # If neither supports the model, raise an error
        raise ValueError(f"No generator found for model: {model_name}")


def improve_report(code_path:os.PathLike, vulnerabilities:list[Vulnerability], model:str, output_path:os.PathLike) -> dict|str:
        """
            Improve the report by adding more information to it. Save in output format.
        """
        api_key = get_env_model()['api_key']
        model = Router.get_generator(model, api_key)

        response = model.generate_report(input=code_path, program_analysis=vulnerabilities, save_to=output_path)
        assert response["report_body"] is not None, "😭 Model response is empty."

        return {
            "report_body": response["report_body"],
            "report_path": response["report_path"],
            "model": model.model_name
        }