# Authors: 
> Par, Cyrus Gelo M. \
> Aguitez, Franc Roger Glason P.



# Nearest/Closest String Problem
### Given _n-input strings_ of string length _m_, find the nearest string that is closest to all input strings

# Files:
- `near_string_brute.c`
  > Contains the brute-force method of finding the nearest string \
  > Should return the **_actual_** nearest string of the given input strings
- `near_string_non_brute.c`
  > Contains the non-brute-force method of finding the nearest string \
  > _May_ return the actual nearest string of the given input range \
  > Only takes into account the characters present at a specific position among the input strings
- `makefile`
  > Contains the functions that could be used to run the program/s in an easy manner
  > Used for test automation for gathering data

# Running the Program:
### Things to remember:
- variables/options enclosed in "<>" are **required**, i.e. `./a.out sample.txt`
- variables/options enclosed in "[]" are **optional**
  - the program would still run without those, i.e. `./a.out sample.txt -v`
- if you plan on using the `make` command, make sure to place/use input files in the `input_files` folder
### Not using the `make` command:
- if you opted to run the program in a _primitive_ fashion, follow these instructions carefully:
  1. Compile the program: `gcc near_string_<brute/non_brute>.c` (choose one of the two enclosed within "<>") \
  2. After compiling successfully type `./a.out <config_file> [-v]`, (adding `-v` would make the program output a more detailed status while running) \
  3. If the program successfully started, wait for a few moments for it to finish. Then, the key details would be seen at the terminal:
  - if not in the verbose option:
    - `time elapsed` and `closest string` would be seen if you ran the _non-brute-force_ file
    - `time elapsed`, `closest string`, and `best cost` would be seen when you ran the _brute force_ file
  - if in the verbose option:
    - the last lines would show the details listed beforehand
    - for the brute force file:
      - additional lines containing `<string_compared_with> -> <hamming_distance or cost> : <best_string after comparison>` would be present
    - for the non-brute force file:
      - blocks containing the column (character index), `'<character>' : <frequency>, <estimated hamming distance>` can also be seen
### Using the `make` command:
- Running `make run [TYPE = brute or non_brute] [V  =0 or 1] [TXT = <valid input file>]` would automatically compile and run the program 
   - default values for TYPE is brute, V is 0, and TXT is `3_strings_4.txt` 
   - outputs on the terminal would be the same as when compiling and running normally 
- Running `make run_all [V = 0 or 1]` would automatically run the program using the default input/test files found in the `input_files` directory 
   - it is _highly suggested_ to not use verbose logging when using this command
### Runtime:
- The runtime of the program gets saved to `runtimes.csv` after every successful run
  - the rows are in this format `<num_strings>,<string_length>,<elapsed_time>`
