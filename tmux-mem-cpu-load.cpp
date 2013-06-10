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
#include <math.h> // for floorf

// Apple specific.
#if defined(__APPLE__) && defined(__MACH__)
// Mach kernel includes for getting memory and CPU statistics
# include <mach/vm_statistics.h>
# include <mach/processor_info.h>
# include <mach/mach_types.h>
# include <mach/mach_init.h>
# include <mach/mach_host.h>
# include <mach/host_info.h>
# include <mach/mach_error.h>
# include <mach/vm_map.h>
# include <mach/mach.h>
# include <sys/sysctl.h> // for sysctl
# include <sys/types.h> // for integer types
#endif

// if we are on a BSD system
#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
// TODO: Includes and *BSD support
# define BSD_BASED 1
#endif

// Function declarations for global use.
float cpu_percentage( unsigned int cpu_usage_delay );
std::string mem_string();
std::string load_string();

// This function is platform independent and therefore can be placed at the top
std::string cpu_string( unsigned int cpu_usage_delay, unsigned int graph_lines )
{
	std::string meter( graph_lines + 2, ' ' );
	meter[0] = '[';
	meter[meter.length() - 1] = ']';
	int meter_count = 0;
	float percentage;
	std::ostringstream oss;
	oss.precision( 1 );
	oss.setf( std::ios::fixed | std::ios::right );

	percentage = cpu_percentage( cpu_usage_delay );
	float meter_step = 99.9 / graph_lines;
	meter_count = 1;
	while(meter_count*meter_step < percentage)
	{
		meter[meter_count] = '|';
		meter_count++;
	}

	oss << meter;
	oss.width( 5 );
	oss << percentage;
	oss << "%";

	return oss.str();
}

#if defined(BSD_BASED) || (defined(__APPLE__) && defined(__MACH__))
// OSX or BSD based system, use BSD APIs instead

#if defined(__APPLE__) && defined(__MACH__)
std::string mem_string()
{
	// These values are in bytes
	int64_t total_mem;
	int64_t used_mem;
	int64_t unused_mem;
	// blah
	vm_size_t page_size;
	mach_port_t mach_port;
	mach_msg_type_number_t count;
	vm_statistics_data_t vm_stats;
	std::ostringstream oss;

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

	oss << used_mem / 1024 / 1024 << '/' << total_mem / 1024 / 1024 << "MB";

	return oss.str();
}

// See: http://www.opensource.apple.com/source/xnu/xnu-201/osfmk/mach/host_info.h
// and: http://web.mit.edu/darwin/src/modules/xnu/osfmk/man/
host_cpu_load_info_data_t _get_cpu_percentage()
{
	kern_return_t		error;
	mach_msg_type_number_t	count;
	host_cpu_load_info_data_t	r_load;
	mach_port_t		mach_port;

	count = HOST_CPU_LOAD_INFO_COUNT;
	mach_port = mach_host_self();
	error = host_statistics(mach_port, HOST_CPU_LOAD_INFO, (host_info_t)&r_load, &count);
	if (error != KERN_SUCCESS)
		return host_cpu_load_info_data_t();

	return r_load;
}

float cpu_percentage( unsigned int cpu_usage_delay )
{
	// Get the load times from the XNU kernel
	host_cpu_load_info_data_t load1 = _get_cpu_percentage();
	usleep(cpu_usage_delay);
	host_cpu_load_info_data_t load2 = _get_cpu_percentage();

	// Current load times
	unsigned long long current_user    = load1.cpu_ticks[CPU_STATE_USER];
	unsigned long long current_system  = load1.cpu_ticks[CPU_STATE_SYSTEM];
	unsigned long long current_nice    = load1.cpu_ticks[CPU_STATE_NICE];
	unsigned long long current_idle    = load1.cpu_ticks[CPU_STATE_IDLE];
	// Next load times
	unsigned long long next_user       = load2.cpu_ticks[CPU_STATE_USER];
	unsigned long long next_system     = load2.cpu_ticks[CPU_STATE_SYSTEM];
	unsigned long long next_nice       = load2.cpu_ticks[CPU_STATE_NICE];
	unsigned long long next_idle       = load2.cpu_ticks[CPU_STATE_IDLE];
	// Difference between the two
	unsigned long long diff_user       = next_user - current_user;
	unsigned long long diff_system     = next_system - current_system;
	unsigned long long diff_nice       = next_nice - current_nice;
	unsigned long long diff_idle       = next_idle - current_idle;

	return static_cast<float>(diff_user + diff_system + diff_nice)/static_cast<float>(diff_user + diff_system + diff_nice + diff_idle)*100.0;
}

#else // APPLE

// BSD type systems (FreeBSD/NetBSD/OpenBSD)
// Incomplete
float cpu_percentage( unsigned int cpu_usage_delay )
{
	return 0.0f;
}

std::string mem_string()
{
	return "0/0 MB";
}

#endif // APPLE

// Both apple and BSD style systems have these api calls
std::string load_string()
{
	std::stringstream ss;
	// Only get 3 load averages
	int nelem = 3;
	double averages[3];
	// based on: http://www.opensource.apple.com/source/Libc/Libc-262/gen/getloadavg.c
	if(getloadavg(averages, nelem) < 0)
		ss << "0.00 0.00 0.00"; // couldn't get averages.
	else
	{
		for(int i = 0; i < nelem; ++i)
		{
			// Round to nearest, make sure this is only a 0.00 value not a 0.0000
			float avg = floorf(static_cast<float>(averages[i]) * 100 + 0.5) / 100;
			ss << avg << " ";
		}
	}

	return ss.str();
}

#else // APPLE || BSD_BASED

// Linux based system, if it's windows, too much work, they can deal with their compile errors - CMD sucks anyway.
std::string mem_string()
{
	unsigned int total_mem;
	unsigned int used_mem;
	unsigned int unused_mem;
	size_t line_start_pos;
	size_t line_end_pos;
	std::istringstream iss;
	std::ostringstream oss;
	std::string mem_line;

	std::ifstream meminfo_file( "/proc/meminfo" );
	std::getline( meminfo_file, mem_line );
	line_start_pos = mem_line.find_first_of( ':' );
	line_start_pos++;
	line_end_pos = mem_line.find_first_of( 'k' );
	iss.str( mem_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
	iss >> total_mem;

	used_mem = total_mem;

	for( unsigned int i = 0; i < 3; i++ )
	{
		std::getline( meminfo_file, mem_line );
		line_start_pos = mem_line.find_first_of( ':' );
		line_start_pos++;
		line_end_pos = mem_line.find_first_of( 'k' );
		iss.str( mem_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
		iss >> unused_mem;
		used_mem -= unused_mem;
	}
	meminfo_file.close();

	oss << used_mem / 1024 << '/' << total_mem / 1024 << "MB";

	return oss.str();
}

float cpu_percentage( unsigned int cpu_usage_delay )
{
	std::string stat_line;
	size_t line_start_pos;
	size_t line_end_pos;
	unsigned long long current_user;
	unsigned long long current_system;
	unsigned long long current_nice;
	unsigned long long current_idle;
	unsigned long long next_user;
	unsigned long long next_system;
	unsigned long long next_nice;
	unsigned long long next_idle;
	unsigned long long diff_user;
	unsigned long long diff_system;
	unsigned long long diff_nice;
	unsigned long long diff_idle;
	std::istringstream iss;

	std::ifstream stat_file("/proc/stat");
	std::getline(stat_file, stat_line);
	stat_file.close();

	// skip "cpu"
	line_start_pos = stat_line.find_first_not_of(" ", 3);
	line_end_pos = stat_line.find_first_of(' ', line_start_pos);
	line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);
	line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);
	line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);

	iss.str( stat_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
	iss >> current_user >> current_nice >> current_system >> current_idle;
	iss.clear();

	usleep( cpu_usage_delay );

	stat_file.open("/proc/stat");
	std::getline(stat_file, stat_line);
	stat_file.close();

	// skip "cpu"
	line_start_pos = stat_line.find_first_not_of(" ", 3);
	line_end_pos = stat_line.find_first_of(' ', line_start_pos);
	line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);
	line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);
	line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);

	iss.str( stat_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
	iss >> next_user >> next_nice >> next_system >> next_idle;
	iss.clear();

	diff_user   = next_user - current_user;
	diff_system = next_system - current_system;
	diff_nice   = next_nice - current_nice;
	diff_idle   = next_idle - current_idle;

	return static_cast<float>(diff_user + diff_system + diff_nice)/static_cast<float>(diff_user + diff_system + diff_nice + diff_idle)*100.0;
}

std::string load_string()
{
	std::ifstream loadavg_file( "/proc/loadavg" );
	std::string load_line;
	std::getline( loadavg_file, load_line );
	loadavg_file.close();

	return load_line.substr( 0, 14 );
}

#endif // BSD

int main(int argc, char** argv)
{
  unsigned int cpu_usage_delay = 900000;
  int graph_lines = 10;
  bool use_colors = false;
  try
    {
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
      unsigned int status_interval;
      iss >> status_interval;
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
        std::cerr << "graph lines argument must be one or greater." << std::endl;
        return 1;
        }
      }
    }
  catch(const std::exception &e)
    {
    std::cerr << "Usage: " << argv[0] << " [--colors] [tmux_status-interval(seconds)] [graph lines]" << std::endl;
    return 1;
    }

  std::cout << mem_string( use_colors ) << ' ' << cpu_string( cpu_usage_delay, graph_lines, use_colors ) << ' ' << load_string( use_colors );

  return 0;
}

