% Determine the number of lines in results file:
[status, result] = system( ['wc -l ', 'results.csv'] );
token = strtok(result, ' ');

% Import results
[technique_name,dataset_name,dataset_size,num_iterations,ratio,average_compression_time_ms,std_compression_time_ms,average_decompression_time_ms,std_decompression_time_ms] = importresults('results.csv', 2, str2num(token(1)));

% Determine all the different datasets and their indices

% Create graphs (for each dataset)

%%
bar3([1:4],cat(2,ratio(1:4)./average_compression_time_ms(1:4),ratio(5:8)./average_compression_time_ms(1:4)))
set(gca,'Xtick',1:4,'YTickLabel',technique_name(1:4), 'XTickLabel', [dataset_name(1), dataset_name(5)])
saveas(gcf, 'ceans.eps')