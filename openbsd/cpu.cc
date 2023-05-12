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

#include <unistd.h> // usleep

#include <sys/sysctl.h>
#include <sys/types.h>

#include "error.h"
#include "cpu.h"

uint32_t get_cpu_count()
{
  int cpu_count = 1; // default to 1
  int mib[2] = { CTL_HW, HW_NCPUONLINE };
  size_t len = sizeof( cpu_count );

  if( sysctl( mib, 2, &cpu_count, &len, NULL, 0 ) < 0 )
  {
    error( "sysctl: error getting cpu count" );
  }

  return static_cast<uint32_t>( cpu_count );
}

float cpu_percentage( unsigned int cpu_usage_delay )
{
  int cpu_ctl[] = { CTL_KERN, KERN_CPTIME };

  u_long load1[CP_STATES];
  u_long load2[CP_STATES];

  size_t size = sizeof( load1 );

  // get cpu times
  if( sysctl( cpu_ctl, 2, &load1, &size, NULL, 0 ) < 0 )
  {
    error( "sysctl: error getting initial cpu stats" );
  }

  usleep( cpu_usage_delay );

  size = sizeof( load2 );

  // update cpu times
  if( sysctl( cpu_ctl, 2, &load2, &size, NULL, 0 ) < 0 )
  {
    error( "sysctl: error getting updated cpu stats" );
  }

  // Current load times
  unsigned long long current_user = load1[CP_USER];
  unsigned long long current_system = load1[CP_SYS];
  unsigned long long current_nice = load1[CP_NICE];
  unsigned long long current_idle = load1[CP_IDLE];
  // Next load times
  unsigned long long next_user = load2[CP_USER];
  unsigned long long next_system = load2[CP_SYS];
  unsigned long long next_nice = load2[CP_NICE];
  unsigned long long next_idle = load2[CP_IDLE];
  // Difference between the two
  unsigned long long diff_user = next_user - current_user;
  unsigned long long diff_system = next_system - current_system;
  unsigned long long diff_nice = next_nice - current_nice;
  unsigned long long diff_idle = next_idle - current_idle;

  return static_cast<float>( diff_user + diff_system + diff_nice ) /
    static_cast<float>( diff_user + diff_system + diff_nice + diff_idle ) *
    100.0;
}
