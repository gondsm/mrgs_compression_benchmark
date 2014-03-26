/*********************************************************************
*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2014, ISR University of Coimbra.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the ISR University of Coimbra nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
* Author: Gonçalo S. Martins, 2014
*********************************************************************/

#include "compression_test.h"

#include <sstream>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>

bool LoadDataset(std::string filename, Dataset * dataset){
  // Variable declarations
  std::ifstream infile(filename.c_str());
  std::string line;
  dataset->name = filename;

  // Check if file opened correctly
  if(!infile.is_open()) {
    // Error!
    return false;
  }
  
  // Read file into vector line by line
  while(std::getline(infile,line)) {
    // Break line into tokens
    std::istringstream line_stream(line);
    std::string element;
    // Create vector of tokens
    std::vector<std::string> tokens;
    while(std::getline(line_stream, element, ' ')) {
      tokens.push_back(element);
    }   
    // Reserve space for new elements
    dataset->bytes.reserve(dataset->bytes.size()+tokens.size());
    // Copy data
    for(int i = 0; i < tokens.size(); i++)
      dataset->bytes.push_back(atoi(tokens.at(i).c_str()));
  }

  // Close file and return
  infile.close();
  return true;
}

bool LoadDatasetPGM(std::string filename, Dataset* dataset){
  // Variable declarations
  std::ifstream infile(filename.c_str());
  std::string line;
  dataset->name = filename;
  
  // Check if file opened correctly
  if(!infile.is_open()) {
    // Error!
    return false;
  }
  
  /// Read file
  // Header
  std::vector<std::string> header;
  std::string comment;
  bool last_whitespace_newline = true;
  int string_counter = 0;
  while(header.size() < 4) { // The PGM header contains 4 values
    // Check for comments
    if(last_whitespace_newline == true)
      if(infile.peek() == '#')
        getline(infile, comment, '\n');
    // Get value char by char
    header.push_back(std::string());
    while(infile.peek() != '\n' && infile.peek() != '\t' && infile.peek() != ' ') {
      header.at(string_counter) += infile.get();
    }
    
    string_counter++;
    
    // Burn whitespace
    while(infile.peek() == '\n' || infile.peek() == '\t' || infile.peek() == ' ') {
      if(infile.get() == '\n') last_whitespace_newline = true;
    }
  }
  
  // Content
  if(atoi(header.at(3).c_str()) > 255) 
    return false;
  char in;
  while(infile) {
    in = infile.get();
    if(in != '\n') dataset->bytes.push_back(in);
    else if(infile.peek() == '#')
      getline(infile, comment, '\n');
  }
  
  // We're done.
  infile.close();
  return true;
}

void FillInTimes(std::vector<double> compress_times, 
                 std::vector<double> decompress_times, Results* results) {
  // Average compression time
  double avg = 0;
  for(int i = 0; i < compress_times.size(); i++) {
    avg += compress_times.at(i);
  }
  avg = avg/compress_times.size();
  results->avg_time_compression = avg;
  
  // Standard deviation of compression time
  double aux = 0;
  for(int i = 0; i < compress_times.size(); i++)
    aux += (compress_times.at(i) - avg)*(compress_times.at(i) - avg);
  results->std_deviation_compression = sqrt(((double)1/(double)compress_times.size()) * aux);
  
  // Average decompression time
  avg = 0;
  for(int i = 0; i < decompress_times.size(); i++) {
    avg += decompress_times.at(i);
  }
  avg = avg/decompress_times.size();
  results->avg_time_decompression = avg;
  
  // Standard deviation of decompression time
  aux = 0;
  for(int i = 0; i < decompress_times.size(); i++)
    aux += (decompress_times.at(i) - avg)*(decompress_times.at(i) - avg);
  results->std_deviation_decompression = sqrt(((double)1/(double)decompress_times.size()) * aux);
}

void WriteResults(std::vector<Results>& results, std::string filename, bool print_to_terminal){
  // Declarations
  int col_width = 18;
  std::ofstream results_file(filename.c_str());
  
  // Setup csv header:
  results_file << "technique_name,dataset_name,dataset_size,num_iterations,"
               << "ratio,average_compression_time_ms,std_compression_time_ms,"
               << "average_decompression_time_ms,std_decompression_time_ms"
               << std:: endl;
  
  for(int i = 0; i < results.size(); i++)
  {
    // Names
    if(print_to_terminal == true)
    {
      std::cout << std::left << "Name: '" << results.at(i).technique_name << "'. "; 
      std::cout << std::left << "Dataset: '" << results.at(i).dataset_name <<"'.";
      std::cout << std::endl;
      std::cout << std::left << "Dataset length: " << results.at(i).dataset_size << " bytes. ";
      std::cout << std::left << "Iterations: " << results.at(i).num_iterations << ".";
      std::cout << std::endl;
    }
    
    // Detect errors
    if(results.at(i).error != 0) {
      switch(results.at(i).error) {
      case 1:
        if(print_to_terminal == true)
          std::cout << "This technique found an error while testing." << std::endl;
        break;
      case 2:
        if(print_to_terminal == true)
          std::cout << "There was a data mismatch error in this technique's output" << std::endl;
        break;
      }
      continue;
    }
    
    // CSV
    results_file << results.at(i).technique_name << ","
                 << results.at(i).dataset_name << ","
                 << results.at(i).dataset_size << ","
                 << results.at(i).num_iterations << ","
                 << results.at(i).compression_ratio << ","
                 << results.at(i).avg_time_compression*1000 << ","
                 << results.at(i).std_deviation_compression*1000 << ","
                 << results.at(i).avg_time_decompression*1000 << ","
                 << results.at(i).std_deviation_decompression*1000 << ","
                 << std::endl;
    
    if(print_to_terminal == true)
    {
      // Heading
      std::cout.width(col_width-5);
      std::cout << std::left << "Comp. Ratio";
      std::cout.width(col_width);
      std::cout << std::left << "Avg t (comp)[ms]";
      std::cout.width(col_width);
      std::cout << std::left << "Avg t (decomp)[ms]";
      std::cout << std::endl;
      // Results
      std::cout.width(col_width-5);
      std::cout << std::left << results.at(i).compression_ratio;
      std::cout.width(col_width);
      std::cout << std::left << results.at(i).avg_time_compression*1000;
      std::cout.width(col_width);
      std::cout << std::left << results.at(i).avg_time_decompression*1000;
      std::cout << std::endl;
      // Blank line, for readability
      std::cout << std::endl;
    }
  }
}
