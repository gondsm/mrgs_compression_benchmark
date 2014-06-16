%% This code was authored by Gonçalo S. Martins, and is hereby released into public domain.

%% Import data from text file.
clear all
% Auto-generated by MATLAB
filename = 'all_100.csv';
%filename = 'small_100.csv';
delimiter = ',';
startRow = 2;

formatSpec = '%s%s%f%f%f%f%f%f%f%[^\n\r]';

fileID = fopen(filename,'r');

dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'HeaderLines' ,startRow-1, 'ReturnOnError', false);

fclose(fileID);

technique_name = dataArray{:, 1};
dataset_name = dataArray{:, 2};
dataset_size = dataArray{:, 3};
num_iterations = dataArray{:, 4};
ratio = dataArray{:, 5};
average_compression_time_ms = dataArray{:, 6};
std_compression_time_ms = dataArray{:, 7};
average_decompression_time_ms = dataArray{:, 8};
std_decompression_time_ms = dataArray{:, 9};

% Clear temporary variables
clearvars filename delimiter startRow formatSpec fileID dataArray ans;

% Names, Indices
% Get dataset names and indices
temp = dataset_name;
dataset_name_diff = {};
while length(temp) > 0
    dataset_name_diff{end+1} = temp{1};
    temp(strcmp(temp, temp{1})) = [];
end
clear temp;
dataset_indices = {};
for i = 1:length(dataset_name_diff)
    dataset_indices{i} = strcmp(dataset_name, dataset_name_diff{i});
end

% Get technique names and indices
temp = technique_name;
technique_name_diff = {};
while length(temp) > 0
    technique_name_diff{end+1} = temp{1};
    temp(strcmp(temp, temp{1})) = [];
end
clear temp;
technique_indices = {};
for i = 1:length(technique_name_diff)
    technique_indices{i} = strcmp(technique_name, technique_name_diff{i});
end

dataset_sizes = [753078 1280342 1929232 9732154 49561658];
temporal_efficiency = ratio ./ (average_compression_time_ms + average_decompression_time_ms);


%% Plot results
% All datasets
markers = {'+','o','x','s','d','^','v','>','<','p','h'};
colors = ['c', 'm', 'r', 'g', 'b', 'k'];
colors = repmat(colors, 1, 2);
figure('Color', [1 1 1], 'Position', [155 41 512 620]);
hold all;
for i = 1:length(technique_name_diff)
    h(i) = plot(dataset_sizes, temporal_efficiency(strcmp(technique_name, technique_name_diff{i})), sprintf('%s%s', markers{i}, colors(i)), 'markers', 10, 'linewidth', 1);
    plot(dataset_sizes, temporal_efficiency(strcmp(technique_name, technique_name_diff{i})), sprintf('%s%s', ':', colors(i)), 'linewidth', 1);
end
set(gca, 'yscale', 'log') 
set(gca, 'xscale', 'log')
axis([dataset_sizes(1)-100000 dataset_sizes(end) 10e-3 150])
ylabel('Temporal Efficiency (E)')
xlabel('Dataset Size (Bytes)')
legend(h, technique_name_diff, 'Location', 'NorthEast')
%export_fig efficiency.eps -eps

figure('Color', [1 1 1], 'Position', [155 41 512 620]);
hold all;
for i = 1:length(technique_name_diff)
    h(i) = plot(dataset_sizes, ratio(strcmp(technique_name, technique_name_diff{i})), sprintf('%s%s', markers{i}, colors(i)), 'markers', 10, 'linewidth', 1);
    plot(dataset_sizes, ratio(strcmp(technique_name, technique_name_diff{i})), sprintf('%s%s', ':', colors(i)), 'linewidth', 1);
end
set(gca, 'xscale', 'log')
axis([dataset_sizes(1)-100000 dataset_sizes(end) 0 200])
ylabel('Ratio (R)')
xlabel('Dataset Size (Bytes)')
legend(h, technique_name_diff, 'Location', 'NorthWest')
%export_fig ratio.eps -eps

%% Mean Bars
clear avg_E avg_R
for i = 1:length(technique_name_diff)
    avg_E(i) = mean(temporal_efficiency(strcmp(technique_name, technique_name_diff{i})));
    avg_R(i) = mean(ratio(strcmp(technique_name, technique_name_diff{i})));
end

figure('Color', [1 1 1]);
bar3(avg_E)
set(gca,'Ytick',[1:length(technique_name_diff)], 'YTickLabel', technique_name_diff)
zlabel('Mean Temporal Efficiency')
export_fig mean_efficiency.eps -eps

figure('Color', [1 1 1]);
bar3(avg_R)
set(gca,'Ytick',[1:length(technique_name_diff)], 'YTickLabel', technique_name_diff)
zlabel('Mean Compression Ratio')
export_fig mean_ratio.eps -eps