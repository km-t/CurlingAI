import glob
import os
import re
file_list = glob.glob('*.csv')

for filename in file_list:
    file = re.sub("vec","",filename)
    file = re.sub(".csv","",file)
    file+=","
    print(file)
    with open(filename,'r') as f:
        lines = f.readlines()    
    
    with open(filename, "w") as a:
        a.write("")
    with open(filename, "a")as ff:
        for line in lines:
            line = file + line
            ff.write(line)
