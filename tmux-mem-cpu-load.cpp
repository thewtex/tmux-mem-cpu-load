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

#include "luts.h"

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
  getline(stat_file, stat_line);
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
  getline(stat_file, stat_line);
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

std::string cpu_string( unsigned int cpu_usage_delay,
  unsigned int graph_lines,
  bool use_colors = false )
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
  while( meter_count*meter_step < percentage )
    {
    meter[meter_count] = '|';
    meter_count++;
    }

  if( use_colors )
    {
    oss << cpu_percentage_lut[static_cast<unsigned int>( percentage )];
    }
  oss << meter;
  oss.width( 5 );
  oss << percentage;
  oss << "%";
  if( use_colors )
    {
    oss << "#[fg=default,bg=default]";
    }

  return oss.str();
}

std::string mem_string( bool use_colors = false )
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
    line_start_pos = mem_line.find_first_of( ':' );
    line_start_pos++;
    line_end_pos = mem_line.find_first_of( 'k' );
    iss.str( mem_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
    iss >> unused_mem;
    used_mem -= unused_mem;
    }
  meminfo_file.close();

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

std::string load_string( bool use_colors = false )
{
  std::ifstream loadavg_file( "/proc/loadavg" );
  std::string load_line;
  std::getline( loadavg_file, load_line );
  loadavg_file.close();
  if( use_colors )
    {
    std::ostringstream oss;
    std::ifstream stat_file( "/proc/stat" );
    std::string stat_line;
    std::getline( stat_file, stat_line );
    unsigned int number_of_cpus = 0;
    std::getline( stat_file, stat_line );
    do
      {
      ++number_of_cpus;
      std::getline( stat_file, stat_line );
      }
    while( stat_line.compare( 0, 3, "cpu" ) == 0 && stat_file.good() );
    stat_file.close();

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
    oss << load_line.substr( 0, 14 );
    oss << "#[fg=default,bg=default]";

    return oss.str();
    }

  return load_line.substr( 0, 14 );
}

int main(int argc, char** argv)
{
  unsigned int cpu_usage_delay = 900000;
  unsigned int graph_lines = 10;
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

