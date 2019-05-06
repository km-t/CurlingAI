import glob
import os
import re
file_list = glob.glob('*.csv')
sumfile = "logs.csv"

for filename in file_list:
    print(filename)
    with open(filename, 'r') as f:
        lines = f.readlines()

    with open(sumfile, "a") as ff:
        ff.writelines(lines)
