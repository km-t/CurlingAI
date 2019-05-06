import glob
import os
import re
file_list = glob.glob('*.csv')

for filename in file_list:
    print(filename)
    with open(filename,'r') as f:
        lines = f.readlines()
    with open(filename,'w')as f:
        for line in lines:
            st = re.sub("^[0-9]{11},", "", line)
            f.write(st)