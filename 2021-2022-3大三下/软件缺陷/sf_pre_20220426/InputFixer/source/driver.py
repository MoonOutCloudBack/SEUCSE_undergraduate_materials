"""
This file contains the main driver for the InputFixer tool

Run as such: Python3 driver.py config_file.json(optional)
If no configuration file is provided, the default is run, which you probably don't want
Config is in json format. See the readme for information on how to format the config file

"""

import sys

# Make sure I have access to my script functions if I need them
# TODO: Should I insert into the beginning of the path list?
sys.path.append('../scripts')

import json
from inFix_config import Config
from inFix_logger import Logger
from inFix import InFix




def load_configuration(file_name):
    """
    This function loads the configuration as necessary
    """
    if len(sys.argv) < 2:
        print("No configuration file provided. Please provide configuration")
        sys.exit()
    try:
        with open(file_name, 'r') as config_file:
            return Config(json.load(config_file))
    except:
        print("Unable to load provided config file. Terminating program execution.\n")
        sys.exit()

if __name__ == "__main__":

    # Load the configiration file
    inFix_config = load_configuration(sys.argv[-1])
    
    # Get the log file ready
    inFix_log = Logger(inFix_config)

    # Make the inFix object
    input_fixer = InFix(inFix_config, inFix_log)

    # Run the inFix experiments
    results = input_fixer.find_fixes()

    # Analyze the results
    
    # TODO: Modify as needed

    # Log the results
    inFix_log.zip_raw_results()
    inFix_log.finalize_logs()
    
