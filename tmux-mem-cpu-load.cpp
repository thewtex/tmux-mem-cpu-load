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

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h> // sleep

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

std::string load_string()
{
	std::ifstream loadavg_file( "/proc/loadavg" );
	std::string load_line;
	std::getline( loadavg_file, load_line );
	loadavg_file.close();

	return load_line.substr( 0, 14 );
}

int main(int argc, char** argv)
{
	unsigned int cpu_usage_delay = 900000;
	unsigned int graph_lines = 10;
	try
	{
		std::istringstream iss;
		iss.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
		if( argc > 1 )
		{
			iss.str( argv[1] );
			unsigned int status_interval;
			iss >> status_interval;
			cpu_usage_delay = status_interval * 1000000 - 100000;
		}

		if( argc > 2 )
		{
			iss.str( argv[2] );
			iss.clear();
			iss >> graph_lines;
		}
	}
	catch(const std::exception &e)
	{
		std::cerr << "Usage: " << argv[0] << " [tmux_status-interval(seconds)] [graph lines]" << std::endl;
		return 1;
	}

	std::cout << mem_string() << ' ' << cpu_string( cpu_usage_delay, graph_lines ) << ' ' << load_string();

	return 0;
}

