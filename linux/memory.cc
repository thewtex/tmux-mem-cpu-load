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
#include "conversions.h"

void mem_status( MemoryStatus & status )
{
  std::string line;
  std::string substr;
  size_t substr_start;
  size_t substr_len;

  unsigned int total_mem;
  unsigned int used_mem;

  /* Since linux uses some RAM for disk caching, the actuall used ram is lower
   * than what sysinfo(), top or free reports. htop reports the usage in a
   * correct way. The memory used for caching doesn't count as used, since it
   * can be freed in any moment. Usually it hapens automatically, when an
   * application requests memory.
   * In order to calculate the ram that's actually used we need to use the
   * following formula:
   *    total_ram + shmem - free_ram - buffered_ram - cached_ram - srclaimable
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

  std::ifstream memory_info("/proc/meminfo");

  while( std::getline( memory_info, line ) )
  {
    substr_start = 0;
    substr_len = line.find_first_of( ':' );
    substr = line.substr( substr_start, substr_len );
    substr_start = line.find_first_not_of( " ", substr_len + 1 );
    substr_len = line.find_first_of( 'k' ) - substr_start;
    if( substr.compare( "MemTotal" ) == 0 )
    {
      // get total memory
      total_mem = stol( line.substr( substr_start, substr_len ) );
    }
    else if( substr.compare( "MemFree" ) == 0 )
    {
      used_mem = total_mem - stol( line.substr( substr_start, substr_len ) );
    }
    else if( substr.compare( "Shmem" ) == 0 )
    {
      used_mem += stol( line.substr( substr_start, substr_len ) );
    }
    else if( substr.compare( "Buffers" ) == 0 ||
             substr.compare( "Cached" ) == 0  ||
             substr.compare( "SReclaimable" ) == 0 )
    {
      used_mem -= stol( line.substr( substr_start, substr_len ) );
    }
  }

  // we want megabytes on output, but since the values already come as
  // kilobytes we need to divide them by 1024 only once, thus we use
  // KILOBYTES
  status.used_mem = convert_unit(static_cast< float >( used_mem ), MEGABYTES, KILOBYTES);
  status.total_mem = convert_unit(static_cast< float >( total_mem ), MEGABYTES, KILOBYTES);
}

