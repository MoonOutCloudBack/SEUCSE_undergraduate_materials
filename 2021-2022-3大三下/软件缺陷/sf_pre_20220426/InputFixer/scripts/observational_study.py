# This file is being used to check the input specific data

# Load them all

import sys
import json
import glob

"""
with open(sys.argv[-1], 'r') as f:
    j = json.load(f)

# Program length
print(len(j))
"""
"""
Calculates total, nonblank and net lines of code for Python scripts.
"""
import os
import re

def get_line_count(blob):
    """Returns the number of lines of code"""
    return len(blob.split('\n'))

def strip_docstring(blob):
    """Removes docstrings from code"""
    docstring = True
    while docstring == True:
        match_docstring = re.search('\n\s*"""[^"""]*"""', blob)
        if not match_docstring:
            docstring = False
        else:
            blob = blob.replace(blob[match_docstring.span()[0]:match_docstring.span()[1]], '')
    return blob

def strip_blanklines(blob):
    """Strips blank lines from the code"""
    lines = blob.split('\n')
    return '\n'.join([line for line in lines if line.strip() != ''])

def strip_comments(blob, delim='#'):
    """Strips comments from the code"""
    lines = blob.split('\n')
    return '\n'.join([line for line in lines if line.strip()[0] != delim])

def loc(blob, delim='#'):
    """Returns the total line count, nonblank line count, and net line count excluding comments and docstrings"""
    total = get_line_count(blob)
    blob = strip_blanklines(blob)
    nonblank = get_line_count(blob)
    blob = strip_docstring(blob)
    blob = strip_comments(blob, delim)
    net = get_line_count(blob)
    num_inputs = len([m.start() for m in re.finditer('input ?\(', blob)])
    return { 'total': total, 'nonblank': nonblank, 'net': net, 'num_inputs': num_inputs}

def get_folder_total(path):
    """Returns the total, nonblank and net loc for all the python files in a directory"""
    files = os.listdir(path)
    pythonfiles = ['%s/%s' % (path, filename) for filename in files if filename[-3:] == '.py']
    total = { 'net': 0, 'total': 0, 'nonblank': 0, 'num_inputs':0 }
    for filename in pythonfiles:
        with open(filename, 'r') as thisfile:
            blob = thisfile.read()
            # print filename
            thisloc = loc(blob)
            for k, v in thisloc.items():
                total[k] += v
    return total


def get_num_unique_programs_and_users(path, scenarios):
    """Returns the total, nonblank and net loc for all the python files in a directory"""
    unique_programs = set()
    valid_u_ids = set()
    for scenario in scenarios:
        valid_u_ids.add(scenario[0]["UniqueId"])

    for filename in glob.iglob(path + 'PythonTutor_Input_Data_Sessions_20*/**/*.py', recursive=True):
        print(filename)
        x = filename[filename.rfind('/') + 1:filename.rfind('_')]
        if x not in valid_u_ids: continue
        with open(filename, 'r') as thisfile:
            blob = thisfile.read()
            unique_programs.add(blob)

    valid_ips = set()
    not_valid_ips = set()
    count = 0
    for filename in glob.iglob(path + 'PythonTutor_Input_Data_Sessions_20*/**/*.json', recursive=True):
        x = filename[filename.rfind('/') + 1:filename.rfind('_a')]
        if x not in valid_u_ids: continue
        with open(filename, 'r') as thisfile:
            blob = json.load(thisfile)
            if blob["ip"] in not_valid_ips:
                pass
            elif blob["ip"] in valid_ips:
                valid_ips.remove(blob["ip"])
                count += 1
                not_valid_ips.add(blob["ip"])
            else:
                valid_ips.add(blob["ip"])
    
    print(count)
    print(len(valid_ips))
    return unique_programs



