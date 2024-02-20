"""
This file contains the inFix interface. inFix runs the loop for each fixer and keeps track of results
"""
import json
import os
import fixTheories.factory as theory_factory

class InFix:

    def __init__(self, inFix_config, inFix_log):
        self.config = inFix_config
        self.log = inFix_log

    def find_fixes(self, filt=None):
        """
        This function finds all the fixes required by the initializing configuration
        Filter is an optional filter that says which sessions to consider
        """
        
        # See if the config file is indicating that this run is resuming a previous partial run

        to_return = []

        # Load the partial file if there is one
        if self.config.partial_file is not None:
            with open(self.config.partial_file, 'r') as f:
                f = iter(f)
                to_return = []
                for line in f:
                    try:
                        if line.startswith('NEXT'):
                            general_config = json.loads(next(f))
                            experiment_results = json.loads(next(f))
                            if len(experiment_results) == 0: 
                                print("This experiment was not actually completed")
                                continue
                            to_return.append((general_config, experiment_results))
                    except StopIteration:
                        pass
        
        # Make dictionary of values
        zzz = set()
        for config, result in to_return:
            zzz.add(config["UniqueId"])
        theories = self.config.get_theories()
        global_theory_config = [self.config.get_specific_theory_info(x) for x in theories]
        global_theory_factory = [theory_factory.get_theory_solver(i, global_theory_config[0], self.log) for i in theories]
        theory_results = [[] * len(theories)]

        # Now, loop through all the files in path
        base_path = self.config.get_session_path()
        counter = 1
        ak = 0
        for folder in os.listdir(base_path):
            ak += 1
            print("Number: {}".format(ak))
            print(folder)            
            # Check here if the folder has already been done by the thing in the config
            if folder in zzz:
                print("already_done")
                continue
            # Open the additional file
            with open(os.path.join(base_path, folder, folder + '_additional_info.json'), 'r') as out:
                session_config = json.loads(next(out))
            
            # Check if I even need to do this one
            if (not self.config.get_fix_empty()) and session_config["LastIsEmpty"]: continue

            # Check if it matches the given filter
            if filt is not None:
                test_this_one = True
                for x in filt:
                    if not session_config[x]:
                        test_this_one = False
                        break
                if not test_this_one: continue
            
            # If I get here, I will actually run the input fixer format
            self.log.write("File num {} \nID = {}\n".format(counter, folder))
            counter += 1

            for i in range(len(theories)):
                print(theories[i])
                answer = global_theory_factory[i].fix(session_config, os.path.join(base_path, folder),  self.log)
                theory_results[i].append(answer)

            self.log.write('\n')

        return theory_results
