# -*- coding: utf-8 -*-
"""
Created on Mon Jun 16 18:39:52 2014

@author: Gonçalo Martins
"""

from __future__ import print_function
from __future__ import division
import matplotlib
import csv

# Read stuff from the file:
first_row = 1
rows = []
with open('all_100.csv', 'r') as results :
    dialect = csv.Sniffer().sniff(results.read(1024), delimiters=',')
    results.seek(0)
    reader = csv.reader(results, dialect)
    for row in reader:
        if first_row == 1:
            header = row
            first_row = 0
        else:
            rows.append(row)

# Sort lines by technique name and dataset size:
sorted_rows = sorted(rows, key=lambda row: (row[0], int(row[2])))

# Write sorted rows to file, for checking
#with open('test.csv', 'w') as testfile:
#    for row in sorted_rows:
#        testfile.write(','.join(row))
#        testfile.write('\n')

# Find out how many datasets were used:
# (I'm assuming the same datasets were used for all techniques)
num_datasets = 0
for row in sorted_rows:
    if row[0] == sorted_rows[0][0]:
        num_datasets+=1
    else:
        break
    
