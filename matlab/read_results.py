# -*- coding: utf-8 -*-
"""
Created on Mon Jun 16 18:39:52 2014

@author: Gonçalo Martins
"""

import __future__
import matplotlib
import csv

# Read stuff from the file:
with open('all_100.csv', 'r') as results :
    dialect = csv.Sniffer().sniff(results.read(1024), delimiters=',')
    print dialect.delimiter