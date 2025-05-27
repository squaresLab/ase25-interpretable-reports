import spacy
import os

class Extractor:
    def __init__(self) -> None:

        # Get the directory of the current module
        base_dir = os.path.dirname(__file__)
        # Construct the full path to the JSON file
        json_path = os.path.join(base_dir, 'entities', 'patterns.jsonl')
       
        self.engine = spacy.load("en_core_web_sm")
        self.engine.remove_pipe("ner")
        self.engine.add_pipe("entity_ruler").from_disk(json_path)
        
    def entities(self, lines : list[tuple]) -> dict:
        """Extract entities per line in block"""
        entities = {}
        
        for (tag, content) in lines:
            entities[tag] = [(ent.text, ent.label_) for ent in self.engine(content).ents]

        return entities