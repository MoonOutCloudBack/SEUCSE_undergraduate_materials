"""
This file analyzes the new log format where everything is a json output
"""

import test_coverage
import sys
import json
import statistics
import random
import observational_study as obs

def analyze_base_inFix():
    pass

def analyze_best_coverage(results, num_runs):
    """
    This function examines the best coverage run out of num_runs for each scenario
    """
    num_solutions_found = [0] * num_runs
    coverage_of_machine_solutions = [[] for x in range(num_runs)]
    has_max_coverage = [0 for x in range(num_runs)]
    maximum_coverage = []
    total_scenarios_with_at_least_one_solution = 0
    total_scenarios = 0
    for result in results:
        meta_data = result[0]
        experiment_results = result[1]
        if meta_data["ErrorMessage"] == "" or meta_data["ErrorMessage"] == ":":
            continue
        total_scenarios += 1
        
        # If not all blank, loop and see which ones solved
        max_coverage = 0
        max_index = -1
        has_solution = False
        for i in range(num_runs):
            run = experiment_results[i]
            if len(run) == 0: continue
            
            # If this experiment found a solution
            if run["FoundSolution"]:
                num_solutions_found[i] += 1
                has_solution = True
                if run["FinalCoverage"] is not None:
                    cov_result = run["FinalCoverage"][4]
                    coverage_of_machine_solutions[i].append(cov_result)
                    if cov_result > max_coverage:
                        max_coverage = cov_result
                        max_index = i
        if max_index >= 0:
            has_max_coverage[max_index] += 1
            maximum_coverage.append(max_coverage)

        if has_solution:
            total_scenarios_with_at_least_one_solution += 1
    
    print(num_solutions_found)
    print(total_scenarios)
    print(total_scenarios_with_at_least_one_solution)
    print(has_max_coverage)

    print('Maximum Coverage: {} {} {}'.format(statistics.mean(maximum_coverage), statistics.median(maximum_coverage), statistics.stdev(maximum_coverage))) 
    print('\nIndividual ones:')
    for x in coverage_of_machine_solutions:
        print('{} {} {}'.format(statistics.mean(x), statistics.median(x), statistics.stdev(x)))

def find_closest_good_after(bad_time, good):
    for i in range(len(good)):
        if good[i][1] > bad_time:
            return i
    return None

def find_closest_good_before(bad_time, good):
    for i in reversed(range(len(good))):
        if good[i][1] < bad_time:
            return i
    return None

def find_closest_good(general_config):
    bad_time = general_config['TimeStamp']
    good = general_config['CorrectInputs']
    before = find_closest_good_before(bad_time, good)
    after = find_closest_good_after(bad_time, good)
    return before, after
    if before is None and after is None: return None, None
    if before is None: return after, after
    if after is None: return before, after
    if good[before][1] - bad_time < good[after][1] - bad_time: return before, after
    return after, after
    
def analyze_student_fix_coverage(results):
    """
    This one just sees if how the students did on coverage
    Looks at the last good in the list
    """
    good_after_coverage = []
    all_closest_coverages = []
    for general_config, experiment_results in results:
        #print(experiment_results)
        # print out the file
        #FIXME: CHANGE EACH RUN
        filename = '/home/endremad/Projects/Python_Tutor_Input_Experiments/PythonTutor_Input_Data_Sessions_2015/' + general_config['UniqueId'] + '/' + general_config['UniqueId'] + '_code.py'
        cov_file_name = 'hfhhgurweufheufhwef'
        
        student_fixes = general_config['CorrectInputs']
        closest, after = find_closest_good(general_config)
        if after is None and closest is None: continue
        elif after == closest:
            # This is if they are the same thing
            #def get_coverage(filename, program_input, cov_file_name, care_about_errors=True):
            coverage = test_coverage.get_coverage(filename, student_fixes[after][0], cov_file_name, False)
            if coverage is None: continue
            good_after_coverage.append(coverage[4])
            all_closest_coverages.append(coverage[4])
        elif after is None:
            coverage = test_coverage.get_coverage(filename, student_fixes[closest][0], cov_file_name, False)
            if coverage is None: continue
            all_closest_coverages.append(coverage[4])
        elif closest is None:
            coverage = test_coverage.get_coverage(filename, student_fixes[after][0], cov_file_name, False)
            if coverage is None: continue
            good_after_coverage.append(coverage[4])
        else:
            coverage = test_coverage.get_coverage(filename, student_fixes[after][0], cov_file_name, False)
            if coverage is None: continue
            good_after_coverage.append(coverage[4])
            coverage = test_coverage.get_coverage(filename, student_fixes[closest][0], cov_file_name, False)
            if coverage is None: continue
            all_closest_coverages.append(coverage[4])
        # If here, now just print out so far
        if len(good_after_coverage) > 0:
            print('Student Fixes After: Median {}, Mean {}, Number {}\n'.format(statistics.median(good_after_coverage), statistics.mean(good_after_coverage), len(good_after_coverage)))
        if len(all_closest_coverages) > 0:
            print('Student Fixes Closest: Median {}, Mean {}, Number {}\n'.format(statistics.median(all_closest_coverages), statistics.mean(all_closest_coverages), len(all_closest_coverages)))


def get_probe_length_of_shortest_fix(scenario):
    """
    This function returns the length of the number of probes in the shortest fix for the scenario
    Returns 0 if there were no fixes
    """
    results = scenario[1]
    to_return = 0
    time = 0
    for run in results:
        if not "FoundSolution" in run: continue
        if not run["FoundSolution"]: continue
        if to_return == 0 or run["NumProbesMade"] < to_return: 
            to_return = run["NumProbesMade"]
            time = run["EndTime"] - run["StartTime"]

    return to_return, time

def find_solution_thread(scenario):
    y = get_probe_length_of_shortest_fix(scenario)[0]
    results = scenario[1]
    to_return = 0
    i_saved = -1
    for i in range(5):
        #print(y)
        if not "FoundSolution" in results[i]: continue
        if results[i]["NumProbesMade"] != y: continue
        if not "FinalCoverage" in results[i]: continue
        if results[i]["FinalCoverage"] is None: continue
        if not results[i]["FoundSolution"]: continue
        
        m = (scenario[1][i]["FinalCoverage"][0]-scenario[1][i]["FinalCoverage"][1]) / scenario[1][i]["FinalCoverage"][0]
        print(m)
        if m > to_return: 
            to_return = m
            i_saved = i

    return i_saved

def see_probe_lengths(scenarios):
    results = []
    time = []
    for scenario in scenarios:
        x = get_probe_length_of_shortest_fix(scenario)
        if x != 0:
            y = get_probe_length_of_shortest_fix(scenario)
            results.append(y[0])
            time.append(y[1])
            
    print("Probe lengths")
    results.sort()
    #print(results)
    print(statistics.median(results))
    print(statistics.mean(results))
    print("time")
    time.sort()
    #print(time)
    print(statistics.median(time))
    print(statistics.mean(time))

def compare_coverage(scenarios):
    machine_fixes = []
    student_fixes = []
    for scenario in scenarios:
        x = 0
        x = find_solution_thread(scenario)
        if x != -1:
            try:
                #s = scenario[0]["CorrectInputs"][0][2][4]
                #m = scenario[1][x]["FinalCoverage"][4]
                s = (scenario[0]["CorrectInputs"][0][2][0] - scenario[0]["CorrectInputs"][0][2][1])/ scenario[0]["CorrectInputs"][0][2][0]
                m = (scenario[1][x]["FinalCoverage"][0]-scenario[1][x]["FinalCoverage"][1]) / scenario[1][x]["FinalCoverage"][0]
                #print(s)
                #print(scenario[0]["CorrectInputs"])

                machine_fixes.append(m)
                student_fixes.append(s)
            except:
                #print(scenario[0]["CorrectInputs"][0])
                #print(scenario[1])
                pass
    
    #print(machine_fixes)
    #print(student_fixes)
    print("Machine Coverages")
    print(statistics.median(machine_fixes))
    print(statistics.mean(machine_fixes))
    print(len(student_fixes))
    print("Student Coverages")
    print(statistics.median(student_fixes))
    print(statistics.mean(student_fixes))
    print(len(machine_fixes))

def get_unique_progs(scenarios):
    path = "../../"
    x = obs.get_num_unique_programs_and_users(path, scenarios)
    print(len(x))

    
def analyze_all_valid_prog(scenarios):
    
    line_count = []
    num_inputs = []
    for scenario in scenarios:
        details = scenario[0]
        uid = details["UniqueId"]
        path = "../../PythonTutor_Input_Data_Sessions_2015/" + uid
        x = obs.get_folder_total(path)
        line_count.append(x["net"])
        num_inputs.append(x["num_inputs"])
    
    line_count.sort()
    print(len(line_count))
    num_inputs.sort()
    print("Non comment / blank line_counts:")
    print(statistics.median(line_count))
    print(statistics.mean(line_count))
    print(statistics.stdev(line_count))
    print(line_count[-1])
    print(line_count[0])
    print("Num inputs per program:")
    print(statistics.median(num_inputs))
    print(statistics.mean(num_inputs))
    print(statistics.stdev(num_inputs))
    print(num_inputs[-1])
    print(num_inputs[0])
    

    times = []
    for scenario in scenarios:
        details = scenario[0]
        correct = sorted(details["CorrectInputs"],key=lambda x: x[1])
        time = correct[0][1] - details["TimeStamp"]
        if time > 0:
            times.append(time)
    print("Time for students to complete program:")
    print("len: {}".format(len(times)))
    times.sort()
    print(statistics.median(times))
    print(statistics.mean(times))
    print(statistics.stdev(times))
    print(times[-1])
    print(times[-2])
    print(times[2851])
    print(times[3002])
    print(times[0])
    for i in range(len(times)):
        if times[i] >= 420:
            print(i)
            break
    #Error Messages
    errors = {"invalid_int":0, "invalid_float":0, "too_many_val":0, "two_few_val":0}
    for scenario in scenarios:
        details = scenario[0]
        error = details["ErrorType"]
        if (error in errors):
            errors[error]+=1
        else:
            errors[error] = 0
        if error == "ValueError":
            if details["ErrorMessage"].find("invalid literal for int") >= 0:
                errors["invalid_int"] += 1
            elif details["ErrorMessage"].find("could not convert string to float") >= 0:
                errors["invalid_float"] += 1           
            elif details["ErrorMessage"].find("not enough values to") >= 0:
                errors["two_few_val"] += 1
            elif details["ErrorMessage"].find("too many values to") >= 0:
                errors["too_many_val"] += 1
    print("Total error types")
    print(errors)

    # Average size
    num_chars = []
    num_tokens = []
    for scenario in scenarios:
        details = scenario[0]
        num = 0
        num_t = 0
        inp = details["BadInput"]
        for x in inp: 
            num += len(x)
            num_t += len(x.strip().split())
        num_chars.append(num + len(inp))
        num_tokens.append(num_t)
    print("Num chars per input:")
    num_chars.sort()
    print(statistics.median(num_chars))
    print(statistics.mean(num_chars))
    print(statistics.stdev(num_chars))
    print(num_chars[-1])
    print(num_chars[0])
    print(num_chars[5559])
    print(num_chars[6254])
    print("Num tokens per input:")
    num_tokens.sort()
    print(statistics.median(num_tokens))
    print(statistics.mean(num_tokens))
    print(statistics.stdev(num_tokens))
    print(num_tokens[-1])
    print(num_tokens[0])
    print(num_tokens[5559])
    print(num_tokens[6254])
    


def read_json_log(log_file):
    """
    General function for loading a json based log
    """
    with open(log_file, 'r') as f:
        f = iter(f)
        to_return = []
        for line in f:
            try:
                if line.startswith('NEXT'):
                    general_config = json.loads(next(f))
                    experiment_results = json.loads(next(f))
                    if len(experiment_results) == 0: print('hi')
                    to_return.append((general_config, experiment_results))
            except StopIteration:
                pass
        return to_return

"""
The following functions are all meant to be for calls to filter
"""

def was_solved(log_entry):
    """
    This function returns true if the log entry was solved
    """
    #print(log_entry)
    for result in log_entry[1]:
        #print(result)
        if "FoundSolution" in result and result["FoundSolution"]:
            return True
    return False

def was_first_of_user(log_entry):
    """
    This function returns true if this function was the first one
    in the session -> that is, if the ending was _0
    """
    if log_entry[0]["UniqueId"][-2:] == "_0": return True
    return False

def bad_input_before_all_fixes(log_entry):
    """
    This function returns true if the bad fix was before all fixes
    Returns False if no fixes
    """
    info = log_entry[0]
    if len(info["CorrectInputs"]) == 0: return False
    correct_time = sorted(info["CorrectInputs"], key=lambda x: x[1])[0][1]
    return info["TimeStamp"] < correct_time

def bad_input_before_any_fix(log_entry):
    """
    This function returns true if the bad fix was before any fix
    Returns False if no fixes
    """
    info = log_entry[0]
    if len(info["CorrectInputs"]) == 0: return False
    correct_time = sorted(info["CorrectInputs"], key=lambda x: x[1])[-1][1]
    return info["TimeStamp"] < correct_time

    
def was_solved_in_x(num_probes, num_threads, log_entry):
    for result in log_entry[1][:num_threads]:
        #print(result)
        if "FoundSolution" in result and result["FoundSolution"] and result["NumProbesMade"] <= num_probes:
            return True
    return False

def a_square(num_probes, num_threads, log):

    num = 0
    for entry in log:
        if was_solved_in_x(num_probes, num_threads, entry): num += 1
    return num


def print_grid(results):
    num_probes = [1, 2, 3, 4, 5, 10, 20, 30, 60, 500]

    for p in num_probes:
        for t in range(1, 6):
            num = a_square(p, t, results)
            print("{}/{} ".format(num,num/len(results)),)
        print()

if __name__ == "__main__":
    # First, deal with the command line file
    log_file = sys.argv[-2]

    # Now read in the log file
    results = read_json_log(log_file)
    get_unique_progs(results)
    
    #analyze_all_valid_prog(results)
    #print(results)
    # Now analyze for the specific global config
    #analyze_best_coverage(results, int(sys.argv[-1]))
    #analyze_student_fix_coverage(results)

    # Filter so we only have the ones that were solved
    solved = list(filter(was_solved, results))
    see_probe_lengths(solved)

    print(len(results))
    print(len(solved))
    print(print_grid(results))
    """
    compare_coverage(results)
    # filter so we only have one per session
    one_per_session = list(filter(was_first_of_user, solved))

    # now get the two halves
    bad_before_all_fixes = list(filter(bad_input_before_all_fixes, one_per_session))

    rest = list(filter(lambda v: not bad_input_before_all_fixes(v), one_per_session))
    print(len(results))
    print(len(solved))
    print(len(one_per_session))
    print(len(bad_before_all_fixes))
    print(len(rest))
    bad_before_sample = random.sample(bad_before_all_fixes, 100)
    rest_sample = random.sample(rest, 100)

    # Json_dump these
    with open('stimuli_select.json', 'w') as out:
        out.write(json.dumps({'bad_before': bad_before_sample, 'rest': rest_sample}))
    """

