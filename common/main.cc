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

#include "powerline.h"

std::string cpu_string( CPU_MODE cpu_mode, unsigned int cpu_usage_delay, unsigned int graph_lines,
    bool use_colors = false,
    bool use_powerline_left = false, bool use_powerline_right = false )
{

  float percentage;
  float multiplier = 1.0f;

  //output stuff
  std::ostringstream oss;
  oss.precision( 1 );
  oss.setf( std::ios::fixed | std::ios::right );

  // get %
  percentage = cpu_percentage( cpu_usage_delay );

  // set multiplier to number of threads ?
  if ( cpu_mode == CPU_MODE_THREADS )
  {
    multiplier = get_cpu_count();
  }

  // if percentage*multiplier >= 100, remove decimal point to keep number short
  if ( percentage*multiplier >= 100.0f )
  {
    oss.precision( 0 );
  }

  unsigned int percent = static_cast<unsigned int>( percentage );
  if( use_colors )
  {
    if( use_powerline_right )
    {
      powerline( oss, cpu_percentage_lut[percent], POWERLINE_RIGHT );
    }
    else if( use_powerline_left )
    {
      powerline( oss, cpu_percentage_lut[percent], POWERLINE_LEFT );
    }
    else
    {
      powerline( oss, cpu_percentage_lut[percent], NONE );
    }
  }

  if( graph_lines > 0)
  {
    oss << " [";
    oss << get_graph_by_percentage( unsigned( percentage ), graph_lines );
    oss << "]";
  }
  oss.width( 5 );
  oss << percentage * multiplier;
  oss << "%";
  if( use_colors )
  {
    if( use_powerline_left )
    {
      powerline( oss, cpu_percentage_lut[percent], POWERLINE_LEFT, true );
    }
    else if( !use_powerline_right )
    {
      oss << "#[fg=default,bg=default]";
    }
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
    << "-c, --colors\n"
    << "\tUse tmux colors in output\n"
    << "-p, --powerline-left\n"
    << "\tUse powerline left symbols throughout the output, enables --colors\n"
    << "-q, --powerline-right\n"
    << "\tUse powerline right symbols throughout the output, enables --colors\n"
    << "-i <value>, --interval <value>\n"
    << "\tSet tmux status refresh interval in seconds. Default: 1 second\n"
    << "-g <value>, --graph-lines <value>\n"
    << "\tSet how many lines should be drawn in a graph. Default: 10\n"
    << "-m <value>, --mem-mode <value>\n"
    << "\tSet memory display mode. 0: Default, 1: Free memory, 2: Usage percent.\n"
    << "-t <value>, --cpu-mode <value>\n"
    << "\tSet cpu % display mode. 0: Default max 100%, 1: Max 100% * number of threads. \n"
    << "-a <value>, --averages-count <value>\n"
    << "\tSet how many load-averages should be drawn. Default: 3\n"
    << endl;
}

int main( int argc, char** argv )
{
  unsigned cpu_usage_delay = 990000;
  short averages_count = 3;
  short graph_lines = 10; // max 32767 should be enough
  bool use_colors = false;
  bool use_powerline_left = false;
  bool use_powerline_right = false;
  MEMORY_MODE mem_mode = MEMORY_MODE_DEFAULT;
  CPU_MODE cpu_mode = CPU_MODE_DEFAULT;

  static struct option long_options[] =
  {
    // Struct is a s follows:
    //   const char * name, int has_arg, int *flag, int val
    // if *flag is null, val is option identifier to use in switch()
    // otherwise it's a value to set the variable *flag points to
    { "help", no_argument, NULL, 'h' },
    { "colors", no_argument, NULL, 'c' },
    { "powerline-left", no_argument, NULL, 'p' },
    { "powerline-right", no_argument, NULL, 'q' },
    { "interval", required_argument, NULL, 'i' },
    { "graph-lines", required_argument, NULL, 'g' },
    { "mem-mode", required_argument, NULL, 'm' },
    { "cpu-mode", required_argument, NULL, 't' },
    { "averages-count", required_argument, NULL, 'a' },
    { 0, 0, 0, 0 } // used to handle unknown long options
  };

  int c;
  // while c != -1
  while( (c = getopt_long( argc, argv, "hi:cpqg:m:a:t:", long_options, NULL) ) != -1 )
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
      case 'p': // --powerline-left
        use_colors = true;
        use_powerline_left = true;
        break;
      case 'q': // --powerline-right
        use_colors = true;
        use_powerline_right = true;
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
      case 'm': // --mem-mode, -m
        if( atoi( optarg ) < 0 )
          {
            std::cerr << "Memory mode argument must be zero or greater.\n";
            return EXIT_FAILURE;
          }
        mem_mode = static_cast< MEMORY_MODE >( atoi( optarg ) );
        break;
      case 't': // --cpu-mode, -t
        if( atoi( optarg ) < 0 )
          {
            std::cerr << "CPU mode argument must be zero or greater.\n";
            return EXIT_FAILURE;
          }
        cpu_mode = static_cast< CPU_MODE >( atoi( optarg ) );
        break;
      case 'a': // --averages-count, -a
        if( atoi( optarg ) < 0 || atoi( optarg ) > 3 )
          {
            std::cerr << "Valid averages-count arguments are: 0, 1, 2, 3\n";
            return EXIT_FAILURE;
          }
        averages_count = atoi( optarg );
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

  MemoryStatus memory_status;
  mem_status( memory_status );
  std::cout << mem_string( memory_status, mem_mode, use_colors, use_powerline_left, use_powerline_right )
    << cpu_string( cpu_mode, cpu_usage_delay, graph_lines, use_colors, use_powerline_left, use_powerline_right )
    << load_string( use_colors, use_powerline_left, use_powerline_right, averages_count );

  std::cout << std::endl;

  return EXIT_SUCCESS;
}

