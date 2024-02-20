"""
This file contains the Config class which loads all given config information for InputFixer
"""

class Config:

    def __init__(self, config_dictionary):
        
        self.config = config_dictionary
        self.theories = config_dictionary["Theories"]
        self.session_path = config_dictionary["SessionsFolderPath"]
        self.log_folder_path = config_dictionary["LogFolderPath"]
        self.fix_empty = config_dictionary["FixEmptyLastInputs"]

        # See if the configuration file specifies a partial run we are building off of
        self.partial_file = None
        try:
            self.partial_file = config_dictionary["PartialResultsFile"]
        except KeyError:
            self.partial_file = None
    
    def get_theories(self):
        """
        Returns the list of theories this config will use to find fixes
        """
        return self.theories

    def get_all(self):
        return self.config

    def get_session_path(self):
        return self.session_path

    def get_log_folder_path(self):
        return self.log_folder_path

    def get_fix_empty(self):
        return self.fix_empty

    def get_specific_theory_info(self, theory_name):
        """
        This function returns the dictionary associated with the specified theory
        Returns None if no theory of that type in the configuration
        """
        try:
            return self.config[theory_name]
        except:
            print("No theory of the name " + theory_name + " exists in this config. ")
            return None
