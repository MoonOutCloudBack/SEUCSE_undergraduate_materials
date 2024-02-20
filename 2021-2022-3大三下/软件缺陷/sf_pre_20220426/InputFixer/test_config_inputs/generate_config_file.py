"""
This file helps the user generate configuration files for InFix
"""
import json


### ADD FUNCTIONS FOR CONFIGURING CUSTOM THEORIES HERE ###


### BUILT IN THEORIES ###

def configure_gInFix():
    """
    configures the built in genProg like theory
    """
    gInFix_dict = {}
    print("Now it is time to configure gInFix specifically:")
    gInFix_dict["MaxNumFix"] = int(input("Enter the maximum number of fixes to find for each session: "))
    gInFix_dict["TimeOut"] = float(input("Enter the maximum number of seconds to try and find the fixes: "))
    return gInFix_dict



### OVERALL CONFIGURATION ###

print("Welcome to the inFix configuration generator.")

config_dict = {}

filename = input("Enter the config file name: ")

config_dict["SessionsFolderPath"] = input("Enter the path /path/to/SessionMetaFolder: ")
config_dict["LogFolderPath"] = input("Enter the path to /path/to/LogFolderName: ")

response = input("Do you want to try and fix the inputs where the last line was empty? [Y/N] ")
config_dict["FixEmptyLastInputs"] = (response == "Y" or response == "y")

theory = " "
config_dict["Theories"] = []
while len(theory) > 0:
    theory = input("Enter a theory you want to test. If there are no more, press enter: ")
    if len(theory) > 0:
        config_dict["Theories"].append(theory)

for theory in config_dict["Theories"]:
    if theory == "gInFix":
        config_dict["gInFix"] = configure_gInFix()


# Now print to json
with open(filename, 'w') as config:
    config.write(json.dumps(config_dict))
