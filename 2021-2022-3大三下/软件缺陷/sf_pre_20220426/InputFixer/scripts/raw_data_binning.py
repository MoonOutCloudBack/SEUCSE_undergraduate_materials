#This file bins ids so they can be chosen at random for the human study
import sys
import os
import json
import statistics
import shutil

def load_scenario_configs(folder_path):
    """
    This function loads all the additional configs from the given folder holding scenarios
    Only loads if the last is not empty
    """
    # Load the base time
    folder_lowest_time = {}
    is_first = set()
    i = 0
    for folder in os.listdir(folder_path):
        try:
            with open(os.path.join(folder_path, folder, folder + '_additional_info.json'), 'r') as out:
                i += 1
                session_config = json.loads(next(out))
                fold_base = folder[:folder.find('_')]
                if not (fold_base in folder_lowest_time):
                    folder_lowest_time[fold_base] = session_config['TimeStamp']
                elif folder_lowest_time[fold_base] > session_config['TimeStamp']:
                    folder_lowest_time[fold_base] = session_config['TimeStamp']
        except Exception as e:
            shutil.rmtree(os.path.join(folder_path, folder), ignore_errors=True, onerror=None)
    print('yo')
    configs = []
    for folder in os.listdir(folder_path):
            
        with open(os.path.join(folder_path, folder, folder + '_additional_info.json'), 'r') as out:
            session_config = json.loads(next(out))
            fold_base = folder[:folder.find('_')]
        
            # Makes sure we have first bad one that was made - don't want to make these timestamps too low
            if folder_lowest_time[fold_base] != session_config['TimeStamp']:
                continue
            if not session_config['LastIsEmpty'] and session_config['ErrorType'] != '' and session_config['ErrorType'] != ':' and len(session_config['CorrectInputs']) > 0:
                code_str = ''
                with open(os.path.join(folder_path, folder, folder + '_code.py'), 'r') as code:
                    for line in code:
                        code_str += line
                    session_config['Code'] = code_str    
                    configs.append(session_config)
    return configs
    print('la')

def filter_for_correct_after_bad(scenario_config):
    """
    This function returns a list of indicies into session_config where
    the bad input comes before any good input
    """
    bad_before_any_good = []
    for i in range(len(scenario_config)):
        bad_time = scenario_config[i]['TimeStamp']
        sorted_correct = sorted(scenario_config[i]['CorrectInputs'], key=lambda x: x[1])
        scenario_config[i]['CorrectInputs'] = sorted_correct
        good_time = sorted_correct[0][1]
        if good_time > bad_time:
            bad_before_any_good.append(i)
    return bad_before_any_good

def get_bug_fix_durations(scenario_configs, bad_before_any_good):
    durations = []
    for i in bad_before_any_good:
        bad_time = scenario_configs[i]['TimeStamp']
        good_time = scenario_configs[i]['CorrectInputs'][0][1]
        duration = good_time - bad_time
        student_coverage = scenario_configs[i]['CorrectInputs'][0][2][4]
        durations.append([scenario_configs[i]['UniqueId'], duration, student_coverage])
    return durations

if __name__ == "__main__":
    folder_path = sys.argv[-1]
    all_valid_scenarios = load_scenario_configs(folder_path)
    bad_before_any_good = filter_for_correct_after_bad(all_valid_scenarios)
    print(len(bad_before_any_good))
    durations = get_bug_fix_durations(all_valid_scenarios, bad_before_any_good)
    durations.sort(key=lambda x: x[1])
    dur_times_only = [x[1] for x in durations]
    mean = statistics.mean(dur_times_only)
    stdev = statistics.stdev(dur_times_only)
    print(statistics.stdev(dur_times_only))
    dur_times_only = [x for x in dur_times_only if x < (stdev + mean)]
    print(len(dur_times_only))
    num_per_bucket = len(dur_times_only) // 3
    print(num_per_bucket)
    quick_fix = durations[:num_per_bucket]
    long_fix = durations[2*num_per_bucket:len(dur_times_only)]
    with open('stimuli_duration_bins.json', 'w') as o:
        o.write(json.dumps({'quick_fix': quick_fix, 'long_fix': long_fix}))

    print('coverages')
    coverages = get_bug_fix_durations(all_valid_scenarios, bad_before_any_good)
    coverages.sort(key=lambda x: x[2])
    cov_only = [x[2] for x in coverages]
    mean = statistics.mean(cov_only)
    stdev = statistics.stdev(cov_only)
    print(statistics.stdev(cov_only))
    print(len(cov_only))
    num_per_bucket = len(cov_only) // 3
    print(num_per_bucket)
    quick_fix_cov = coverages[:num_per_bucket]
    long_fix_cov = coverages[2*num_per_bucket:]
    with open('stimuli_cov_bins.json', 'w') as o:
        o.write(json.dumps({'low_cov': quick_fix_cov, 'high_cov': long_fix_cov}))

    print(cov_only)
