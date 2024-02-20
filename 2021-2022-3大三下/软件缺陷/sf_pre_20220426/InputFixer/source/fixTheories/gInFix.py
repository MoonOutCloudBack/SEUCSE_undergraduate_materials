"""
This file contains the GInFix class which uses a genprog like / fuzzing approach to fixing the errors
"""

import datetime


class GFactory:

    def get_representation(self, rstring):
        # TODO: Implement this so supports multiple representations
        return None
    
    def get_localize(self, rstring):
        return Localize()

    def get_search(self, rstring):
        return Search()

class GInFix:

    def __init__(self, global_config, log):
        self.isFirst = True
        log.write(global_config)

        #TODO: Make versions of representation, fault, fix, operators, and search stratagy
        gfac = GFactory()
        self.reprsentation = global_config["Represent"]
        self.Localizer = gfac.get_localize(global_config["Localizer"])
        self.Search = gfac.getSearch(global_congfig["Search"])
        self.max_num_fix = global_config["MaxNumFix"]
        self.max_num_probe = global_config["MaxNumProbes"]

    def fix(self, session_config, log):
        """
        This is the only method that should be called from outside this file
        It runs the overall fixer
        """

        # Convert the bad input into its modifiable datastructure form -> a list of lists
        bad_input = [x.split() for x in session_config["BadInput"]]

        # Initialize the cache with the original bad, so we don't try the same result twice
        cache = {self.original_bad_input}

        # Record the starting time
        self.start_time = datetime.datetime.now()
        
        # Loop through and try to find fixes
        num_fixes_found = 0
        num_probes_made = 0
        current_bad_localized = False

        while num_fixes_found < self.max_num_fix and num_probes_made < self.max_num_probe:
            
            # If this bad has't been looked at yet, do fault and fix localization
            if not current_bad_localized:
                self.Localizer.find_error_weights(bad_input)
                #self.Localizer.find_fixes(self.operators)
                current_bad_localized = True

            # Now use the search to choose an error location and a candidate fix
            self.Search.make_test_input(self.Localizer, cache)
            num_probes_made += 1
            self.log.write("Trying input num {}: Input is {}\n".format(num_probes_made, self.Search.chage_as_str()))

            # Next, test this input and see if it does better or worse
            
            # Finally, either change the bad inputs, modify weights, or nothing and try again
        
        # Record the ending time
        elapsed_time = datetime.datetime.now) - self.start_time
        self.log("The time elapsed was {}".format(elapsed_time))


### SEARCH BASE CLASS AND SUB CLASSES ###
class Search:
    def __init__(self):
        pass

    def make_test_input(self):
        return None

#### FAULT LOCALIZATION ClASSESS ####
class Localize:

    def __init__(self):
        pass

    def find_error_weights(self, bad_input, error_message, error_type):
        """
        finds how likely each token is to be the cause of the error
        """
        # Look at the message itself and use it as a place to start
        if error_type == 'ValueError':
            if 
        return None

    def find_fixes(self, possible_operators):
        """
        Returns a list of operators in order of which to apply to the problem at this location
        """
        return None


#### REPRESENTATION CLASSESS ####

class Representation:
    """
    This class is how the input that I will be modifying is represented
    """
    def __init__(self, bad_input):
        pass



