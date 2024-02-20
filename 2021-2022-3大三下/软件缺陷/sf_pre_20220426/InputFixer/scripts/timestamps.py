# This script just looks at the loog file stuff for time stamps

import sys
import numpy as np
import statistics
import stats

log_file = sys.argv[1]

lf = open(log_file, 'r')
# First read the logfile in
lf_lines = []
for line in lf:
    if line.startswith("SCENARIO") or line.startswith('Found Solution') or line.startswith('Number of Probes made') or line.startswith('Start time: ') or line.startswith('End time: ') or line.startswith('Student ') or line.startswith('Final correct minimized fix'):
           lf_lines.append(line)
lf.close()
lf_lines.extend(["1", "1", "1", "2"])
total_num_inputs = 0
total_num_solutions = 0
total_num_probes = 0
total_num_has_student_fix_time = 0

total_student_time_for_solved = 0
student_solved_probes = []
student_solved_times = []
total_machine_time_for_solved = 0
machine_solved_times = []

total_student_time_for_unsolved = 0
student_unsolved_times = []
total_machine_time_for_unsolved = 0
machine_unsolved_times = []

num_triv = 0

for i in range(len(lf_lines)):
    if lf_lines[i].startswith("SCENARIO") and lf_lines[i + 7].startswith("Student next fix "):
        
        total_num_inputs += 1
        machine_start_time = float(lf_lines[i + 4].strip().split(": ")[-1])
        machine_end_time = float(lf_lines[i + 5].strip().split(": ")[-1])
        machine_time = machine_end_time - machine_start_time

        student_start_time = int(lf_lines[i + 6].strip().split(": ")[-1])
        student_end_time = int(lf_lines[i + 7].strip().split(": ")[-1])
        student_time = student_end_time - student_start_time
        
        
        if lf_lines[i + 2].find("True") >= 0:
            if student_time > 1000: continue
            total_num_solutions += 1
            num_probes = int(lf_lines[i + 3].strip().split(": ")[-1])
            total_num_probes += num_probes
            total_student_time_for_solved += student_time
            total_machine_time_for_solved += machine_time
            student_solved_times.append(student_time)
            student_solved_probes.append(num_probes)
            machine_solved_times.append(machine_time)
            final_fix = lf_lines[i + 8].strip().split(": ")[-1]
            if final_fix == "[]" or final_fix == "['']" or final_fix == "['0']":
                num_triv += 1
        else:
            total_student_time_for_unsolved += student_time
            total_machine_time_for_unsolved += machine_time
            student_unsolved_times.append(student_time)
            machine_unsolved_times.append(machine_time)

print("Total num inputs: {}".format(total_num_inputs))
print("Total num solutions: {}".format(total_num_solutions))
print("Total num probes for solved: {}".format(total_num_probes))
print("Total student time for solved: {}".format(total_student_time_for_solved))
print("Total machine time for solved: {}".format(total_machine_time_for_solved))
print("Total student time for unsolved: {}".format(total_student_time_for_unsolved))
print("Total machine time for unsolved: {}\n".format(total_machine_time_for_unsolved))
print("Total num triv: {}".format(num_triv))
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

student_solved_times.sort()
student_unsolved_times.sort()

student_solved_times = list(filter(lambda x: x < 1000, student_solved_times))
student_unsolved_times = list(filter(lambda x: x < 1000, student_unsolved_times))
x = student_solved_times + student_unsolved_times
#print(student_solved_times)
#print(student_unsolved_times)

# IF I GET HERE, I WANT TO PLOT HOW LONG STUDENTS TOOK for each


mu, sigma = 100, 15
#x = mu + sigma*np.random.randn(10000)

# the histogram of the data
n, bins, patches = plt.hist(student_solved_times, 'auto', label='InFix Solved', cumulative=True, density=True, facecolor='blue', alpha=0.75)
n, bins, patches = plt.hist(student_unsolved_times, 'auto', label='InFix Unsolved', cumulative=True, density=True, facecolor='red', alpha=0.75)
#n, bins, patches = plt.hist(x, 'auto', cumulative=True, density=True, facecolor='green', alpha=0.75)
plt.legend(loc='upper left')

# add a 'best fit' line

plt.xlabel('Seconds for novice to fix')
plt.ylabel('Cumulative Probability')
#plt.title('Histogram showing how long novice input bugs take to fix')
plt.axis([0, 500, 0, 1])
plt.grid(True)

plt.show()

print(stats.mean(student_solved_times))
print(stats.mean(student_unsolved_times))
print(statistics.stdev(student_solved_times))
print(statistics.stdev(student_unsolved_times))
print(len(student_solved_times))
print(len(student_unsolved_times))
print(stats.t_score(student_solved_times, student_unsolved_times))
print(stats.degrees_of_freedom(student_solved_times, student_unsolved_times))

