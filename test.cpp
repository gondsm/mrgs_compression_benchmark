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
#include <vector>
#include <iostream>

int main() {
  /// Import datasets
  // Declare necessary vectors
  std::vector<std::string> dataset_names;
  std::vector<Dataset> datasets;
  
  // Add vectors (add your datasets here!)
  //dataset_names.push_back(std::string("datasets/intel1000.txt"));
  //dataset_names.push_back(std::string("datasets/intel.txt"));
  dataset_names.push_back(std::string("maps/mapa1.pgm"));
  dataset_names.push_back(std::string("maps/mapa2.pgm"));
  
  // Allocate necessary space in dataset vector
  while(datasets.size() < dataset_names.size()) {
    Dataset empty;
    datasets.push_back(empty);
  }
  
  // Load datasets
  for(int i = 0; i < dataset_names.size(); i++) {
    //LoadDataset(dataset_names.at(i), &datasets.at(i));
    LoadDatasetPGM(dataset_names.at(i), &datasets.at(i));
  }
  
  Dataset newset;
  LoadDataset(std::string("datasets/intel.txt"), &newset);
  datasets.push_back(newset);
  LoadDataset(std::string("datasets/intel1000.txt"), &newset);
  datasets.push_back(newset);
  
  /// Test algorithms
  // Declare output vector
  std::vector<Results> result_vector;
  
  // Test stuff
  int n_iter = 50;
  for(int i = 0; i < datasets.size(); i++) {
    result_vector.push_back(lz4Test(n_iter, datasets.at(i)));
    result_vector.push_back(deflateTest(n_iter, datasets.at(i)));
    result_vector.push_back(deflateTestFast(n_iter, datasets.at(i)));
    result_vector.push_back(deflateTestSlow(n_iter, datasets.at(i)));
    result_vector.push_back(lzmaTest(n_iter, datasets.at(i)));
    result_vector.push_back(lzmaTestFast(n_iter, datasets.at(i)));
    result_vector.push_back(lzmaTestSlow(n_iter, datasets.at(i)));
    result_vector.push_back(quicklzTest(n_iter, datasets.at(i)));
  }
  
  // Write results
  WriteResults(result_vector);

  /// Done!
  return 0;
}

