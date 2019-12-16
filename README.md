# CSC 481 Project

## Setup
For initial setup, `cd` to `/root/emp-sh2pc/`,
delete the `test` directory and `git clone this-repo test`.

## Test
We provide a Python script `test.py` to test our solutions.

For **Edit Distance**, the script randomly generates two valid strings
and call the *edit_distance* binary to compute the edit distance.
Use `./test.py ed` to run the tests.

For **Longest Common Subsequence**, the script randomly generates two
valid strings and call the *common_subsequence* binary to compute the result.
Use `./test.py lcs` to run the tests.

For **Primeness**, the script generates randomly generates two numbers
less than 1,000,000 and compare their primeness.
Use `./test/py primeness` to run the tests.
