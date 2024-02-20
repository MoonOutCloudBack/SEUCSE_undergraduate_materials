"""
This file will compare the coverage of the student fix and the generated fix for all scenarios 
for which a solution was found by InFixPy
"""
import sys
import random
# import coverage
import subprocess
import json
import os
import statistics

counter = 0

def load_repair_log(log_file_path):
    """
    Loads Repiar log into a list where each sub part looks like:
        Id, Ip, foundSolution, numProbes, machineTime, studentStartTime, studentEndTime, OriginalBadInput, OriginalErrorType, FinalCorrectFix, FinalMinimized Fix, CorrectStudentInputs
    """
    log_file = open(log_file_path, 'r')
    
    tryReading = False
    to_return = []
    log_file = iter(log_file)
    for line in log_file:
        if line.startswith('SCENARIO'):
            to_return.append({})
            to_return[-1]['Id'] = line.split()[1] # Make it so it's just the file 
            tryReading = True
        elif line.startswith('File num'):
            tryReading = False
        elif tryReading:
            if line.startswith('Student Ip'):
                to_return[-1]['Ip'] = line.strip().split(": ")[-1]
            if line.startswith("Found Solution:"):
                to_return[-1]["FoundSolution"] = bool(line.strip().split(": ")[-1])
            if line.startswith("Final correct fix:"):
                to_return[-1]["MachineFix"] = json.loads(next(log_file))
            if line.startswith("Final correct minimized fix:"):
                to_return[-1]["MinimizedMachineFix"] = json.loads(next(log_file))
            if line.startswith("Final minimized fix:"):
                to_return[-1]["MinimizedMachineFix"] = json.loads(next(log_file))
            if line.startswith("Correct Student Inputs:"):
                to_return[-1]["StudentFixes"] = json.loads(next(log_file))
            if line.startswith("Original bad input"):
                to_return[-1]["OriginalBadInput"] = line.strip().split(": ")[-1]
            if line.startswith("Number of Probes"):
                to_return[-1]["NumProbes"] = int(line.strip().split(": ")[-1])
            if line.startswith("Original Error Type"):
                to_return[-1]["OriginalErrorType"] = line.strip().split(": ")[-1]
        # Here, I just load all the lines so I have it -> not coverage specific in this function

    log_file.close()
    return to_return

def erase_coverage(coverage_file_name):
    try:
        subprocess.run('COVERAGE_FILE=' + coverage_file_name + ' coverage erase', 
            check=True, 
            timeout=30,
            shell=True)
    except Exception as e:
        print(e)
        print('erase')

def report_coverage(coverage_file_name):
    try:
        report = subprocess.check_output('COVERAGE_FILE=' + coverage_file_name + ' coverage report -m',
                shell=True)
    except Exception as e:
        print(e)
        return None

    # Here, go through the things
    report = str(report)
    raw_coverage = report[report.find('.py') + 3:]
    if report.find('.py') <= 1: return None

    raw_coverage = raw_coverage.strip().split()
    raw_coverage[-1] = raw_coverage[-1].strip()[:-3]
    return raw_coverage

def run_program_with_coverage(program_file_name, program_input, coverage_file_name, may_timeout=False):
    """
    This function runs coverage on the program with the given input
    """
    inputs = '\n'.join(program_input) + '\n'

    if may_timeout:
        try:
            subprocess.run(['python3', program_file_name],
                    check=True,
                    timeout=5,
                    input=inputs,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    universal_newlines=True)
        except Exception as e:
            print("IN TIMEOUT")
            return -1
    
    # now run the program
    try:
        subprocess.run('COVERAGE_FILE=' + coverage_file_name + ' coverage run --include=*_code.py --branch ' + program_file_name,
                check=True,
                timeout=5,
                shell=True,
                input=inputs,                                   
                stdout=subprocess.PIPE,
                universal_newlines=True)
    except Exception as e:
        print('HERE')
        print(e) 
        return None

def get_coverage(filename, program_input, cov_file_name, care_about_errors=True, may_timeout=False):
    """
    This function returns a dictionary containing the coverage of the input
    """
    x = run_program_with_coverage(filename, program_input, cov_file_name, may_timeout)
    if care_about_errors and x is None: return None

    if may_timeout and x == -1:
        return -1

    # And we analyze that coverage
    coverage_results = report_coverage(cov_file_name)
    # And erase the coverage record
    erase_coverage(cov_file_name)
    if coverage_results is None:
        return None

    for i in range(4):
        coverage_results[i] = int(coverage_results[i])
    coverage_results[4] = int(coverage_results[4][:-1])

    return coverage_results



def compare_coverage(result_info, useMinimizedFix=True):
    """
    This function sees which program -> the student or the machine gets better coverage
    Returns None if there is no student correct answer or if there was no machine correct answer
    """
    global counter
    if not "MachineFix" in result_info: return None
    if not "StudentFixes" in result_info: return None
    if len(result_info["StudentFixes"]) == 0: return None 
    # If we get here, there was both a machine and a student fix
    file_name = os.path.join('/home/endremad/Projects/Python_Tutor_Input_Experiments/PythonTutor_Input_Data_Sessions', result_info['Id'], result_info['Id'] + '_code.py') 
    
    counter += 1
    print(counter)
    print(result_info)
    
    # Here, we get the coverage from the student fix
    print('Student')
    
    # Make the coverage filename for this run
    cov_file_name = result_info['Id']
    
    student_coverage = get_coverage(file_name, result_info["StudentFixes"][0][0], cov_file_name, True)
    
    
    # Now we get the coverage from the machine fix
    print('Machine')
    machine_coverage = get_coverage(file_name, result_info["MachineFix"], cov_file_name, True)
    
    if machine_coverage is None or student_coverage is None: return None

    return student_coverage, machine_coverage

def get_coverage_from_raw(raw_coverage):
    raw_coverage = raw_coverage[raw_coverage.find('.py') + 3:-1]
    raw_coverage = raw_coverage.strip().split()
    print(raw_coverage)
    
def file_format_hac():
    f = open('coverage_results.json', 'r')
    work = json.load(f)
    f.close()

    print(len(work[0]))
    print(len(work[1]))
    num = 0
    for i in range(len(work[1])):
        if work[1][i] is not None and len(work[1][i]) == 2:
            num += 1
    print(num)
    
    # This is where I do the coverage analysis
    student_coverages = []
    machine_coverages = []
    num_branches = []
    for i in range(len(work[1])):
        if work[1][i] is not None:
            num_branches.append(int(work[1][i][0][0]))
        if work[1][i] is not None and len(work[1][i]) == 2:
            student_coverages.append(int(work[1][i][0][4][:-1]))
            machine_coverages.append(int(work[1][i][1][4][:-1]))
            result_info = work[0][i]
            file_name = os.path.join('/home/endremad/Projects/Python_Tutor_Input_Experiments/PythonTutor_Input_Data_Sessions', result_info['Id'], result_info['Id'] + '_code.py') 
            print(work[0][i], flush=True)
            print()
            print(work[1][i], flush=True)
            print()
            with open(file_name, 'r') as code:
                z = ""
                for line in code:
                    z += line
                print(z, flush=True)
            print()
            print()
            print()
            print()
            if len(student_coverages) == 100:
                break
    print(statistics.median(student_coverages))
    print(statistics.median(machine_coverages))
    print(statistics.median(num_branches))
    print(statistics.mean(student_coverages))
    print(statistics.mean(machine_coverages))
    print(statistics.stdev(student_coverages))
    print(statistics.stdev(machine_coverages))
if __name__ == "__main__":

    # First, load the log information
    scenario_results = load_repair_log(sys.argv[-1])
    #print(scenario_results)
    coverages = [compare_coverage(i, True) for i in scenario_results]

    to_dump = [scenario_results, coverages]
    with open('coverage_results.json', 'w') as o:
        json.dump(to_dump, o)
    
    # Then analyize the coverage information
    #print(coverages)
    
    # Then calculate the coverage information
    coverages = file_format_hac()
