from secgenlint.main import main

def linter(report:str, show_score:bool=True, quiet:bool=False, out:str=None):
    """Run the linter on the provided report."""
    main(report, show_score, quiet, out)
