# InFix

This research project aims to automatically fix error-inducing command-line inputs for novice Python programs. The main dataset used for both the development and research of this project comes from Python Tutor, a free online interpreter that helps novices visualize their code. In general, InFix finds repairs using a template-based randomized search algorithm. The work is explained in detail in the research paper _InFix: Automatically Repairing Novice Program Inputs, ASE 2019_. Please refer to the paper for more detailed information about the algorithm and insights behind InFix. You may also contact Madeline Endres (endremad@umich.edu) if you have questions about the work.


## Quick Start


Unfortunately, we cannot resease the original data used in the ASE paper due to privacy concerns. However, as of spring 2022, we have added a small subset of example programs. To run InFix on these example programs, run the following from the source folder:

`python3.5 driver.py ../test_config_inputs/example_test_config.json`

This will run InFix with 2 different random seeds for each program. Each run will have 6 probes. There is an example log of a run of this program available in the exampleLogs folder. We note that this number of side-by-side runs and number of probes in this example is lower than that in the ASE paper evaluation.

### Package Dependencies 

InFix was written and evaluated using Python 3.5.2.

Some of the evaluation modules rely on [Coverage.py](https://coverage.readthedocs.io/en/v4.5.x/).

### Running InFix

To get started, you will need to make a configuration file. Sample configuration files can be found in `test_config_inputs` In particular, `state\_machine\_with\_coverage\_test\_config.json` is a good starting point. The configuration file is a json file that includes information such as where the programs and buggy inputs are located, which theory you are using for fixing the inputs (*StateMachine* is the only theory currently implemented), timeout information, logging information, etc. The script *generate\_config\_file.py* can be used to generate configuration files.

Each input that InFix tries to repair must be in its own folder with its name, *X*, acting as a unique identifier. The default implementation requires that folder *X* must have two files in it: a file with the Python program code named *X\_code.py*, and a json file with the name *X\_additional\_info.json*. This json file should contain a json object of the form:

```json
{
  "UniqueId": "Unique input id X",
  "BadInput": "List containing the buggy input. Each list element represents a single line of input before a newline. e.g.  ['2', '66 42']",
  "ErrorType": "Contains just the error message type. E.g. 'ValueError'",
  "ErrorMessage": "Contains the full initial error message",
  "LastIsEmpty": "Flag indicating if the last line of the entered input was blank. This was used to focus investigations on certain types of inputs.
}
```

Additional information may need to be added to the json for more specific use cases or evaluations. For example, some of the evaluation scripts use the fields `CorrectInputs`, `TimeStamp`, `ip`, and `CoverageInfo`.

Once you have set up your inputs to be repaired and you have a config file, you are ready to run InFix. To do so, run from the *source* directory: `python3 driver.py config\_file.json`.

The run can be evaluated using `scripts\\\_analyze_new_log_format.py run\_logfile\_directory` by modifying a couple hardcoded paths leading to your buggy input directory.

For more complicated uses, you may have to create your own driver function and substantially modify some of the analysis scripts.
