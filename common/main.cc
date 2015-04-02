/* vim: tabstop=2 shiftwidth=2 expandtab textwidth=80 linebreak wrap
 *
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
 */

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> // EXIT_SUCCESS, atoi()
#include <getopt.h> // getopt_long

#include "version.h"
#include "graph.h"

// Tmux color lookup tables for the different metrics.
#include "luts.h"

#include "cpu.h"
#include "memory.h"
#include "load.h"

std::string cpu_string( unsigned int cpu_usage_delay, unsigned int graph_lines,
	bool use_colors = false )
{

  float percentage;

  //output stuff
  std::ostringstream oss;
  oss.precision( 1 );
  oss.setf( std::ios::fixed | std::ios::right );

  // get %
  percentage = cpu_percentage( cpu_usage_delay );

  if( use_colors )
  {
    oss << cpu_percentage_lut[static_cast<unsigned int>( percentage )];
  }

  if( graph_lines > 0)
  {
    oss << " [";
    oss << get_graph_by_percentage( unsigned( percentage ), graph_lines );
    oss << "]";
  }
  oss.width( 5 );
  oss << percentage;
  oss << "%";
  if( use_colors )
  {
    oss << "#[fg=default,bg=default]";
  }

  return oss.str();
}

void print_help()
{
  using std::cout;
  using std::endl;

  cout << "tmux-mem-cpu-load v" << tmux_mem_cpu_load_VERSION << endl
    << "Usage: tmux-mem-cpu-load [OPTIONS]\n\n"
    << "Available options:\n"
    << "-h, --help\n"
    << "\t Prints this help message\n"
    << "--colors\n"
    << "\tUse tmux colors in output\n"
    << "-i <value>, --interval <value>\n"
    << "\tSet tmux status refresh interval in seconds. Default: 1 second\n"
    << "-g <value>, --graph-lines <value>\n"
    << "\tSet how many lines should be drawn in a graph. Default: 10\n"
    << endl;
}

int main( int argc, char** argv )
{
  unsigned cpu_usage_delay = 990000;
  short graph_lines = 10; // max 32767 should be enough
  bool use_colors = false;

  static struct option long_options[] =
  {
    // Struct is a s follows:
    //   const char * name, int has_arg, int *flag, int val
    // if *flag is null, val is option identifier to use in switch()
    // otherwise it's a value to set the variable *flag points to
    { "help", no_argument, NULL, 'h' },
    { "colors", no_argument, NULL, 'c' },
    { "interval", required_argument, NULL, 'i' },
    { "graph-lines", required_argument, NULL, 'g' },
    { 0, 0, 0, 0 } // used to handle unknown long options
  };

  int c;
  // while c != -1
  while( (c = getopt_long( argc, argv, "hi:g:", long_options, NULL) ) != -1 )
  {
    switch( c )
    {
      case 'h': // --help, -h
        print_help();
        return EXIT_FAILURE;
        break;
      case 'c': // --colors
        use_colors = true;
        break;
      case 'i': // --interval, -i
        if( atoi( optarg ) < 1 )
          {
            std::cerr << "Status interval argument must be one or greater.\n";
            return EXIT_FAILURE;
          }
        cpu_usage_delay = atoi( optarg ) * 1000000 - 10000;
        break;
      case 'g': // --graph-lines, -g
        if( atoi( optarg ) < 0 )
          {
            std::cerr << "Graph lines argument must be zero or greater.\n";
            return EXIT_FAILURE;
          }
        graph_lines = atoi( optarg );
        break;
      case '?':
        // getopt_long prints error message automatically
        return EXIT_FAILURE;
        break;
      default:
        std::cerr << "?? getopt returned character code 0 " << c << std::endl;
        return EXIT_FAILURE;
    }
  }
  // Detect old option specification and return and error message.
  if( argc > optind )
  {
    std::cout <<
      "The interval and graph lines options are now specified with flags.\n\n";
    print_help();
    return EXIT_FAILURE;
  }

  std::cout << mem_string( use_colors )
    << cpu_string( cpu_usage_delay, graph_lines, use_colors )
    << load_string( use_colors );

  return EXIT_SUCCESS;
}

