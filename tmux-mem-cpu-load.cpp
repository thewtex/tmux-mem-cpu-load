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
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> // EXIT_SUCCESS
#include "argParse/argParse.h"

// Tmux color lookup tables for the different metrics.
#include "luts.h"

#include "version.h"

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


std::string cpu_string(unsigned int cpu_usage_delay, unsigned int graph_lines,
	bool use_colors = false) {

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
  using namespace ArgvParse;

  unsigned cpu_usage_delay = 1000000;
  short graph_lines = 10; // max 32767 should be enough
  bool use_colors = false;

  // Argv parser
  ArgvParser arg;

  // ugly, I know
  std::string intro = "tmux-mem-cpu-load v";
  intro += std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR);
  intro += "." + std::to_string(VERSION_PATCH) + "\n";
  intro += "Usage: tmux-mem-cpu-load [OPTIONS]";

  arg.setIntroduction(intro);

  arg.setHelpOption("h", "help", "Prints this help message");

  // define actual options
  arg.defineOption("colors", "Use tmux colors in output",
	  ArgvParser::NoAttribute);
  arg.defineOption("i", "interval", "set tmux status refresh interval in "
	  "seconds. Default: 1 second", ArgvParser::RequiresValue);
  arg.defineOption("g", "graph-lines", "Set how many lines should be drawn in "
	  "a graph. Default: 10", ArgvParser::RequiresValue);

  int result = arg.parse(argc, argv);

  if (result != ArgvParser::Success) {
	std::cerr << arg.parseErrorDescription(result);
	return EXIT_FAILURE;
  }

  // mangle arguments
  if (arg.foundOption("colors"))
	use_colors = true;

  if (arg.foundOption("interval")) {
	int delay = std::stoi(arg.optionValue("interval"));
	if (delay < 1) {
	  std::cerr << "Status interval argument must be one or greater.\n";
	  return EXIT_FAILURE;
	}
	cpu_usage_delay = delay * 1000000;
  }

  if (arg.foundOption("graph-lines")) {
	graph_lines = std::stoi(arg.optionValue("graph-lines"));
	if( graph_lines < 1 ) {
	  std::cerr << "Graph lines argument must be one or greater.\n";
	  return EXIT_FAILURE;
	}
  }

  std::cout << mem_string( use_colors ) << ' ' 
	<< cpu_string( cpu_usage_delay, graph_lines, use_colors ) << ' ' 
	<< load_string( use_colors );

  return EXIT_SUCCESS;
}

