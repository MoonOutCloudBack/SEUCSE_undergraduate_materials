"""
This file contains functions that do static analysis of the code.
This analysis can help with binning and templating
"""
import tokenize
import ast
import parser
from io import BytesIO

def tokenize_program(program_file_name):
    """
    This function returns a list of the tokens in the program
    """
    f = open(program_file_name, 'r', encoding='utf-8')
    s = ''.join([line for line in f])
    print(s)
    f.close()
    g = tokenize.tokenize(BytesIO(s.encode('utf-8')).readline)
    return [x for x in g]

def collect_splits(tokens1):
    """
    This function collects non default split values if they exist as well as any dictionary keys
    and indexs that are literals
    """
    split_types = []
    tokens = iter(tokens1)
    for t, s, _, _, _ in tokens:
        if t == tokenize.NAME and s == "split":
            next(tokens)
            t, s, _, _, _ = next(tokens)
            if t == tokenize.STRING:
                split_types.append(s[1][-1])
            else:
                split_types.append(None)
    return split_types

def has_branches_or_loops_or_splits(tokens):
    """
    This function returns a tuple: noBranches, noLoops
    Both are bools
    """
    has_loop = False
    has_branch = False
    has_split = False
    for t, s, _, _, _ in tokens:
        if t == tokenize.NAME:
            if s == "while" or s == "for":
                has_loop = True
            if s == "if":
                has_branch = True
            if s == "split":
                has_split = True
        if has_loop and has_branch and has_split:
            return True, True, True

    return has_loop, has_branch, has_split

def num_input_calls(tokens):
    # This function returns the number of input calls
    num_calls = 0
    for t, s, _, _, _ in tokens:
        if t == tokenize.NAME and s == "input":
            num_calls += 1
    return num_calls

def collect_string_literals(tokens1):
    """
    This function returns all the string literals in the program in a list
    """
    tokens = iter(tokens1)
    string_literals = []
    two_past_string = ""
    one_past_string = ""
    for t, s, _, _, _ in tokens:
        if t == tokenize.STRING and two_past_string != "print" and two_past_string != "input":
            to_append = s[1:-1]
            if len(to_append) > 0:
                string_literals.append(to_append)
        two_past_string = one_past_string
        one_past_string = s

    return string_literals


def use_grammar_templates(tokens):
    """
    This function returns False if I should just use the error message method
    Otherwise, returns true -> And uses the grammar template?
    """
    has_branch, has_loop, has_split = has_branches_or_loops_or_splits(tokens)
    # Collect the number of sting literals
    #string_literals = collect_string_literals(tokens)

    # If the code is straight line, return False
    if not has_branch and not has_loop and not has_split:
        return False

    

    # Now count the number of inputs in the code
    return True

    # Default is return 
    return False

def try_parsing(program_file_name):
    """
    I am doing this because maybe it will work when the program is correct?
    """
    f = open(program_file_name, 'r')
    s = ''.join([line for line in f])
    print(s)
    f.close()
    tree = ast.parse(s)
    return tree

if __name__ == "__main__":
    import sys
    file_name = sys.argv[-1]
    
    print(collect_splits(file_name))

