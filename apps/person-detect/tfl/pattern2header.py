#!/usr/bin/env python

import os
import sys
import random
import csv

#from past.builtins import execfile

if (len(sys.argv) > 1) :
    filename = sys.argv[1]
    headername = 'fourbyte.h'
else :
    filename = 'FourBytePattern.ArcticPro2.eth_cnt16'
    headername = 'fourbyte.h'
pattern_file = open(filename, "r")
header_file  = open(headername, "w")

#First 3 lines don't care
pattern_rows = pattern_file.readline()
pattern_rows = pattern_file.readline()
pattern_rows = pattern_file.readline()


#pattern starts here
pattern_rows = pattern_file.readlines()


header_file.write("#ifndef FOURBYTE_HEADER\n#define FOURBYTE_HEADER\n\n")

header_file.write("#define NLINES " + str(len(pattern_rows)) +"\n")


header_file.write("PULP_L2_DATA static unsigned int fb_cfg_data[NLINES] = {\n\n")


for row in  pattern_rows:


    values = row.split(" ")
    #pattern_rows = pattern_file.readline()
    header_file.write("\t0x" + values[0] + ",\n")


header_file.write("};\n\n#endif")



pattern_file.close()
header_file.close()


exit()
first_line = QL_CSV[0]
#del QL_CSV[0]
#QL_CSV.sort(key=lambda x: (x[0], x[2], x[4]))

print first_line
for row in QL_CSV:
    print(row)


#QL_CSV.insert(0,first_line)


f = open('ETH_csv.csv', 'wb')

for row in QL_CSV:
    for i in range (0,len(row)-1):
       f.write(row[i] + ',')
    f.write(row[-1])
    f.write('\n')
f.close()

#    spamwriter = csv.writer(csvfile, delimiter=',', quotechar='|')
#    spamwriter.writerow(first_line)
#    for row in QL_CSV:
#        spamwriter.writerow(row)


########## REMAINING PINS


print("remaining_pins_F2A")
print(remaining_pins_F2A)

print("remaining_pins_A2F")
print(remaining_pins_A2F)


print("F2A")
print(F2A)

print("A2F")
print(A2F)

print("init_F2A")
print(init_F2A)

print("init_A2F")
print(init_A2F)


