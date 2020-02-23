/* vim: tabstop=2 shiftwidth=2 expandtab textwidth=80 linebreak wrap
 *
 * Copyright 2012 Matthew McCormick
 * Copyright 2015 Pawel 'l0ner' Soltys
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

#include <string>
#include <fstream>
#include <unistd.h> // usleep

#include "cpu.h"
#include "luts.h"

uint32_t get_cpu_count()
{
  return sysconf( _SC_NPROCESSORS_ONLN );
}

float cpu_percentage( unsigned cpu_usage_delay )
{
  std::string line;
  size_t substr_start = 0;
  size_t substr_len;

  // cpu stats
  // user, nice, system, idle
  // in that order
  unsigned long long stats[CP_STATES];

  std::ifstream stat_file( "/proc/stat" );
  getline( stat_file, line );
  stat_file.close();

  // skip "cpu"
  substr_len = line.find_first_of( " ", 3 );
  // parse cpu line
  for( unsigned i=0; i < 4; i++ )
  {
    substr_start = line.find_first_not_of( " ", substr_len );
    substr_len   = line.find_first_of( " ", substr_start );
    stats[i] = std::stoll( line.substr( substr_start, substr_len ) );
  }

  usleep( cpu_usage_delay );

  stat_file.open( "/proc/stat" );
  getline( stat_file, line );
  stat_file.close();

  // skip "cpu"
  substr_len = line.find_first_of( " ", 3 );
  // parse cpu line
  for( unsigned i=0; i < 4; i++ )
  {
    substr_start = line.find_first_not_of( " ", substr_len );
    substr_len   = line.find_first_of    ( " ", substr_start );
    stats[i] = std::stoll( line.substr( substr_start, substr_len ) ) - stats[i];
  }

  return static_cast<float>( 
    stats[CP_USER] + stats[CP_NICE] + stats[CP_SYS]
    ) / static_cast<float>( 
        stats[CP_USER] + stats[CP_NICE] + stats[CP_SYS] + stats[CP_IDLE] 
    ) * 100.0;
}

