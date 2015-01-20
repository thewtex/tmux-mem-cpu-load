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

#include <sstream>
#include <fstream>
#include <sys/sysinfo.h>

#include "memory.h"
#include "luts.h"
#include "conversions.h"

std::string mem_string( bool use_colors = false )
{
  using std::string;
  using std::ifstream;
  using std::stoi;

  std::ostringstream oss;

  string line, substr;
  size_t substr_start;
  size_t substr_len;

  unsigned int total_mem, used_mem;

  /* Since linux uses some RAM for disk caching, the actuall used ram is lower 
   * than what sysinfo(), top or free reports. htop reports the usage in a 
   * correct way. The memory used for caching doesn't count as used, since it
   * can be freed in any moment. Usually it hapens automatically, when an
   * application requests memory.
   * In order to calculate the ram that's actually used we need to use the 
   * following formula:
   *    total_ram - free_ram - buffered_ram - cached_ram
   *
   * example data, junk removed, with comments added:
   *
   * MemTotal:        61768 kB    old
   * MemFree:          1436 kB    old
   * MemAvailable     ????? kB    ??
   * MemShared:           0 kB    old (now always zero; not calculated)
   * Buffers:          1312 kB    old
   * Cached:          20932 kB    old
   * SwapTotal:      122580 kB    old
   * SwapFree:        60352 kB    old
   */

  ifstream memory_info("/proc/meminfo");

  while( getline( memory_info, line ) )
  {
    substr_start = 0;
    substr_len = line.find_first_of( ':' );
    substr = line.substr( substr_start, substr_len );
    substr_start = line.find_first_not_of( " ", substr_len + 1 );
    substr_len = line.find_first_of( 'k' ) - substr_start;
    if( substr.compare( "MemTotal" ) == 0 )
    {
      // get total memory
      total_mem = stoi( line.substr( substr_start, substr_len ) );
    }
    else if( substr.compare( "MemFree" ) == 0 )
    {
      used_mem = total_mem - stoi( line.substr( substr_start, substr_len ) );
    }
    else if( substr.compare( "Buffers" ) == 0 ||
             substr.compare( "Cached" ) == 0 )
    {
      used_mem -= stoi( line.substr( substr_start, substr_len ) );
    }
  }

  if( use_colors )
  {
    oss << mem_lut[(100 * used_mem) / total_mem];
  }

  // we want megabytes on output, but since the values already come as 
  // kilobytes we need to divide them by 1024 only once, thus we use
  // KILOBYTES
  oss << convert_unit(used_mem, MEGABYTES, KILOBYTES) << '/' 
    << convert_unit(total_mem, MEGABYTES, KILOBYTES) << "MB";

  if( use_colors )
  {
    oss << "#[fg=default,bg=default]";
  }

  return oss.str();
}

