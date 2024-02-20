"""
This is a hail mary approach based off my state machine idea
"""
from datetime import datetime
import json
import time
import os
import subprocess
from copy import deepcopy
import random
import string
import test_coverage
import fixTheories.static_analysis as static_analysis

class StateMachine:
    def __init__(self, global_config, log):
        """
        This function is only run once when the first scenrio is run. It sets constants that remain the same for each scenario.
        """

        # Print out the global config to the log so we have it
        log.write(str(global_config))
        self.log = log
        
        #TODO: Make versions of representation, fault, fix, operators, and search stratagy
        self.max_num_fix = global_config["MaxNumFix"]
        self.max_num_probe = global_config["MaxNumProbes"]
        self.testCoverage = global_config["UseCoverage"]
        
        # The number of scenarios that are tried and the number that are fixed
        self.num_tried = 0
        self.num_fixed = 0
        
        self.bad_input_cache = [''] #TODO: Add good input cache?
        self.num_worse = 0

        # Maybe should remove this too
        self.string_literals = []
        self.MIN_RAND_INT = -1
        self.MAX_RAND_INT = 10
    
    def init_scenario(self, session_config, scenario_folder_path, isFirst):
        """
        This function initializes the specific run of a scenario
        isFirst says if this is the first run of the scenario -> there can be multiple runs
        depending on the coverage option
        """
        if isFirst:
            self.input_history = []
            self.num_tried += 1 

            # This gets the values held in the split calls in the program
            tokens = static_analysis.tokenize_program(os.path.join(scenario_folder_path, session_config['UniqueId'] + '_code.py'))
            self.split_values = static_analysis.collect_splits(tokens)
            self.string_literals = static_analysis.collect_string_literals(tokens)

            # Might be deleating this
            if len(session_config["BadInput"]) > 15:
                self.bad_input_cache.extend(session_config["BadInput"][:15])
            elif session_config["BadInput"][-1] == '':
                self.bad_input_cache.extend(session_config["BadInput"][-1])
            else:
                self.bad_input_cache.extend(session_config["BadInput"])
            self.cache = {str(session_config["BadInput"])}
            self.found_at_least_one_solution = False
        else:
            # Otherwise, initialize it to just having the original bad input
            self.input_history = [deepcopy(session_config["BadInput"])]
        self.found_solution = False
        self.coverage_info = session_config["CoverageInfo"]
    
    def fix(self, scenario_config, scenario_folder_path, log):
        """
        This function is the only one that runs from the main file, 
        chooses what version of coverage to use
        """
        
        self.log.write('NEXT SCENARIO\n')
        
        # Log which scenario we are doing
        self.log.write('{}\n'.format(json.dumps(scenario_config)))

        # Initialize the scenario run
        self.init_scenario(scenario_config, scenario_folder_path, True)
        
        # If we don't need to use coverage
        if not self.testCoverage:
            results = self.fix_body(scenario_config, scenario_folder_path, log)

        # If we want to use the best coverage of multiple runs:
        elif isinstance(self.testCoverage, int):
            results = []
            for i in range(self.testCoverage):
                results.append(self.fix_body(scenario_config, scenario_folder_path, log))
                self.init_scenario(scenario_config, scenario_folder_path, False)

        """
        # This is the two phase approach where I do random, then for the rest of the budget try improving coverage
        elif self.testCoverage == "TwoPhase":
            pass

        # This one uses static analysis to be smart about what coverage it chooses
        elif self.testCoverage == "SmartCoverage":
            tokens = static_analysis.tokenize_program(os.path.join(scenario_folder_path, scenario_config['UniqueId'] + '_code.py'))
            use_g_templates = static_analysis.use_grammar_templates(tokens)
            if use_g_templates and False:
                results = []
                print('LOL')
            else:
                self.string_literals = static_analysis.collect_string_literals(tokens)
                results = self.fix_body(scenario_config, scenario_folder_path, log) 
                print(self.num_worse)
                results['GrammarTemplateUsed'] = False
        """

        # Log the results
        self.log.write('{}\n'.format(json.dumps(results)))
    
    def find_location_in_input(self, bad_input, error_message):
        """
        This function searches backwards and returns the index of the element that should be replaced
        TODO: Make it find all instances if possible?
        """
        
        # TODO: make this better
        stripped_error = error_message.strip()
        last_element = stripped_error[-1] 
        to_look_for = stripped_error[stripped_error.find(last_element)+1:-1]

        for i, e in reversed(list(enumerate(bad_input))):
            if len(to_look_for) == 0 and len(e) == 0:
                return i, 0, 0

            element_index = e.rfind(to_look_for)
            if element_index >= 0:
                return (i, element_index, len(to_look_for))
        
        # If not found, do a random one
        rand_row = random.randint(0, len(bad_input) - 1)
        return rand_row, 0, len(bad_input[rand_row])

    def start_timer(self):
        self.start_time = time.time()

    def end_timer(self):
        self.end_time = time.time()

    def get_join_val(self):
        """
        This function returns a value inside of a split call in the
        program if a split call exists there
        """
        if len(self.split_values) == 0: return ' '
        to_return = random.choice(self.split_values)
        if to_return is None: return ' '
        return to_return

    def gen_random_string(self, length):
        if len(self.string_literals) > 0 and random.choice('01') == '1':
            return random.choice(self.string_literals)
        return ''.join(random.choice(string.ascii_uppercase + string.ascii_lowercase + string.digits + string.digits + '--') for _ in range(length))

    def random_mutation(self, bad_input):
        """
        If this function is called, the bad input string is randomly mutated
        """
        #TODO: Modify so random token generator will generate tokens from the source code
        # First, figure out which broad one to do
        # First pick a line to operate on
        if len(bad_input) == 0:
            bad_input.append(self.gen_random_string(3))
            return bad_input
        
        operation_line = random.randint(0, len(bad_input) - 1)
        split_val = self.get_join_val()

        # Now, see if this line can be broken into tokens
        #TODO: deal with splitting this some ways
        split_version = bad_input[operation_line].split(split_val)
        #TODO: MAKE THIS MORE EFFICIENT
        if len(split_version) <= 1:
            # Then here, we either add, delete, or swap
            operation_choice = random.randint(0, 4)
            if operation_choice == 0: #Empties all input
                bad_input = []
            elif operation_choice == 1: #Fully removes a random line
                bad_input.pop(operation_line)
            elif operation_choice == 2: # Inserts a new line with either a random element from the original buggy input or a string literal from the program
                bad_input.insert(operation_line, random.choice(bad_input + self.input_history[0] + self.string_literals))
            elif operation_choice == 3: #inserts a new line with a random string
                bad_input.insert(operation_line, self.gen_random_string(3))
            elif operation_choice == 4: #adds a random string token at the end of the line
                bad_input[operation_line] = bad_input[operation_line] + split_val + self.gen_random_string(3)
        else:
            operation_choice = random.randint(0, 4)
            if operation_choice == 0: #Makes a random line blank
                bad_input[operation_line] = ''
            elif operation_choice == 1:
                bad_input.pop(operation_line) #Replaces a random line with multiple lines split on whitespace
                bad_input[operation_line:operation_line] = split_version
            elif operation_choice == 2: # Replaces a token with a random string from the past or a string literal
                split_version[random.randint(0, len(split_version) - 1)] = random.choice(bad_input + self.input_history[0] + self.string_literals)
                bad_input[operation_line] = split_val.join(split_version)
            elif operation_choice == 3: # Same as last, but random string
                split_version[random.randint(0, len(split_version) - 1)] = self.gen_random_string(3)
                bad_input[operation_line] = split_val.join(split_version)
            #elif operation_choice == 4:
            #    split_version[random.randint(0, len(split_version) - 1)] = random.choice(self.bad_input_cache)
            #    bad_input[operation_line] = split_val.join(split_version)

        return bad_input

    
    def fix_body(self, session_config, scenario_folder_path, log):
        """
        This function tries to do the repair the original way with error message and random
        templates
        """ 

        # Set up the local variables for this fix
        bad_input = deepcopy(session_config["BadInput"])
        num_fixes_found = 0
        num_probes_made = 0
        last_error_type = deepcopy(session_config["ErrorType"])
        last_error_message = deepcopy(session_config["ErrorMessage"])
        last_error_message = last_error_message[last_error_message.find(last_error_type) + len(last_error_type):]

        # Record the starting time
        self.start_timer()
        
        while num_fixes_found < self.max_num_fix and num_probes_made < self.max_num_probe:
            print('message was: ' + last_error_type)
            if last_error_type == "" or last_error_type == ":":
                print('ERROR -> message was: ' + last_error_type)
                num_probes_made = self.max_num_probe
                return {}

            self.input_history.append(deepcopy(bad_input))
            
            x = random.randint(0,1)
            # This section applies the error message templateas
            #TODO: Put in own function
            if last_error_type  == "ValueError" and x == 1:
                #TODO: Don't have this fault location always be the last input line
                # First look for values to unpack
                if last_error_message.find('too many values to unpack') >= 0:
                    #TODO: add in static for split
                    if len(bad_input) == 0:
                        bad_input.append(self.gen_random_string(3) + ' ' + self.gen_random_string(3))
                    split_val = self.get_join_val()
                    split_last = bad_input[-1].strip().split(split_val)
                    bad_input[-1] = split_val.join(split_last[:-1])

                elif last_error_message.find("not enough values to unpack") >= 0:
                    split_val = self.get_join_val()
                    if len(bad_input) == 0:
                        bad_input.append(self.gen_random_string(3) + split_val + self.gen_random_string(3))
                        #bad_input.append('')
                    split_last = bad_input[-1].strip().split(split_val)
                    
                    # Likely the error refers to an earlier part of bad input
                    if len(split_last) > self.MAX_RAND_INT:
                        bad_input = bad_input[:-1]
                    else:
                        if len(split_last) > 0:
                            split_last.append(split_last[-1])
                        else:
                            split_last = [self.gen_random_string(3)]
                        bad_input[-1] = split_val.join(split_last)

                elif last_error_message.find("invalid literal for int()") >= 0:
                    if len(bad_input) == 0:
                        bad_input = [str(random.randint(self.MIN_RAND_INT, self.MAX_RAND_INT))]
                    else:
                        line, i, l = self.find_location_in_input(bad_input, last_error_message)
                        bad_input[line] = bad_input[line][:i] + str(random.randint(self.MIN_RAND_INT, self.MAX_RAND_INT)) + bad_input[line][i + l:]

                elif last_error_message.find("could not convert string to float") >= 0:
                    # First, localize the error
                    if len(bad_input) == 0:
                        bad_input = [str(random.randint(self.MIN_RAND_INT, self.MAX_RAND_INT)) + '.' + str(random.randint(0, 10))]
                    else:
                        line, i, l = self.find_location_in_input(bad_input, last_error_message)
                        bad_input[line] = bad_input[line][:i] + str(random.randint(self.MIN_RAND_INT, self.MAX_RAND_INT)) + '.' + str(random.randint(0, 10))+ bad_input[line][i + l:]

            elif last_error_type == "EOFError" and x == 1:
                # In this case, repete the last line
                # TODO: Make this work better in the future
                if len(bad_input) > 0:
                    bad_input.append(random.choice(bad_input + self.input_history[0] + [self.gen_random_string(3)]))
                else:
                    bad_input.append(self.gen_random_string(3))

            # This section applies the random mutation templates as needed
            
            while str(bad_input) in self.cache:
                bad_input = self.random_mutation(bad_input)
            
            if str(bad_input) in self.cache:
                break
            # Ok, now add the modified version back into the cache
            self.cache.add(str(bad_input))
            num_probes_made += 1

            # Next, run the program to see if it was fixed
            program_file_name = os.path.join(scenario_folder_path, session_config['UniqueId'] + '_code.py')
            last_error_type, last_error_message, full_error = self.try_program(program_file_name, bad_input)

            if last_error_type is None:
                num_fixes_found += 1
                print("SOLVED THE ERROR")
                self.found_solution = True
                if not self.found_at_least_one_solution:
                    self.num_fixed += 1
                self.found_at_least_one_solution = True
                self.final_input = bad_input
                self.minimized_input = self.minimize(program_file_name, deepcopy(bad_input))
                self.solution_coverage = self.get_solution_coverage(program_file_name, bad_input)
        
        self.end_timer()
        self.num_worse += 1
        print("Num tried: {}".format(self.num_tried))
        print("Num fixed: {}".format(self.num_fixed)) 
        return self.collect_fix_results(num_probes_made)

    def collect_fix_results(self, num_probes_made):
        results = {}
        results['FoundSolution'] = self.found_solution
        results['NumProbesMade'] = num_probes_made
        results['StartTime'] = self.start_time
        results['EndTime'] = self.end_time
        if self.found_solution:
            results['FinalCoverage'] = self.solution_coverage
            results['FinalSolution'] = deepcopy(self.final_input)
            results['FinalMinimizedSolution'] = deepcopy(self.minimized_input)
        results['InputHistory'] = deepcopy(self.input_history)
        return results
 
    def get_solution_coverage(self, program_file_name, program_input):
        cov_file_name = self.gen_random_string(20)
        return test_coverage.get_coverage(program_file_name, program_input, cov_file_name, False)
    
    def try_program(self, program_file_name, program_input):
        """
        This function runs the program with the specidied input and sees if it errors out or not
        """
        inputs = '\n'.join(program_input) + '\n'
        
        print('Program Input: {}'.format(program_input))
        # Now run the program:
        run_result = None
        error_last_line = None
        full_error = None
        try:
            subprocess.run(['python3', program_file_name],
                    check=True,
                    timeout=5,
                    input=inputs,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    universal_newlines=True)
        except Exception as e:
            print(e.stderr)
            full_error = e.stderr
            important = e.stderr.strip().split('\n')[-1].strip()
            run_result = important[:important.find(':')]
            error_last_line = important[important.find(':'):]

        return run_result, error_last_line, full_error

    def minimize(self, program_file_name, program_input):
        """
        Assumes program_input is a working input
        """
        
        if (len(program_input)) <= 1:
            return program_input

        result, _, _ = self.try_program(program_file_name, program_input[:-1])

        if result is not None:
            return program_input
        else:
            return self.minimize(program_file_name, program_input[:-1])
