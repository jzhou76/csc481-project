#!/usr/bin/env python3

'''
This script runs a secure computation of an application.
It accepts one argument - the name of the application.
'''

import sys
import os
from random import randint

argv = sys.argv


'''
run_ed() randomly generates two valid strings for the the Edit Distance
problem, and then calls the edit_distance binary to do the secure computation.
'''
def run_ed():
    # Randomly generate two DNS sequences.
    ATGC_num_map = { 1:'A', 2:'T', 3:'G', 4:'C' }
    input_alice, input_bob = "", ""
    for i in range(0, 5):
        input_alice += ATGC_num_map[randint(1, 4)]
        input_bob += ATGC_num_map[randint(1, 4)]

    # Construct the command of running edit_distance
    cmd = "./edit_distance 1 12345 " + input_alice + " & " + \
          "./edit_distance 2 12345 " + input_bob

    print(os.popen(cmd).read())


'''
run_primeness() randomly generates two integers that are less than 1,000,000,
and then calls the primeness binary to do the secure computation.
'''
def run_primeness():
    # Randomly generate two integer less than 1,000,000
    input_alice, input_bob = randint(2, 1000000), randint(2, 1000000)

    cmd = "./primeness 1 12345 " + str(input_alice) + " & " + \
          "./primeness 2 12345 " + str(input_bob)

    print(os.popen(cmd).read())


'''
run_lcs() randomly generates two integers between 10,000 and 99,999,
converts them to strings as input for the lcs binary.
'''
def run_lcs():
    input_alice, input_bob = randint(10000, 99999), randint(10000, 99999)

    cmd = "./lcs 1 12345 " + str(input_alice) + " & " + \
          "./lcs 2 12345 " + str(input_bob)

    print(os.popen(cmd).read())


#
# Entrance of this script
#
if __name__ == "__main__":
    if len(argv) != 2:
        print("Please input an application's name.")
        exit()

    os.chdir("/root/emp-sh2pc/build/bin")

    if argv[1] == "ed":
        run_ed()
    elif argv[1] == "primeness":
        run_primeness()
    elif argv[1] == "lcs":
        run_lcs()
    else:
        print("Please input a correct application: \
                \"ed\", \"primeness\", or \"lcs\"")
