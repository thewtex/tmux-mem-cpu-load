/*
 * Copyright 2012 Matthew McCormick
 * Copyright 2013 Justin Crawford <Justasic@gmail.com>
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

#include <mach/mach.h>
#include <unistd.h> // usleep()

#include "cpu.h"

uint32_t get_cpu_count()
{
  return sysconf( _SC_NPROCESSORS_ONLN );
}

// OSX or BSD based system, use BSD APIs instead
// See: http://www.opensource.apple.com/source/xnu/xnu-201/osfmk/mach/host_info.h
// and: http://web.mit.edu/darwin/src/modules/xnu/osfmk/man/

host_cpu_load_info_data_t _get_cpu_percentage()
{
  kern_return_t              error;
  mach_msg_type_number_t     count;
  host_cpu_load_info_data_t  r_load;
  mach_port_t                mach_port;

  count = HOST_CPU_LOAD_INFO_COUNT;
  mach_port = mach_host_self();
  error = host_statistics(mach_port, HOST_CPU_LOAD_INFO, 
      ( host_info_t )&r_load, &count );

  if ( error != KERN_SUCCESS )
  {
    return host_cpu_load_info_data_t();
  }

  return r_load;
}

float cpu_percentage( unsigned int cpu_usage_delay )
{
  // Get the load times from the XNU kernel
  host_cpu_load_info_data_t load1 = _get_cpu_percentage();
  usleep( cpu_usage_delay );
  host_cpu_load_info_data_t load2 = _get_cpu_percentage();

  // Current load times
  unsigned long long current_user = load1.cpu_ticks[CP_USER];
  unsigned long long current_system = load1.cpu_ticks[CP_SYS];
  unsigned long long current_nice = load1.cpu_ticks[CP_NICE];
  unsigned long long current_idle = load1.cpu_ticks[CP_IDLE];
  // Next load times
  unsigned long long next_user = load2.cpu_ticks[CP_USER];
  unsigned long long next_system = load2.cpu_ticks[CP_SYS];
  unsigned long long next_nice = load2.cpu_ticks[CP_NICE];
  unsigned long long next_idle = load2.cpu_ticks[CP_IDLE];
  // Difference between the two
  unsigned long long diff_user = next_user - current_user;
  unsigned long long diff_system = next_system - current_system;
  unsigned long long diff_nice = next_nice - current_nice;
  unsigned long long diff_idle = next_idle - current_idle;

  return static_cast<float>( diff_user + diff_system + diff_nice ) / 
    static_cast<float>( diff_user + diff_system + diff_nice + diff_idle ) * 
    100.0;
}

