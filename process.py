import sys
import os
import traceback

def process(target, sources):

    harvester_path = os.path.abspath('./theHarvester')
    if os.path.exists(harvester_path):
        sys.path.append(harvester_path)
    else:
        print(f'Error: theHarvester directory does not exist at {harvester_path}.')
        return 'Error: theHarvester directory not found.'
    # Import and run theHarvester from theHarvester.py
    try:
        from theHarvester import theHarvester
    except ImportError as e:
        print(f'ImportError: {e}')
        return 'Error: Could not import theHarvester module.'

    # Redirect stdout to capture output
    import io
    output = io.StringIO()
    sys.stdout = output
    sys.stderr = output

    try:
        # Simulate how theHarvester handles command-line arguments
        sys.argv = ['theHarvester', '-d', target, '-b', sources]
        # Run theHarvester
        theHarvester.main()
    except Exception as e:
        # Capture the traceback
        traceback.print_exc()
        print(f'An error occurred: {e}')
    finally:
        # Get the captured output and reset stdout
        sys.stdout = sys.__stdout__
        sys.stderr = sys.__stderr__
    return output.getvalue()

if __name__ == '__main__':

    result = process("fox.com", "threatminer")
    print(result)

