/*
 * Copyright 2012 Matthew McCormick
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * */
// This file was Authored by Justin Crawford <Justasic@gmail.com>
// Based on: https://github.com/freebsd/freebsd/blob/master/usr.bin/top/machine.c
// Based on: Apple.cpp for load_string/mem_string and apple's documentation


#include <sstream>
#include <string>
#include <stdlib.h> // getloadavg()
#include <cmath> // floorf()

#include "load.h"

// Load Averages
std::string load_string( bool use_colors = false ) {
  std::stringstream ss;
  // Only get 3 load averages
  int nelem = 3;
  double averages[3];
  // based on: opensource.apple.com/source/Libc/Libc-262/gen/getloadavg.c
  if(getloadavg(averages, nelem) < 0)
	ss << "0.00 0.00 0.00"; // couldn't get averages.
  else {
	for(int i = 0; i < nelem; ++i) {
	  // Round to nearest, make sure this is only a 0.00 value not a 0.0000
	  float avg = floorf(static_cast<float>(averages[i]) * 100 + 0.5) / 100;
	  ss << avg << " ";
	}
  }

  return ss.str();
}

