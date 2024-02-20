input_data_json = "../../Data/processed_data/2018-py3_inFixPy_ready.json"

session_meta_directory_path = "/home/endremad/Projects/Python_Tutor_Input_Experiments/"

session_meta_directory_name = "PythonTutor_Input_Data_Sessions_2018_with_coverage"

########## UNLESS INPUT FORMAT CHANGES, DON'T NEED TO MODIFY BELOW HERE ##########

import json
import sys
import os
import subprocess
import shutil
from collections import defaultdict
import test_coverage
import random

test_folder = session_meta_directory_path + session_meta_directory_name

# Load the input data
with open(input_data_json, 'r') as in_file:
    raw_data = json.loads(next(in_file))

print(raw_data[0])
ids = defaultdict(int)
programs_cache = set()
num_bads = 0
num_programs = 0
num_not_skipped = 0
bad_files = open('bad_files', 'w')
for session_name_base, program, inputs in raw_data:
    #if (session_name_base !=  '31ef10db-c6eb-4017-ff81-fecdeca5ee00'): continue
    session_name_base = session_name_base + '_'
    num_not_skipped += 1
    #if program in programs_cache: continue
    new_inputs = []
    # First make sure this one fits the profile of ones to use -> That is, has a good one after it
    # And has no duplicates
    for i in range(len(inputs)):
        new_inputs.append(inputs[i])
        for j in range(i):
            if inputs[i][0] == inputs[j][0]:
                del new_inputs[-1]
                break
    inputs = new_inputs
    inputs = [ one for one in inputs if not (one[1] == 'bad' and (one[2] == '' or one[3].find('Error') < 0 or one[2] == 'EOFError'))]
    # TODO: Get rid of this last good thing? I'm not sure
    """
    first_bad = 0
    last_good = len(inputs)-1
    done_bad = False
    done_good = False
    while first_bad < last_good:
        if not done_bad and inputs[first_bad][1] == 'bad':
            done_bad = True
        elif not done_good and inputs[last_good][1] == 'good':
            done_good = True
        
        if done_good and done_bad: break
        if not done_bad: first_bad += 1
        if not done_good: last_good -= 1
    
    if first_bad < last_good:   
        inputs = inputs[first_bad: last_good +1]
    else: continue
    """
    has_good = False
    has_bad = False
    for x in inputs:
        if x[1] == 'bad': has_bad = True
        if x[1] == 'good': has_good = True
    if not has_good or not has_bad: continue
    print(inputs)

    programs_cache.add(program)
    num_programs += 1

    # First Make a list of all the fixes and errors
    correct_inputs = []
    bad_inputs = []
    for ip in inputs:
        if ip[1] == 'bad': 
            bad_inputs.append(ip)
            num_bads += 1
        else: correct_inputs.append([ip[0], ip[5]])
    
    # Then go through each bad and make a file for it
    for bad in bad_inputs:
        session_name = session_name_base + str(ids[session_name_base])
        ids[session_name_base] += 1
        session_folder = test_folder + '/' + session_name
        if os.path.exists(session_folder):
            #os.removedirs(session_folder)
            shutil.rmtree(session_folder)
        os.makedirs(session_folder)
        
        # Add all the files to each folder as needed
        code_filename = session_folder +'/' + session_name + '_code.py'
        with open(code_filename, 'w') as out:
            out.write(program)
        with open(session_folder + '/' + session_name + '_bad_input.txt', 'w') as out:
            out.write('\n'.join(bad[0]) + '\n')
        
        # Calcuate the coverage this bad-input achieves
        coverage_info = None
        coverage_info = test_coverage.get_coverage(code_filename, bad[0], '2018coverage.txt', False, False)

        # Calculate the coverage the good-inputs achieve
        actual_correct_inputs = []
        for i in range(len(correct_inputs)):
            x = test_coverage.get_coverage(code_filename, correct_inputs[i][0], '2018correctcoverage.txt', False, True)
            if x != -1:
                correct_inputs[i].append(x)
                actual_correct_inputs.append(correct_inputs[i])
        
        if len(actual_correct_inputs) == 0:
            bad_files.write(session_folder + '\n')
            continue

        print(coverage_info)
        with open(session_folder + '/' + session_name + '_additional_info.json', 'w') as out:
            out.write(json.dumps(
                {'CorrectInputs': actual_correct_inputs,
                 'BadInput': bad[0],
                 'ErrorType' : bad[2],
                 'ErrorMessage' : bad[3],
                 'ip' : bad[4],
                 'TimeStamp' : bad[5],
                 'UniqueId' : session_name,
                 'LastIsEmpty': (bad[0][-1] == ""),
                 'CoverageInfo': coverage_info}))

print(num_bads)
print(num_programs)
print(num_not_skipped)
