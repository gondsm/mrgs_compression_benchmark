# -*- coding: utf-8 -*-
"""
Created on Mon Jun 16 18:39:52 2014

@author: Gonçalo Martins
"""

import __future__
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
for row in rows:
    