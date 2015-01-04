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
    {
    oss << "#[fg=default,bg=default]";
    }

  return oss.str();
}


std::string mem_string( bool use_colors )
{
  std::ostringstream oss;
#if defined(BSD_BASED) || (defined(__APPLE__) && defined(__MACH__))
// OSX or BSD based system, use BSD APIs instead

#if defined(__APPLE__) && defined(__MACH__)
  // These values are in bytes
  int64_t total_mem;
  int64_t used_mem;
  int64_t unused_mem;
  vm_size_t page_size;
  mach_port_t mach_port;
  mach_msg_type_number_t count;
  vm_statistics_data_t vm_stats;

  // Get total physical memory
  int mib[2];
  mib[0] = CTL_HW;
  mib[1] = HW_MEMSIZE;
  size_t length = sizeof(int64_t);
  sysctl(mib, 2, &total_mem, &length, NULL, 0);

  mach_port = mach_host_self();
  count = sizeof(vm_stats) / sizeof(natural_t);
  if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
    KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO, (host_info_t)&vm_stats, &count))
    {
    unused_mem = (int64_t)vm_stats.free_count * (int64_t)page_size;

    used_mem = ((int64_t)vm_stats.active_count +
    (int64_t)vm_stats.inactive_count +
    (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
    }

  // To kilobytes
#endif // Apple
  // TODO BSD

  used_mem /= 1024;
  total_mem /= 1024;

#else // Linux
  unsigned int total_mem;
  unsigned int used_mem;
  unsigned int unused_mem;
  size_t line_start_pos;
  size_t line_end_pos;
  std::istringstream iss;
  std::string mem_line;

  std::ifstream meminfo_file( "/proc/meminfo" );
  getline( meminfo_file, mem_line );
  line_start_pos = mem_line.find_first_of( ':' );
  line_start_pos++;
  line_end_pos = mem_line.find_first_of( 'k' );
  iss.str( mem_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
  iss >> total_mem;

  used_mem = total_mem;

  for( unsigned int i = 0; i < 3; i++ )
    {
    getline( meminfo_file, mem_line );
    // accomodate MemAvailable potentially being in lines 2-4 of /proc/meminfo
    // did this in a way to not break the original logic of the loop
    if( mem_line.find("MemAvailable") == 0 )
        {
        i--;
        }
    else
        {
        line_start_pos = mem_line.find_first_of( ':' );
        line_start_pos++;
        line_end_pos = mem_line.find_first_of( 'k' );
        iss.str( mem_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
        iss >> unused_mem;
        used_mem -= unused_mem;
        }
    }
  meminfo_file.close();

#endif // platform

  if( use_colors )
    {
    oss << mem_lut[(100 * used_mem) / total_mem];
    }
  oss << used_mem / 1024 << '/' << total_mem / 1024 << "MB";
  if( use_colors )
    {
    oss << "#[fg=default,bg=default]";
    }

  return oss.str();
}


std::string load_string( bool use_colors )
{
  std::ostringstream oss;

#if defined(BSD_BASED) || (defined(__APPLE__) && defined(__MACH__))
  // Both apple and BSD style systems have these api calls

  // Only get 3 load averages
  int nelem = 3;
  double averages[3];
  // based on: http://www.opensource.apple.com/source/Libc/Libc-262/gen/getloadavg.c
  if( getloadavg(averages, nelem) < 0 )
    {
    oss << "0.00 0.00 0.00"; // couldn't get averages.
    }
  else
    {
    for(int i = 0; i < nelem; ++i)
      {
      // Round to nearest, make sure this is only a 0.00 value not a 0.0000
      float avg = floorf(static_cast<float>(averages[i]) * 100 + 0.5) / 100;
      oss << avg << " ";
      }
    }
  std::string load_line( oss.str() );
  oss.str( "" );

#else // Linux
  std::ifstream loadavg_file( "/proc/loadavg" );
  std::string load_line;
  std::getline( loadavg_file, load_line );
  loadavg_file.close();

#endif // platform

  if( use_colors )
    {
    std::ifstream stat_file( "/proc/stat" );
    std::string stat_line;
    std::getline( stat_file, stat_line );
    // Likely does not work on BSD, but not tested
    unsigned int number_of_cpus = sysconf( _SC_NPROCESSORS_ONLN );

    std::istringstream iss( load_line.substr( 0, 4 ) );
    float recent_load;
    iss >> recent_load;
    // colors range from zero to twice the number of cpu's for the most recent
    // load metric
    unsigned int load_percent = static_cast< unsigned int >( recent_load / number_of_cpus * 0.5f * 100.0f );
    if( load_percent > 100 )
      {
      load_percent = 100;
      }
    oss << load_lut[load_percent];
    }

  oss << load_line.substr( 0, 14 );
  if( use_colors )
    {
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
        std::cerr << "Status interval argument must be one or greater." 
		          << std::endl;
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
        std::cerr << "Graph lines argument must be one or greater." 
		          << std::endl;
        return EXIT_FAILURE;
        }
      }
    }
  catch(const std::exception &e)
    {
    std::cerr << "Usage: " << argv[0] 
	          << " [--colors] [tmux_status-interval(seconds)] [graph lines]" 
			  << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << mem_string( use_colors ) << ' ' 
            << cpu_string( cpu_usage_delay, graph_lines, use_colors ) << ' ' 
			<< load_string( use_colors );

  return EXIT_SUCCESS;
}

