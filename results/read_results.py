# -*- coding: utf-8 -*-
"""
Created on Mon Jun 16 18:39:52 2014

@author: Gonçalo Martins
"""

from __future__ import print_function
from __future__ import division
import matplotlib.pyplot as plot
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
# (I'm assuming the same datasets were used for all techniques, which is a reasonable assumption)
num_datasets = 0
for row in sorted_rows:
    if row[0] == sorted_rows[0][0]:
        num_datasets+=1
    else:
        break

# Sizes of datasets
dataset_sizes = [row[2] for row in sorted_rows[0:num_datasets]]

# Names of techniques
technique_names = [row[0] for row in sorted_rows[0:len(sorted_rows):num_datasets]]

# Build vectors
# Ratio
ratio = []
for i in range(0, len(technique_names)):
    ratio.append([float(row[4]) for row in sorted_rows[i*num_datasets:(i+1)*num_datasets]])

# Temporal efficiency
efficiency = []
for i in range(0, len(technique_names)):
    efficiency.append([float(row[4])/(float(row[5])+float(row[7])) for row in sorted_rows[i*num_datasets:(i+1)*num_datasets]])

# Mean ratio for smaller datasets
mean_ratio = []
num_smaller_datasets = 3
for i in range(0, len(technique_names)):
    local_ratio = [float(row[4]) for row in sorted_rows[i*num_datasets: i*num_datasets+num_smaller_datasets]]  
    mean_ratio.append(sum(local_ratio)/len(local_ratio))

# Mean temporal efficiency for smaller datasets
mean_efficiency = []
for i in range(0, len(technique_names)):
    local_efficiency = [float(row[4])/(float(row[5])+float(row[7])) for row in sorted_rows[i*num_datasets: i*num_datasets+num_smaller_datasets]]  
    mean_efficiency.append(sum(local_efficiency)/len(local_efficiency))

# Plot Stuff
markers = 'ox' # We will alternate between these two markers and, since colors vary, will get enough combinations to show all data.

# Plot Ratio
fig_ratio = plot.figure(figsize=[5,7])
plot.hold(True)
for i in range(0, len(ratio)):
    plot.plot(dataset_sizes, ratio[i], ''.join(['--', markers[i%len(markers)]]))
plot.legend(technique_names, loc='upper center', fontsize='small', labelspacing=0.1, ncol=2, fancybox=True, bbox_to_anchor=(0.5, 1.07))
plot.gca().set_xscale('log')
plot.grid(True, alpha=0.2, which='both')
plot.autoscale(True, axis='x', tight='True')
plot.tick_params(labelsize='small')
plot.xlabel('Dataset Size (Bytes)', fontsize='small')
plot.ylabel('Compression Ratio (R)', fontsize='small')
plot.savefig('ratio.pdf', dpi=1000, bbox_inches='tight')
plot.close(fig_ratio)

# Plot Efficiency
fig_eff = plot.figure(figsize=[5,7])
plot.hold(True)
for i in range(0, len(efficiency)):
    plot.plot(dataset_sizes, efficiency[i], ''.join(['--', markers[i%len(markers)]]))
plot.legend(technique_names, loc='upper center', fontsize='small', labelspacing=0.1, ncol=2, fancybox=True, bbox_to_anchor=(0.5, 1.07))
plot.gca().set_yscale('log')
plot.gca().set_xscale('log')
plot.grid(True, alpha=0.2, which='both')
plot.autoscale(True, axis='x', tight='True')
plot.tick_params(labelsize='small')
plot.xlabel('Dataset Size (Bytes)', fontsize='small')
plot.ylabel('Temporal Efficiency (E)', fontsize='small')
plot.savefig('efficiency.pdf', dpi=1000, bbox_inches='tight')
plot.close(fig_eff)

# Efficiency vs Ratio
#fig_vs = plot.figure()
#plot.hold(True)
#for i in range(len(efficiency)):
#    plot.plot(mean_ratio[i]/max(mean_ratio), mean_efficiency[i]/max(mean_efficiency), markers[i%len(markers)], markersize = 10)
#plot.ylabel('Normalized Mean Temporal Efficiency (E)')
#plot.xlabel('Normalized Mean Compression Ratio')
#plot.legend(technique_names)
#plot.show()

# Mean ratio for 3 smaller datasets
fig_mean_ratio = plot.figure()
h = plot.bar(range(10),mean_ratio)
xticks_pos = [0.65*patch.get_width() + patch.get_xy()[0] for patch in h]
plot.xticks(xticks_pos, technique_names, rotation = 30, ha = 'right', fontsize='x-large')
plot.grid(axis = 'y', alpha=0.3)
plot.autoscale(True, axis='x', tight='True')
plot.gca().set_axisbelow(True) # So that bars are on top of grid
plot.ylabel(r'Mean Compression Ratio', fontsize='x-large')
plot.savefig('mean_ratio.pdf', dpi=1000, bbox_inches='tight')
plot.close(fig_mean_ratio)

# Mean efficiency for 3 smaller datasets
fig_mean_efficiency = plot.figure()
h = plot.bar(range(10),mean_efficiency)
xticks_pos = [0.65*patch.get_width() + patch.get_xy()[0] for patch in h]
plot.xticks(xticks_pos, technique_names, rotation = 30, ha = 'right', fontsize='x-large')
plot.grid(axis = 'y', alpha=0.3)
plot.autoscale(True, axis='x', tight='True')
plot.gca().set_axisbelow(True) # So that bars are on top of grid
plot.ylabel(r'Mean Temporal Efficiency', fontsize='x-large')
plot.savefig('mean_efficiency.pdf', dpi=1000, bbox_inches='tight')
plot.close(fig_mean_efficiency)