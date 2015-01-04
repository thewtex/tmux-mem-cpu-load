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

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h> // sleep
#include <cmath> // for floorf
#include <cstdlib> // EXIT_SUCCESS

// Tmux color lookup tables for the different metrics.
#include "luts.h"

#if defined(__APPLE__) && defined(__MACH__)
   // Apple osx system
   #include "osx/cpu.h"
   #include "osx/memory.h"
   #include "osx/load.h"
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
   // BSD system
   // TODO: Includes and *BSD support
   #define BSD_BASED 1
   // include _get_cpu_percentage (see osx/cpu.cc)
   // include cpu_percentage (see osx/cpu.cc)
#else
   // assume linux system
   #include "linux/cpu.h"
   #include "linux/memory.h"
   #include "linux/load.h"
#endif

#include "graph.h"

// Function declarations.
// TODO: those should stay in separate headers
//    LINUX: DONE/partial
//    OSX:   DONE/partial
//    BSD:   TODO

std::string cpu_string( unsigned int cpu_usage_delay,
      unsigned int graph_lines, bool use_colors = false ) {
  
  float percentage;

   //output stuff
  std::ostringstream oss;
  oss.precision( 1 );
  oss.setf( std::ios::fixed | std::ios::right );

  // get %
  percentage = cpu_percentage( cpu_usage_delay );

  if( use_colors )
    oss << cpu_percentage_lut[static_cast<unsigned int>( percentage )];
   
  oss << "[";
  oss << getGraphByPercentage( unsigned(percentage), graph_lines );
   oss << "]";
  oss.width( 5 );
  oss << percentage;
  oss << "%";
  if( use_colors )
    oss << "#[fg=default,bg=default]";

  return oss.str();
}

int main(int argc, char** argv) {
  unsigned int cpu_usage_delay = 900000;
  int graph_lines = 10;
  bool use_colors = false;
  try {
    std::istringstream iss;
    iss.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    std::string current_arg;
    unsigned int arg_index = 1;
    if( argc > arg_index )
      {
      if( strcmp( argv[arg_index], "--colors" ) == 0 )
        {
        use_colors = true;
        ++arg_index;
        }
      }
    if( argc > arg_index )
      {
      iss.str( argv[arg_index] );
      int status_interval;
      iss >> status_interval;
      if( status_interval < 1 )
        {
        std::cerr << "Status interval argument must be one or greater." << std::endl;
        return EXIT_FAILURE;
        }
      cpu_usage_delay = status_interval * 1000000 - 100000;
      ++arg_index;
      }
    if( argc > arg_index )
      {
      iss.str( argv[arg_index] );
      iss.clear();
      iss >> graph_lines;
      if( graph_lines < 1 )
        {
        std::cerr << "Graph lines argument must be one or greater." << std::endl;
        return EXIT_FAILURE;
        }
      }
    }
  catch(const std::exception &e)
    {
    std::cerr << "Usage: " << argv[0] << " [--colors] [tmux_status-interval(seconds)] [graph lines]" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << mem_string( use_colors ) << ' ' << cpu_string( cpu_usage_delay, graph_lines, use_colors ) << ' ' << load_string( use_colors );

  return EXIT_SUCCESS;
}

