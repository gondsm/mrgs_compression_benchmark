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
  // Import dataset
  Dataset d1;
  LoadDataset(std::string("datasets/intel1000.txt"), &d1);

  // Declare output vector
  std::vector<Results> result_vector;

  // Test stuff
  int n_iter = 50;
  Results lz4_results = lz4Test(n_iter, d1);
  result_vector.push_back(lz4_results);
  Results deflate_results = deflateTest(n_iter, d1);
  result_vector.push_back(deflate_results);
  
  // Section to make a display function out of:
  /*std::cout << "LZ4 ratio: " << lz4_results.compression_ratio << std::endl;
  std::cout << "LZ4 mean compression time: " << lz4_results.avg_time_compression*1000 << "ms." << std::endl;
  std::cout << "LZ4 mean decompression time: " << lz4_results.avg_time_decompression*1000 << "ms." << std::endl;
  std::cout << "LZ4 std compression time: " << lz4_results.std_deviation_compression*1000 << "ms." << std::endl;
  std::cout << "LZ4 std decompression time: " << lz4_results.std_deviation_decompression*1000 << "ms." << std::endl;*/
  
  WriteResults(result_vector);

  // Return
  return 0;
}

