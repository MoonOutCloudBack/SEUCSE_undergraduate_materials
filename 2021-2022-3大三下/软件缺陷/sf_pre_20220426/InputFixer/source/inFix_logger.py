"""
This file contains the Logging class which loads all given config information for InputFixer
"""
import datetime
import os
import json
import shutil

class Logger:

    def __init__(self, inFix_config):
        """
        This function initilizes the log by setting up a folder named by the current time in the
        specified log folder
        """
        self.log_folder_path = inFix_config.get_log_folder_path()
        self.session = inFix_config.get_session_path()
        self.config_info = inFix_config

        # Now make a log file folder in the specified location
        self.log_folder_name = str(datetime.datetime.now().strftime('%Y-%m-%d-%H-%M-%S'))
        self.log_folder = os.path.join(self.log_folder_path, self.log_folder_name)
        os.makedirs(self.log_folder)
        
        # Print the config here
        with open(self.log_folder + '/config_log.json', 'w') as out:
            out.write(json.dumps(self.config_info.get_all()))
        
        self.log_file = open(self.log_folder + '/general_log.txt', 'w')
    
    def write(self, string):
        self.log_file.write(string)

    def zip_raw_results(self):
        """
        This function zips up all the raw results and saves them in the file specified by config
        """
        shutil.make_archive(self.log_folder + '/results', 'zip', os.path.join(self.session, '..'), os.path.basename(self.session))

    def finalize_logs(self):
        """
        This function should always be the last call to a logger object
        It closes any open log_files associated with the instance
        """
        self.log_file.close()
