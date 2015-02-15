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

#include <string>
#include <sstream>
#include <mach/mach.h>
#include <sys/sysctl.h> // for sysctl

#include "memory.h"
#include "luts.h"
#include "conversions.h"

std::string mem_string( bool use_colors )
{
  std::ostringstream oss;

  // These values are in bytes
  u_int64_t total_mem;
  u_int64_t used_mem;
  //u_int64_t unused_mem;

  vm_size_t page_size;
  vm_statistics_data_t vm_stats;

  // Get total physical memory
  int mib[] = { CTL_HW, HW_MEMSIZE };
  size_t length = sizeof( total_mem );
  sysctl( mib, 2, &total_mem, &length, NULL, 0 );

  mach_port_t mach_port = mach_host_self();
  mach_msg_type_number_t count = sizeof( vm_stats ) / sizeof( natural_t );
  if( KERN_SUCCESS == host_page_size( mach_port, &page_size ) &&
      KERN_SUCCESS == host_statistics( mach_port, HOST_VM_INFO, 
        ( host_info_t )&vm_stats, &count ) 
    )
  {
    //unused_mem = static_cast<u_int64_t>( vm_stats.free_count * page_size );

    used_mem = static_cast<u_int64_t>(
        ( vm_stats.active_count + vm_stats.wire_count ) * page_size);
  }

  if( use_colors )
  {
    oss << mem_lut[( 100 * used_mem ) / total_mem];
  }

  oss << convert_unit( used_mem, MEGABYTES ) << '/' 
    << convert_unit( total_mem, MEGABYTES ) << "MB";

  if( use_colors )
  {
    oss << "#[fg=default,bg=default]";
  }

  return oss.str();
}
